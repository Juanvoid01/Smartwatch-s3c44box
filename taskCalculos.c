/*
 * taskCalculos.c
 *
 *  Created on: 27/12/2023
 *      Author: Juan
 */

#define MEDICIONES_MINUTO 120
#define PASOS_POR_KM 1200  // Número promedio de pasos por kilómetro

//con 3 digitos decimales nos vale 0 - 999
#define QM 10 //representacion Q22.10
#include "taskCalculos.h"
#include <fix_types.h>

typedef struct
{
	int pulsaciones; //latidos/minuto
	int pasosMinuto;
	fix32 calorias;
	fix32 velocidad; //km/h
	fix32 velocidad_max;
	fix32 distancia;

}Estadisticas;


//array circular, una medicion por segundo
Estadisticas estadisticas = {0,0,0,0,0,0};


static int latidosMediciones[20];
static int pasosMediciones[20];

static int numeroMedicion = 0;
static int vueltaCompleta = 0;

static int getParteEntera(int numero);
static int getParteDecimal(int numero);

static void calcularPulsaciones();
static void calcularPasos();
static void calcularVelocidad();

void inicializarCalculos()
{
	estadisticas.pulsaciones = 0;
	estadisticas.velocidad = 0;
	estadisticas.calorias = 0;
	estadisticas.pasosMinuto = 0;
	estadisticas.velocidad_max = 0;
	estadisticas.distancia = 0;

	numeroMedicion = 0;
	vueltaCompleta = 0;
	latidosMediciones[numeroMedicion] = 0;
	pasosMediciones[numeroMedicion] = 0;
}

/*
 * recalcula las pulsaciones por minuto
 * usa el numero de pulsaciones en los ultimos 10 segundos(20 muestras)
 * cada muestra tiene el numero de pulsaciones en 0.5 segundos
 */
static void calcularPulsaciones()
{
	int i, nuevoRitmo = 0;

	if(vueltaCompleta)
	{
		for(i = 0; i < 20; i++)
		{
			nuevoRitmo+= latidosMediciones[i];
		}
		nuevoRitmo = nuevoRitmo * 6;
	}
	else
	{
		//este caso es para cuando no hemos rellenado el buffer completamente
		for(i = 0; i < numeroMedicion; i++)
		{
			nuevoRitmo+= latidosMediciones[i];
		}
		nuevoRitmo = nuevoRitmo* MEDICIONES_MINUTO/i;
	}

	estadisticas.pulsaciones = nuevoRitmo;
}

/*
 * recalcula los pasos por minuto
 * usa el numero de pasos en los ultimos 10 segundos(20 muestras)
 * cada muestra tiene el numero de pasos en 0.5 segundos
 */
static void calcularPasos()
{
	int i, nuevoRitmo = 0;

	if(vueltaCompleta)
	{
		for(i = 0; i < 20; i++)
		{
			nuevoRitmo+= pasosMediciones[i];
		}
		nuevoRitmo = nuevoRitmo * 6;
	}
	else
	{
		//este caso es para cuando no hemos rellenado el buffer completamente
		for(i = 0; i < numeroMedicion; i++)
		{
			nuevoRitmo+= pasosMediciones[i];
		}
		nuevoRitmo = nuevoRitmo* MEDICIONES_MINUTO/i;
	}

	estadisticas.pasosMinuto = nuevoRitmo;
}

static void calcularVelocidad()
{
	fix32 a = 0;
	a = FADDI(a,estadisticas.pasosMinuto,QM);
	fix32 b = 0;
	b = FADDI(a,PASOS_POR_KM,QM);
	a = FDIV(a,b,QM);
	estadisticas.velocidad = FMULI(a,60);

	//actualizar velociad maxima
	if(getParteEntera(estadisticas.velocidad) > getParteEntera(estadisticas.velocidad_max))
	{
		estadisticas.velocidad_max = estadisticas.velocidad;
	}
	else if(getParteEntera(estadisticas.velocidad) == getParteEntera(estadisticas.velocidad_max))
	{
		if(getParteDecimal(estadisticas.velocidad) > getParteDecimal(estadisticas.velocidad_max))
		{
			estadisticas.velocidad_max = estadisticas.velocidad;
		}
	}
	else
	{

	}

}


//actualiza estadisticas, funcion llamada cada 0.5 segundos
void calcularEstadisticas()
{

	calcularPulsaciones();
	calcularPasos();
	calcularVelocidad();

	numeroMedicion++;
	if(numeroMedicion >= 20)
	{
		numeroMedicion = 0;
		vueltaCompleta = 1;
	}
	latidosMediciones[numeroMedicion] = 0;
	pasosMediciones[numeroMedicion] = 0;
}

// inserta un (0)latido o un  (1)paso
void insertarMedicion(int tipo)
{
	switch(tipo)
	{
		case 0: latidosMediciones[numeroMedicion]++;
			break;
		case 1: pasosMediciones[numeroMedicion]++;
				const fix32 distanciaPorPaso = 0x4c8; //1.200 pasos/metro
				estadisticas.distancia = FADD(estadisticas.distancia,distanciaPorPaso);
				const fix32 caloriasPorPaso = 0x3f; // 0.063 calorias/paso
				estadisticas.calorias = FADD(estadisticas.calorias,caloriasPorPaso);
			break;
		default:;
	}
}

//devuelve la parte entera de un numero en Q22.10
static int getParteEntera(int numero)
{

	return (numero & (~(0x3ff))) >> 10;
}

//devuelve la parte decimal de un numero en Q22.10
static int getParteDecimal(int numero)
{

	return numero & (0x3ff);
}


void getPulsacionesMinuto(int* parteEntera)
{
	*parteEntera = estadisticas.pulsaciones;
}

void getPasosMinuto(int* parteEntera)
{
	*parteEntera = estadisticas.pasosMinuto;
}

void getVelocidad(int* parteEntera, int* decimales)
{
	*parteEntera = getParteEntera(estadisticas.velocidad);
	*decimales = getParteDecimal(estadisticas.velocidad);
}

void getCalorias(int* parteEntera, int* decimales)
{
	*parteEntera = getParteEntera(estadisticas.calorias);
	*decimales = getParteDecimal(estadisticas.calorias);
}

void getVelocidadMax(int* parteEntera, int* decimales)
{
	*parteEntera = getParteEntera(estadisticas.velocidad_max);
	*decimales = getParteDecimal(estadisticas.velocidad_max);
}

void getDistancia(int* parteEntera, int* decimales)
{
	*parteEntera = getParteEntera(estadisticas.distancia);
	*decimales = getParteDecimal(estadisticas.distancia);
}
