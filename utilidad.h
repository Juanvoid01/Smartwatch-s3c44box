/*
 * taskHora.h
 *
 *  Created on: 27/12/2023
 *      Author: Juan
 */

#ifndef TASKHORA_H_
#define TASKHORA_H_

#include <common_types.h>

#define VALOR_LATIDOS_BAJOS 40
#define VALOR_LATIDOS_ALTOS 180

typedef enum { MENU , AJUSTES, AUDIOS, RELOJ} EstadoPrograma;

typedef enum {PASOS = 0, VELOCIDAD = 1, VELOCIDAD_MAX = 2, CALORIAS = 3, DISTANCIA = 4, PULSACIONES = 5, NADA = 6}t_estadistica;

typedef enum {LIBRE = 0, SONANDO, SONANDOX2, PARADO} t_audioEstado;

typedef enum {RELOJ_NADA = 0,INC_H, INC_M, INC_S, DEC_H, DEC_M,DEC_S, ALARMA5, ALARMA30, ALARMA60} t_accionReloj;

typedef enum {NINGUNA_ALARMA, ALARMA_5, ALARMA_30, ALARMA_60} t_tipo_alarma;

typedef struct
{
	char pasos[4]; //int 3 cifras
	char pulsaciones[4]; //int 3 cifras
	char calorias[7]; //float
	char velocidad[7]; //float
	char distancia[7]; //float
	char velocidad_max[7]; //float
	boolean favoritas[6]; //estadistica[i] = FALSE si no es favorita
	int numFavoritas;
	t_audioEstado audiosEstado[3]; //  audiosEstado[i] = LIBRE, SONANDO, PARADO
	t_accionReloj accionReloj; //RELOJ_NADA,INC_H, INC_M, INC_S, DEC_H, DEC_M,DEC_S, ALARMA5, ALARMA30, ALARMA60
	t_tipo_alarma alarmaSeleccionada; //NINGUNA_ALARMA, ALARMA_5, ALARMA_30, ALARMA_60
	int contReloj;

}Estadisticas_Strings;

void inicializarEstadisticasStrings(Estadisticas_Strings* estadisticas_strings);

//formatea una hora en un string con formato 00:00:00
void formatearHora(int hour, int minute, int second, char* timeString);

//formatea un numero en un string con formato 000
void formatearInt(int number, char* formattedString);

//formatea un numero en un string con formato 000.00
void formatearFloat(int parteEntera,int parteDecimal, char* formattedString);

/* devuelve la estadistica seleccionada con el keypad,dependiente de la distribucion de
   las checkboxes en la pantalla*/
t_estadistica scancodeToEstadistica(uint8 scancode);

//devuelve la longitud de una palabra
uint16 str_len(char *s);

#endif /* TASKHORA_H_ */
