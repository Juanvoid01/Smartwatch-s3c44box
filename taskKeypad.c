/*
 * taskKeypad.c
 *
 *  Created on: 13/01/2024
 *      Author: Juan
 */
#include "taskKeypad.h"




//detecta tecleo en keypad en menu y cambia el estado del programa
void keypadMenu(uint8 scancode, boolean* cambioEstado, EstadoPrograma* estado)
{
	if(scancode == 0xf)
	{
    	*cambioEstado = TRUE;
    	*estado = AJUSTES;
	}
	else if(scancode == 0xb)
	{
    	*cambioEstado = TRUE;
		*estado = RELOJ;
	}
	else if(scancode == 0xc)
	{
    	*cambioEstado = TRUE;
		*estado = AUDIOS;
	}
}

//detecta tecleo en keypad en ajustes y cambia las estadisticas seleccionadas
void keypadAjustes(uint8 scancode, Estadisticas_Strings* estadisticas,boolean* cambioEstado, EstadoPrograma* estado)
{
	t_estadistica est_seleccionada;

	if(scancode == 0xc)
	{
    	*cambioEstado = TRUE;
		*estado = MENU;
	}
	else if((est_seleccionada = scancodeToEstadistica(scancode))!= NADA)
	{
		if(estadisticas->favoritas[est_seleccionada] == TRUE)
		{
			estadisticas->favoritas[est_seleccionada] = FALSE;
			estadisticas->numFavoritas--;
		}
		else
		{
			if(estadisticas->numFavoritas < 3)
			{
				estadisticas->favoritas[est_seleccionada] = TRUE;
				estadisticas->numFavoritas++;
			}
		}

	}
}

//detecta tecleo en keypad en menu audios y cambia los audios seleccionados
void keypadAudios(uint8 scancode, Estadisticas_Strings* estadisticas,boolean* cambioEstado,EstadoPrograma* estado, boolean* grabar)
{
	*grabar = FALSE;
	if(scancode == 0xc)
	{
		*cambioEstado = TRUE;
		*estado = MENU;
	}
	else if(scancode == 0xf)
	{
		*grabar = TRUE;
	}
	else if(scancode == 0x0)
	{
		if(estadisticas->audiosEstado[0]!=LIBRE)
		{
			estadisticas->audiosEstado[0] = SONANDO;
		}
	}
	else if(scancode == 0x1)
	{
		if(estadisticas->audiosEstado[0]!=LIBRE)
		{
			estadisticas->audiosEstado[0] = SONANDOX2;
		}
	}
	else if(scancode == 0x3)
	{
		estadisticas->audiosEstado[0] = LIBRE;
	}
	else if(scancode == 0x4)
	{
		if(estadisticas->audiosEstado[1]==PARADO)
		{
			estadisticas->audiosEstado[1] = SONANDO;
		}
	}
	else if(scancode == 0x5)
	{
		if(estadisticas->audiosEstado[1]==PARADO)
		{
			estadisticas->audiosEstado[1] = SONANDOX2;
		}
	}
	else if(scancode == 0x7)
	{
		estadisticas->audiosEstado[1] = LIBRE;
	}
	else if(scancode == 0x8)
	{
		if(estadisticas->audiosEstado[2]==PARADO)
		{
			estadisticas->audiosEstado[2] = SONANDO;
		}
	}
	else if(scancode == 0x9)
	{
		if(estadisticas->audiosEstado[2]==PARADO)
		{
			estadisticas->audiosEstado[2] = SONANDOX2;
		}
	}
	else if(scancode == 0xb)
	{
		estadisticas->audiosEstado[2] = LIBRE;
	}
}

//detecta tecleo en keypad en reloj y realiza las acciones que el usuario elija
void keypadReloj(uint8 scancode, Estadisticas_Strings* estadisticas,boolean* cambioEstado, EstadoPrograma* estado)
{

	if(scancode == 0xc)
	{
		*cambioEstado = TRUE;
		*estado = MENU;
	}
	else if(scancode == 0x0)
	{
		estadisticas->accionReloj = INC_H;
	}
	else if(scancode == 0x1)
	{
		estadisticas->accionReloj = INC_M;
	}
	else if(scancode == 0x2)
	{
		estadisticas->accionReloj = INC_S;
	}
	else if(scancode == 0x4)
	{
		estadisticas->accionReloj = DEC_H;
	}
	else if(scancode == 0x5)
	{
		estadisticas->accionReloj = DEC_M;
	}
	else if(scancode == 0x6)
	{
		estadisticas->accionReloj = DEC_S;
	}
	else if(scancode == 0x8)
	{
		estadisticas->accionReloj = ALARMA5;
	}
	else if(scancode == 0x9)
	{
		estadisticas->accionReloj = ALARMA30;
	}
	else if(scancode == 0xa)
	{
		estadisticas->accionReloj = ALARMA60;
	}
}
