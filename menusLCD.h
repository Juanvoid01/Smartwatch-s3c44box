/*
 * menusLCD.h
 *
 *  Created on: 26/12/2023
 *  Author: Jaime
 */

#ifndef MENUSLCD_H_
#define MENUSLCD_H_

#include <s3c44b0x.h>
#include <s3cev40.h>
#include <common_types.h>
#include <system.h>
#include <lcd.h>
#include "utilidad.h"

/* Funciones que dibujan los distintos menus */

void pintarMenuPrincipal(const Estadisticas_Strings* est);

void pintarMenuAjustes(const Estadisticas_Strings* est);

void pintarMenuAudios(const Estadisticas_Strings* est);

void pintarMenuReloj(const Estadisticas_Strings* est);


//Muestra la hora actual
void pintarHora(EstadoPrograma estadoPrograma, char *time);

//Muestra que hay una grabacion en curso
void pintarSimboloGrabar();

//Elimina el simbolo de grabacion en curso
void quitarSimboloGrabar();

//cambia el simbolo del audio concreto a reproduciendo
void pintarSimboloEjecutarAudio(uint8 idAudio);

//pinta alerta cardiaca si es necesario
void pintarAlertaCardiaca(int latidos);


#endif /* MENUSLCD_H_ */
