/*
 * taskCalculos.h
 *
 *  Created on: 27/12/2023
 *      Author: Juan
 */

#ifndef TASKCALCULOS_H_
#define TASKCALCULOS_H_



void inicializarCalculos();

//actualiza estadisticas, funcion llamada cada 0.5 segundos
void calcularEstadisticas();

// inserta un (0)latido o un  (1)paso
void insertarMedicion(int tipo);

void getPulsacionesMinuto(int* parteEntera);

void getPasosMinuto(int* parteEntera);

void getVelocidad(int* parteEntera, int* decimales);

void getCalorias(int* parteEntera, int* decimales);

void getVelocidadMax(int* parteEntera, int* decimales);

void getDistancia(int* parteEntera, int* decimales);


#endif /* TASKCALCULOS_H_ */
