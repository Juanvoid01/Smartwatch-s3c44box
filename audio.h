/*
 * audio.h
 *
 *  Created on: 12/01/2024
 *      Author: Juan
 */

#ifndef AUDIO_H_
#define AUDIO_H_


#include <uda1341ts.h>
#include "iis.h"
#include "utilidad.h"

void inicializarAudio();


//emite el sonido de un latido de corazon
void sonidoLatido();

//emite el sonido de la alarma
void sonidoAlarma();

/*
 *	grabacion de nota de voz de 10 segundos
 *	devuelve la posicion que se ha grabado
 */
int8 grabarAudio();

//eliminar el audio de la posicion indicada (id del 0 al 2)
void eliminarAudio(int8 idAudio);

//reproducir el audio de la posicion indicada
void reproducirAudio(int8 idAudio);

//reproducir el audio al doble de velocidad
void reproducirAudioX2(int8 idAudio);

//devuelve 0 si ese audio es vacio
int estadoAudio(int8 idAudio);

#endif /* AUDIO_H_ */



