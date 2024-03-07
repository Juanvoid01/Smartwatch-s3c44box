/*
 * taskKeypad.h
 *
 *  Created on: 13/01/2024
 *      Author: Juan
 */

#ifndef TASKKEYPAD_H_
#define TASKKEYPAD_H_

#include <keypad.h>
#include "utilidad.h"

//detecta tecleo en keypad en menu y cambia el estado del programa
void keypadMenu(uint8 scancode, boolean* cambioEstado, EstadoPrograma* estado);

//detecta tecleo en keypad en ajustes y cambia las estadisticas seleccionadas
void keypadAjustes(uint8 scancode, Estadisticas_Strings* estadisticas,boolean* cambioEstado, EstadoPrograma* estado);

//detecta tecleo en keypad en menu audios y cambia los audios seleccionados
void keypadAudios(uint8 scancode, Estadisticas_Strings* estadisticas,boolean* cambioEstado,EstadoPrograma* estado, boolean* grabar);

//detecta tecleo en keypad en reloj y realiza las acciones que el usuario elija
void keypadReloj(uint8 scancode, Estadisticas_Strings* estadisticas,boolean* cambioEstado, EstadoPrograma* estado);

#endif /* TASKKEYPAD_H_ */
