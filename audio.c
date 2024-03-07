/*
 * audio.c
 *
 *  Created on: 12/01/2024
 *      Author: Juan
 */
#include "audio.h"

#define AUDIOBUFFER_SIZE   (16000*5) // 16000sps * 5s
#define WAVETABLE_SIZE     (202)
#define VOLUME_FACTOR 3

typedef struct {
	int16 ch0[AUDIOBUFFER_SIZE];
	int16 ch1[AUDIOBUFFER_SIZE];
	int8 estado;
} audiobuffer_t;

static audiobuffer_t audio0;
static audiobuffer_t audio1;
static audiobuffer_t audio2;

int16 heartbeat[WAVETABLE_SIZE] = {
    0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60,
    62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94, 96, 98, 100, 102, 104, 106, 108, 110, 112, 114,
    116, 118, 120, 122, 124, 126, 128, 130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160,
    162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190, 192, 194, 196, 198, 200, 198, 196, 194,
    192, 190, 188, 186, 184, 182, 180, 178, 176, 174, 172, 170, 168, 166, 164, 162, 160, 158, 156, 154, 152, 150, 148,
    146, 144, 142, 140, 138, 136, 134, 132, 130, 128, 126, 124, 122, 120, 118, 116, 114, 112, 110, 108, 106, 104, 102,
    100, 98, 96, 94, 92, 90, 88, 86, 84, 82, 80, 78, 76, 74, 72, 70, 68, 66, 64, 62, 60, 58, 56, 54, 52, 50, 48, 46,
    44, 42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0
};


void inicializarAudio()
{
    uda1341ts_init();
    iis_init( IIS_POLLING );

    audio0.estado = 0;
    audio1.estado = 0;
    audio2.estado = 0;
}

void sonidoLatido()
{
	uint32 i = 0;

	for(i = 0; i < WAVETABLE_SIZE; ++i)
	{
		iis_putSample( heartbeat[i] << VOLUME_FACTOR, heartbeat[i] << VOLUME_FACTOR);
	}
}

//emite el sonido de la alarma
void sonidoAlarma()
{
	uint32 i ,j, k = 4, t = 0;

	while(t < 9)
	{
		for(j=0;j<20;j++)
		{
			for(i = 0; i < WAVETABLE_SIZE; ++i)
			{
				iis_putSample( heartbeat[i] << k, heartbeat[i] << k);
			}
		}
		for(j = 0; j < 16000; ++j)
		{
			iis_putSample( 0 , 0 );
		}
		k = k >= 6 ? 4 : k + 1;
		t++;
	}

}




//grabacion de nota de voz de 10 segundos
//devuelve la posicion que se ha grabado

int8 grabarAudio()
{
	int8 posicionGrabada = -1;
	if(audio0.estado == 0)
	{
		int32 i;
	    for( i=0; i<AUDIOBUFFER_SIZE; i++ )
	     	iis_getSample( &(audio0.ch0[i]), &(audio0.ch1[i]) );

	    audio0.estado = 1;
	    posicionGrabada = 0;
	}
	else if(audio1.estado == 0)
	{
		int32 i;
	    for( i=0; i<AUDIOBUFFER_SIZE; i++ )
	     	iis_getSample( &(audio1.ch0[i]), &(audio1.ch1[i]) );

		audio1.estado = 1;
		posicionGrabada = 1;
	}
	else if(audio2.estado == 0)
	{
		int32 i;
	    for( i=0; i<AUDIOBUFFER_SIZE; i++ )
	     	iis_getSample( &(audio2.ch0[i]), &(audio2.ch1[i]) );

	    audio2.estado = 1;
	    posicionGrabada = 2;
	}
	else
	{
		//no hay hueco libre
		posicionGrabada = -1;
	}

	return posicionGrabada;
}

//eliminar el audio de la posicion indicada (id del 0 al 2)
void eliminarAudio(int8 idAudio)
{
	if(idAudio == 0)
	{
		audio0.estado = 0;
	}
	else if(idAudio == 1)
	{
		audio1.estado = 0;
	}
	else if(idAudio == 2)
	{
		audio2.estado = 0;
	}
}

//reproducir el audio de la posicion indicada
void reproducirAudio(int8 idAudio)
{
	if(idAudio == 0 && audio0.estado)
	{
		int32 i;
	    for( i=0; i<AUDIOBUFFER_SIZE; i++ )
	     	iis_putSample( audio0.ch0[i], audio0.ch1[i] );
	}
	else if(idAudio == 1 && audio1.estado)
	{
		int32 i;
	    for( i=0; i<AUDIOBUFFER_SIZE; i++ )
	     	iis_putSample( audio1.ch0[i], audio1.ch1[i] );
	}
	else if(idAudio == 2 && audio2.estado)
	{
		int32 i;
	    for( i=0; i<AUDIOBUFFER_SIZE; i++ )
	     	iis_putSample( audio2.ch0[i], audio2.ch1[i] );
	}
}

//reproducir el audio al doble de velocidad
void reproducirAudioX2(int8 idAudio)
{
	if(idAudio == 0 && audio0.estado)
	{
		int32 i;
	    for( i=0; i<AUDIOBUFFER_SIZE; i+=2 )
	     	iis_putSample( audio0.ch0[i], audio0.ch1[i] );
	}
	else if(idAudio == 1 && audio1.estado)
	{
		int32 i;
	    for( i=0; i<AUDIOBUFFER_SIZE; i+=2 )
	     	iis_putSample( audio1.ch0[i], audio1.ch1[i] );
	}
	else if(idAudio == 2 && audio2.estado)
	{
		int32 i;
	    for( i=0; i<AUDIOBUFFER_SIZE; i+=2 )
	     	iis_putSample( audio2.ch0[i], audio2.ch1[i] );
	}
}

//devuelve 0 si ese audio es vacio
int estadoAudio(int8 idAudio)
{
	if(idAudio == 0)
	{
		return audio0.estado;
	}
	else if(idAudio == 1)
	{
		return audio1.estado;
	}
	else
	{
		return audio2.estado;
	}
}

