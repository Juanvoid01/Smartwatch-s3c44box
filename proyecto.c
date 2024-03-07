#include <s3c44b0x.h>
#include <s3cev40.h>
#include <common_types.h>
#include <leds.h>
#include <system.h>
#include <leds.h>
#include <segs.h>
#include <uart.h>
#include <pbs.h>
#include <timers.h>
#include <rtc.h>
#include <lcd.h>
#include <ts.h>
#include "sensorsEmulator.h"
#include "taskCalculos.h"
#include "menusLCD.h"
#include "audio.h"
#include "taskKeypad.h"

#define TICKS_PER_SEC   (100)

static void beatHandler( void ) __attribute__ ((interrupt ("IRQ")));
static void stepHandler( void ) __attribute__ ((interrupt ("IRQ")));

/*******************************************************************/

volatile static boolean newBeat = FALSE;
volatile static boolean newStep = FALSE;

/*******************************************************************/

/* Declaración de fifo de punteros a funciones */

#define BUFFER_LEN   (512)

typedef void (*pf_t)(void);


typedef struct fifo {
    uint16 head;
    uint16 tail;
    uint16 size;
    pf_t buffer[BUFFER_LEN];
} fifo_t;

void fifo_init( void );
void fifo_enqueue( pf_t pf );
pf_t fifo_dequeue( void );
boolean fifo_is_empty( void );
boolean fifo_is_full( void );

/* Declaración de recursos */

volatile fifo_t fifo;

static rtc_time_t rtc_time;

/* Declaración de RTI */

void isr_tick( void ) __attribute__ ((interrupt ("IRQ")));

void ActualizarSensores ( void );

void Task1Calculos( void );
void Task2Pantalla( void );
void Task3Sonido( void );
void Task4Reloj( void );
void Task5KeyPad( void );

void ejecutarAccionesAudio(boolean grabar, Estadisticas_Strings* estadisticas);
void ejecutarAccionesReloj(Estadisticas_Strings* estadisticas);

EstadoPrograma estadoPrograma;
Estadisticas_Strings estadisticas_strings;

boolean cambioEstado;
void main( void )
{
	sys_init();
	leds_init();
    uart0_init();
	leds_init();
	segs_init();
	rtc_init();
	pbs_init();
	keypad_init();
	lcd_init();
	lcd_on();
	lcd_clear();

	estadoPrograma = MENU;
	cambioEstado = FALSE;
    pf_t pf;
    fifo_init();             /* Inicializa cola de funciones */

   /* Ejecuta por primera vez a las funciones para inicializarlas */
    inicializarEstadisticasStrings(&estadisticas_strings);

    Task1Calculos();
    Task2Pantalla();
    Task3Sonido();
    Task4Reloj();
    Task5KeyPad();

    timer0_open_tick( isr_tick, TICKS_PER_SEC );  /* Instala isr_tick como RTI del timer0  */

	startSensorsEmulator( beatHandler, stepHandler, 60, 60 );     // Arranca el emulador de sensores, instalando beatHandler y stepHandler como RTI de los respectivos sensores

    while( 1 )
    {
    	ActualizarSensores();
        sleep();                        /* Entra en estado IDLE, sale por interrupción */
        while( !fifo_is_empty() )
        {
        	ActualizarSensores();
            pf = fifo_dequeue();
            (*pf)();                    /* Las tareas encoladas se ejecutan en esta hebra (background) en orden de encolado */
        }
    }
}

void ActualizarSensores( void )
{

	if( newBeat )
	{
		newBeat = FALSE;
		insertarMedicion(0);
		led_toggle( LEFT_LED );
		Task3Sonido();
	}
	if( newStep )
	{
		newStep = FALSE;
		insertarMedicion(1);
		led_toggle( RIGHT_LED );
	}

}

void Task1Calculos( void )
{
    static boolean init = TRUE;

    if( init )
    {
        init  = FALSE;
        inicializarCalculos();
    }
    else
    {
    	calcularEstadisticas();
    }
}

void Task2Pantalla( void )
{
    static boolean init = TRUE;

    if( init )
    {
        init  = FALSE;

    }
    else
    {
    	int pentera,pdecimal;


    	getPasosMinuto(&pentera);
    	formatearInt(pentera,estadisticas_strings.pasos);
    	getPulsacionesMinuto(&pentera);
    	formatearInt(pentera,estadisticas_strings.pulsaciones);
    	getCalorias(&pentera,&pdecimal);
    	formatearFloat(pentera,pdecimal,estadisticas_strings.calorias);
    	getVelocidad(&pentera,&pdecimal);
    	formatearFloat(pentera,pdecimal,estadisticas_strings.velocidad);
    	getDistancia(&pentera,&pdecimal);
    	formatearFloat(pentera,pdecimal,estadisticas_strings.distancia);
    	getVelocidadMax(&pentera,&pdecimal);
    	formatearFloat(pentera,pdecimal,estadisticas_strings.velocidad_max);

    	if(cambioEstado)
    	{
        	lcd_clear();
        	cambioEstado = FALSE;
    	}


		switch(estadoPrograma)
		{
			case MENU:pintarMenuPrincipal(&estadisticas_strings);
				break;
			case AJUSTES:pintarMenuAjustes(&estadisticas_strings);
				break;
			case AUDIOS:pintarMenuAudios(&estadisticas_strings);
				break;
			case RELOJ:pintarMenuReloj(&estadisticas_strings);
				break;
			default:;
		}

    }
}

