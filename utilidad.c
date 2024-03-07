/*
 * taskHora.c
 *
 *  Created on: 27/12/2023
 *      Author: Juan
 */
#include "utilidad.h"

void inicializarEstadisticasStrings(Estadisticas_Strings* estadisticas_strings)
{
    estadisticas_strings->favoritas[PASOS] = TRUE;
    estadisticas_strings->favoritas[VELOCIDAD] = TRUE;
    estadisticas_strings->favoritas[VELOCIDAD_MAX] = FALSE;
    estadisticas_strings->favoritas[CALORIAS] = FALSE;
    estadisticas_strings->favoritas[DISTANCIA] = FALSE;
    estadisticas_strings->favoritas[PULSACIONES] = TRUE;
    estadisticas_strings->numFavoritas = 3;
    estadisticas_strings->audiosEstado[0] = LIBRE;
    estadisticas_strings->audiosEstado[1] = LIBRE;
    estadisticas_strings->audiosEstado[2] = LIBRE;
    estadisticas_strings->accionReloj = RELOJ_NADA;
    estadisticas_strings->alarmaSeleccionada = NINGUNA_ALARMA;
    estadisticas_strings->contReloj = 0;
}

//formatea una hora en un string con formato 00:00:00
void formatearHora(int hour, int minute, int second, char* timeString)
{
    // Extract digits for hours, minutes, and seconds
    int h1 = hour / 10;
    int h2 = hour % 10;
    int m1 = minute / 10;
    int m2 = minute % 10;
    int s1 = second / 10;
    int s2 = second % 10;

    // Convert digits to characters and construct the string
    timeString[0] = '0' + h1;
    timeString[1] = '0' + h2;
    timeString[2] = ':';
    timeString[3] = '0' + m1;
    timeString[4] = '0' + m2;
    timeString[5] = ':';
    timeString[6] = '0' + s1;
    timeString[7] = '0' + s2;
    timeString[8] = '\0';  // Null-terminate the string
}

//formatea un numero en un string con formato 000
void formatearInt(int number, char* formattedString)
{

    int hundreds = number / 100;
    int tens = (number % 100) / 10;
    int units = number % 10;

    formattedString[0] = '0' + hundreds;
    formattedString[1] = '0' + tens;
    formattedString[2] = '0' + units;
    formattedString[3] = '\0';
}

//formatea un numero en un string con formato 000.00
void formatearFloat(int parteEntera,int parteDecimal, char* resultado)
{
	//truncamos a tres decimales
	while(parteDecimal%1000 != parteDecimal)
		parteDecimal/=10;

	// Extract digits from parteEntera
	    int hundreds = parteEntera / 100;
	    int tens = (parteEntera % 100) / 10;
	    int units = parteEntera % 10;

	    resultado[0] = '0' + hundreds;
	    resultado[1] = '0' + tens;
	    resultado[2] = '0' + units;
	    resultado[3] = '.';

	    // Extract digits from parteDecimal
	    int decimalHundreds = parteDecimal / 100;
	    int decimalTens = (parteDecimal % 100) / 10;

	    resultado[4] = '0' + decimalHundreds;
	    resultado[5] = '0' + decimalTens;
	    resultado[6] = '\0'; // Null-terminate the string

}


//devuelve la longitud de una palabra
uint16 str_len(char *s){
	uint16 s_lenght = 0;
	char *aux = s;
	while(*s)
	{
		s_lenght++;
		s++;
	}
	s = aux;
	return s_lenght;
}

/* devuelve la estadistica seleccionada con el keypad,dependiente de la distribucion de
   las checkboxes en la pantalla*/

t_estadistica scancodeToEstadistica(uint8 scancode)
{
	t_estadistica resultado = NADA;

	if(scancode == 0x0)
	{
		resultado = PASOS;
	}
	else if(scancode == 0x1)
	{
		resultado = VELOCIDAD;
	}
	else if(scancode == 0x2)
	{
		resultado = VELOCIDAD_MAX;
	}
	else if(scancode == 0x4)
	{
		resultado = CALORIAS;
	}
	else if(scancode == 0x5)
	{
		resultado = DISTANCIA;
	}
	else if(scancode == 0x6)
	{
		resultado = PULSACIONES;
	}
	return resultado;
}