void Task3Sonido( void )
{
    static boolean init = TRUE;

    if( init )
    {
        init = FALSE;
        inicializarAudio();
    }
    else
    {
    	int pentera;
    	getPulsacionesMinuto(&pentera);

    	if(pentera >= VALOR_LATIDOS_ALTOS || pentera <= VALOR_LATIDOS_BAJOS)
    	{
    		sonidoLatido();
    	}

		if(estadoPrograma == MENU)
		{
			pintarAlertaCardiaca(pentera);
		}

    }
}

void Task4Reloj( void )
{
    static boolean init = TRUE;

    static char timeString[9];
    static int limiteContador;

    if( init )
    {
        init = FALSE;
        rtc_time_t horaActual;
        horaActual.hour = (__TIME__[0] - '0') * 10 + (__TIME__[1] - '0');
        horaActual.min =  (__TIME__[3] - '0') * 10 + (__TIME__[4] - '0');
        horaActual.sec =  (__TIME__[6] - '0') * 10 + (__TIME__[7] - '0');
        rtc_puttime(&horaActual);
    }
    else
    {

        rtc_gettime( &rtc_time );
        ejecutarAccionesReloj(&estadisticas_strings);

        formatearHora(rtc_time.hour, rtc_time.min, rtc_time.sec, timeString);
        pintarHora(estadoPrograma,timeString);

        limiteContador = 0;
        switch(estadisticas_strings.alarmaSeleccionada)
        {
        case ALARMA_5 : limiteContador = 5;
        	break;
        case ALARMA_30: limiteContador = 30;
        	break;
        case ALARMA_60: limiteContador = 60;
        	break;
        default: limiteContador = 0;
        }
        if(limiteContador != 0)
        {
    		if(estadisticas_strings.contReloj >= limiteContador)
    		{
    			estadisticas_strings.contReloj = 0;
    			estadisticas_strings.alarmaSeleccionada = NINGUNA_ALARMA;
    			sonidoAlarma();
    		}
    		else
    		{
    			estadisticas_strings.contReloj++;
    		}
        }

    }
}

void Task5KeyPad( void )
{
	static boolean init = TRUE;
	static enum { wait_keydown, scan, wait_keyup } keypad_state;
	static uint8 scancode;
	boolean grabar = FALSE;
	if( init )
	{
		init  = FALSE;
		keypad_state = wait_keydown;
	}
	else switch( keypad_state )
	{
		case wait_keydown:
			if( keypad_pressed() )
				keypad_state = scan;
			break;
		case scan:
			scancode = keypad_scan();
			if( scancode != KEYPAD_FAILURE )
			{
				switch(estadoPrograma)
				{
					case MENU:keypadMenu(scancode, &cambioEstado, &estadoPrograma);

						break;
					case AJUSTES:keypadAjustes(scancode,&estadisticas_strings,&cambioEstado,&estadoPrograma);

						break;
					case AUDIOS:keypadAudios(scancode, &estadisticas_strings,&cambioEstado,&estadoPrograma, &grabar);

						ejecutarAccionesAudio(grabar,&estadisticas_strings);

						break;
					case RELOJ:keypadReloj(scancode, &estadisticas_strings, &cambioEstado, &estadoPrograma);
						break;
					default:;
				}
			}
			keypad_state = wait_keyup;
			break;
		case wait_keyup:
			if( !keypad_pressed() )
				keypad_state = wait_keydown;
			break;
	}
}


//************************************************

void beatHandler( void )
{
    newBeat = TRUE;
    I_ISPC  = BIT_BEATEMULATOR;
}

void stepHandler( void )
{
    newStep = TRUE;
    I_ISPC  = BIT_STEPEMULATOR;
}

void isr_tick( void )
{
    static uint16 cont5ticks    = 5;
    static uint16 cont25ticks   = 25;
    static uint16 cont50ticks   = 50;
    static uint16 cont100ticks  = 100;
    static uint16 cont1000ticks = 1000;

    if( !(--cont5ticks) ) //50ms
    {
        cont5ticks = 5;
        fifo_enqueue( Task5KeyPad );
    }
    if( !(--cont25ticks) ) //250ms
    {
    	cont25ticks = 25;
        fifo_enqueue( Task2Pantalla );
    }
    if( !(--cont50ticks) ) //500ms
    {
        cont50ticks = 50;
        fifo_enqueue( Task1Calculos );
    }
    if( !(--cont100ticks) ) //1s
    {
        cont100ticks = 100;
        fifo_enqueue( Task4Reloj );
    }
    if( !(--cont1000ticks) ) //10s
    {
        cont1000ticks = 1000;
    }

    I_ISPC = BIT_TIMER0;
};

//ejecuta acciones sobre la hora que el usuario haya realizado
void ejecutarAccionesReloj(Estadisticas_Strings* estadisticas)
{
	switch(estadisticas->accionReloj)
	{
	case INC_H:rtc_time.hour = rtc_time.hour < 24 ? rtc_time.hour+1 : 24;rtc_puttime(&rtc_time);
		break;
	case INC_M:rtc_time.min = rtc_time.min < 60 ? rtc_time.min+1 : 60;rtc_puttime(&rtc_time);
		break;
	case INC_S:rtc_time.sec = rtc_time.sec < 60 ? rtc_time.sec+1 : 60;rtc_puttime(&rtc_time);
		break;
	case DEC_H:rtc_time.hour = rtc_time.hour > 0 ? rtc_time.hour-1 : 0;rtc_puttime(&rtc_time);
		break;
	case DEC_M:rtc_time.min = rtc_time.min > 0 ? rtc_time.min-1 : 0;rtc_puttime(&rtc_time);
		break;
	case DEC_S:rtc_time.sec = rtc_time.sec > 0 ? rtc_time.sec-1 : 0;rtc_puttime(&rtc_time);
		break;
	case ALARMA5: if(estadisticas->alarmaSeleccionada == NINGUNA_ALARMA)
			 	 	 estadisticas->alarmaSeleccionada = ALARMA_5;
		break;
	case ALARMA30: if(estadisticas->alarmaSeleccionada == NINGUNA_ALARMA)
	 	 	 	 	   estadisticas->alarmaSeleccionada = ALARMA_30;
		break;
	case ALARMA60: if(estadisticas->alarmaSeleccionada == NINGUNA_ALARMA)
	 	 	 	 	  estadisticas->alarmaSeleccionada = ALARMA_60;
		break;
	default:;
	}

	estadisticas->accionReloj = RELOJ_NADA;
}

//ejecuta acciones sobre los audios que el usuario haya seleccionado
void ejecutarAccionesAudio(boolean grabar, Estadisticas_Strings* estadisticas)
{
	if(grabar)
	{
		segs_putchar( 8 );
		pintarSimboloGrabar();
		int8 posAudio = grabarAudio();
		quitarSimboloGrabar();
		segs_off();
		if(posAudio >= 0 && posAudio < 3)
		{
			estadisticas_strings.audiosEstado[posAudio] = PARADO;
		}
		return;
	}

	uint8 i;
	for(i = 0; i < 3; i++)
	{
		if(estadisticas->audiosEstado[i] == LIBRE)
		{
			eliminarAudio(i);
		}
		else if(estadisticas->audiosEstado[i] == SONANDO)
		{
			pintarSimboloEjecutarAudio(i);
			reproducirAudio(i);
			estadisticas->audiosEstado[i] = PARADO;
			break;
		}
		else if(estadisticas->audiosEstado[i] == SONANDOX2)
		{
			pintarSimboloEjecutarAudio(i);
			reproducirAudioX2(i);
			estadisticas->audiosEstado[i] = PARADO;
			break;
		}
		else if(estadisticas->audiosEstado[i] == PARADO)
		{

		}

	}
}


/*******************************************************************/

void fifo_init( void )
{
    fifo.head = 0;
    fifo.tail = 0;
    fifo.size = 0;
}

void fifo_enqueue( pf_t pf )
{
    fifo.buffer[fifo.tail++] = pf;
    if( fifo.tail == BUFFER_LEN )
        fifo.tail = 0;
    INT_DISABLE;
    fifo.size++;
    INT_ENABLE;
}

pf_t fifo_dequeue( void )
{
    pf_t pf;

    pf = fifo.buffer[fifo.head++];
    if( fifo.head == BUFFER_LEN )
        fifo.head = 0;
    INT_DISABLE;
    fifo.size--;
    INT_ENABLE;
    return pf;
}

boolean fifo_is_empty( void )
{
    return (fifo.size == 0);
}

boolean fifo_is_full( void )
{
    return (fifo.size == BUFFER_LEN-1);
}

/*******************************************************************/
