/*
 * menusLCD.c
 *
 *  Created on: 26/12/2023
 *  Author: Jaime
 */
#include "menusLCD.h"

#define FRAME_WIDTH (3)//Anchura de los marcos

#define Y_MARGEN_FRAME (15) //Margen vertical del marco de la pantalla

#define MARGEN_SIMBOLO (5)//Margen entre el dibujo interior del boton y sus marcos

/* Botones */
#define X_TAM_BOTON_RETURN (65)//Tamano horizontal del boton Return

#define X_MARGEN_BOTON (30)//Margen horizontal de los botones

#define Y_BOTON_LENGTH (26)//Longitud vertical de un boton

#define Y_BOTON_NAME_SPACE (30)//Separacion vertical entre un boton y su nombre


/* Menu Principal */
#define X_TAM_BOTON_RELOJ (40)//Tamno horizontal del boton MenuReloj

#define X_POS_BOTON_RELOJ (215)//Posicion horizontal del boton MenuReloj

#define Y_POS_BOTONES (185)// Posicion vertical para los botones de MenuAudio y MenuAjustes

#define X_TAM_BOTON_AJUST (65)//Tamno horizontal del boton MenuAjustes

#define X_ESTAD_NUM (101) //Margen horizontal de las estadisticas con respecto a su valor numerico

#define X_MARGEN_ESTAD (7) //Margen inicial horizontal de las estadisticas

#define Y_ESTAD_MARGEN (55) //Margen inicial vertical de las estadisticas

#define Y_ESTAD_SPACE (40) //Separacion vertical entre estadisticas

#define X_POS_ALERTA (207)// Posicion vertical de mensajeAlerta

#define Y_POS_ALERTA (105)// Posicion vertical de mensaje  Alerta

/* Menu Ajustes - Menu Reloj */
#define CHECKBOX_SIZE (26)//Longitud del lado de un checkBox

#define Y_CHECKBOX_SPACE (60) //Separacion vertical entre checkBox

#define X_CHECKBOX_COL1 (25) //Posicion horizontal inicial de cada columna de checkbox

#define X_NAME_CHECKBOX_COL1 (15)//Posicion horizontal inicial en la columna  del nombre del checkbox

#define X_CHECKBOX_COL2 (120)

#define X_NAME_CHECKBOX_COL2 (95)

#define X_CHECKBOX_COL3 (215)

#define X_NAME_CHECKBOX_COL3 (180)


/* Menu Audio */
#define x_TAM_BOTON_LIBRE (30)//Longitud horizontal del boton libre

#define Y_TAM_BOTON_LIBRE (5) //Longitud vertical del boton libre

#define Y_TAM_BOTON_LIBRE_SPACE (10) //Espacio extra vertical del boton libre

#define x_TAM_BOTON_BORRAR (28)//Tamano  del boton borrar

#define X_TAM_BOTON_AUDIO (80)//Longitud horizontal de un botonAudio

#define Y_AUDIO_ROW_SPACE (50) //Separacion vertical entre las filas de MenuAudio

#define X_AUDIO_COL1 (10) //Posicion horizontal inicial de cada columna de MenuAudio

#define X_AUDIO_COL2 (100)

#define X_AUDIO_COL3 (190)

#define X_AUDIO_COL4 (280)



//Borra lo que haya en la posicion idicada en el LCD
void clearSection(uint16 x, uint16 y, uint16 length, uint16 depth){
	int i, j;
	for(i= 0; i <= length; i++){
		 for(j= 0; j <= depth; j++){
			 lcd_putpixel(x + i,y + j, WHITE);
		 }
	}
}

//Muestra una estadistica del menu principal
static void pintarEstadistica(uint16 x, uint16 y,char *nombre, const char *valor){

	lcd_puts(x, y, BLACK, nombre);
	lcd_puts(x + X_ESTAD_NUM, y, BLACK, valor);
}

//Muestra un boton con un nombre debajo
static void pintarBoton(uint16 x, uint16 y, uint8 color,char *nombre, uint16 largo){

	lcd_draw_box(x, y, x + largo, y + Y_BOTON_LENGTH, color, FRAME_WIDTH);
	lcd_puts(x, y + Y_BOTON_NAME_SPACE, color, nombre);
}

//Muestra una cruz dada la posicion y el tamano
void pintarCruz(uint16 x, uint16 y, uint16 largo){
	int i = 0;
	int aux1, aux2;
	while(i <= largo  - FRAME_WIDTH){
		for(aux1 = 0; aux1 < 3; aux1++){
			for(aux2 = 0; aux2 < 3; aux2++){
				lcd_putpixel(x + i + aux1, y + i + aux2, BLACK);
			}
		}
		i+=3;
	}
	i = 0;
	y = y + largo;

	while(i <= largo  - FRAME_WIDTH){
		for(aux1 = 0; aux1 < 3; aux1++){
			for(aux2 = 0; aux2 < 3; aux2++){
				lcd_putpixel(x + i + aux1, y - i - aux2, BLACK);
			}
		}
		i+=3;
	}
}

//Muestra el boton de seleccion para el menuAjustes
void pintarCheckBox(uint16 x, uint16 y,boolean checked){
	lcd_draw_box(x, y, x + CHECKBOX_SIZE, y + CHECKBOX_SIZE, BLACK, FRAME_WIDTH);

    //Borramos el grid interior de checkBox
    clearSection(x + FRAME_WIDTH, y + FRAME_WIDTH, CHECKBOX_SIZE - FRAME_WIDTH*2, CHECKBOX_SIZE - FRAME_WIDTH*2);

	//Si el chekcBox esta seleccionado, rellenamos el grid interior
	if(checked){
		 pintarCruz(x, y, CHECKBOX_SIZE);
	}
}

//Muestra el boton del MenuAudio
static void pintarBotonMenuAudio(uint16 x, uint16 y, char *nombre){

	pintarBoton(x, y, BLACK, nombre, X_TAM_BOTON_AUDIO);

	//Simbolo
	x+=MARGEN_SIMBOLO;
	y+=MARGEN_SIMBOLO;
	lcd_draw_box(x, y, x + 10 , y + Y_BOTON_LENGTH - MARGEN_SIMBOLO * 2, BLACK, 1);
	x+=15;
	lcd_draw_box(x, y + 10, x + 10, y + Y_BOTON_LENGTH - MARGEN_SIMBOLO * 2, BLACK, 1);
	x+=15;
	lcd_draw_box(x, y + 7, x + 10, y + Y_BOTON_LENGTH - MARGEN_SIMBOLO * 2, BLACK, 1);
	x+=15;
	lcd_draw_box(x, y + 3, x + 10, y + Y_BOTON_LENGTH - MARGEN_SIMBOLO * 2, BLACK, 1);
	x+=15;
	lcd_draw_box(x, y + 5, x + 10, y + Y_BOTON_LENGTH - MARGEN_SIMBOLO * 2, BLACK, 1);
}

//Muestra el boton del MenuAjustes
static void pintarBotonAjustes(uint16 x, uint16 y, char *nombre){

	//uint16 largo = str_len(nombre) * 9;

	pintarBoton(x, y, BLACK, nombre, X_TAM_BOTON_AJUST);

	//Simbolo
	y+=MARGEN_SIMBOLO;
	lcd_draw_box(x + MARGEN_SIMBOLO, y + MARGEN_SIMBOLO, x + X_TAM_BOTON_AJUST - MARGEN_SIMBOLO, y + 10, BLACK, 1);
	int i;
	int j;
	for(i = 0; i < 10; i++){
		for(j = 0; j < Y_BOTON_LENGTH - MARGEN_SIMBOLO * 2; j++){
			lcd_putpixel(x + X_TAM_BOTON_AJUST - 15 - i, y + j, BLACK);
		}
	}
}

//Muestra el boton del Reloj
static void pintarBotonReloj(uint16 x, uint16 y, uint16 largo){

	lcd_draw_box(x, y, x + largo, y + largo, BLACK, FRAME_WIDTH);
	int i,j;
	y+= FRAME_WIDTH;
	for(i = 0; i <= FRAME_WIDTH; i++){
			for(j = 0; j <= FRAME_WIDTH; j++){
				lcd_putpixel(x+ (largo/2) - FRAME_WIDTH + i,y + (largo/2) - FRAME_WIDTH + j, BLACK);
			}
		}

	for(i = FRAME_WIDTH; i <= (largo/2) - FRAME_WIDTH *2; i++){
		for(j = - FRAME_WIDTH; j <= 0; j++){
			lcd_putpixel(x + (largo/2) + i, y + (largo/2) + j, BLACK);
		}
	}

	for(i = 0; i <= FRAME_WIDTH; i++){
			for(j = 0; j <= (largo/2) - FRAME_WIDTH*3; j++){
				lcd_putpixel(x+ (largo/2) - FRAME_WIDTH + i, y + FRAME_WIDTH + j, BLACK);
			}
		}
}

//Muestra el boton de Return
static void pintarBotonReturn(uint16 x, uint16 y, char *nombre){

	//uint16 largo = str_len(nombre) * 9;

	pintarBoton(x, y, BLACK, nombre, X_TAM_BOTON_RETURN);

	//Simbolo
	int ancho_flecha = 3;
	int x1 = x + MARGEN_SIMBOLO * 2;
	int y2 = y + MARGEN_SIMBOLO + ancho_flecha * 2;
	int i;
	int j;
	for(i = x1; i <=  x + X_TAM_BOTON_RETURN - MARGEN_SIMBOLO * 2; i++){
		for(j = y2; j <=  y2 + ancho_flecha; j++){
			lcd_putpixel(i, j, BLACK);
		}
	}

	for(i = x1 + ancho_flecha *2; i <=  x1 + ancho_flecha * 7 - ancho_flecha; i++){
		for(j = y2-ancho_flecha; j <= y2; j++){
			lcd_putpixel(i, j, BLACK);
		}
	}

	for(i = x1 + ancho_flecha * 4; i <= x1 + ancho_flecha * 7 - ancho_flecha; i++){
		for(j = y2-ancho_flecha*2; j <=  y2 - ancho_flecha; j++){
			lcd_putpixel(i, j, BLACK);
		}
	}

	for(i = x1 + ancho_flecha*2; i <= x1 + ancho_flecha * 7 - ancho_flecha; i++){
		for(j = y2+ancho_flecha; j <= y2+ancho_flecha*2; j++){
			lcd_putpixel(i, j, BLACK);
		}
	}

	for(i = x1 + ancho_flecha * 4; i <=  x1 + ancho_flecha * 7 - ancho_flecha; i++){
		for(j = y2+ancho_flecha*2; j <=  y2+ancho_flecha*3; j++){
			lcd_putpixel(i, j, BLACK);
		}
	}
}

//Muestra el boton de Audio en el MenuAudios
void pintarBotonAudio(uint16 x, uint16 y, char *nombre, int indice){
	pintarBoton(x, y, BLACK, "", X_TAM_BOTON_AUDIO);
	lcd_puts(x + MARGEN_SIMBOLO*3, y + MARGEN_SIMBOLO, BLACK, nombre);
	lcd_putint(x + X_TAM_BOTON_AUDIO - MARGEN_SIMBOLO*4, y + MARGEN_SIMBOLO, BLACK, indice);
}

//Muestra el boton de audio vacio en el MenuAudios
void pintarBotonSinAudio(uint16 x, uint16 y){
	int i, j;
	y+=Y_TAM_BOTON_LIBRE_SPACE;
	for(i= 0; i <= x_TAM_BOTON_LIBRE; i++){
		 for(j= 0; j <= Y_TAM_BOTON_LIBRE; j++){
			 lcd_putpixel(x + i,y + j, BLACK);
		 }
	}
}

//Muestra el boton de grabacion en curso en el MenuAudios
void pintarBotonGrabandoAudio(uint16 x, uint16 y){
	pintarBoton(x, y, BLACK, "", X_TAM_BOTON_AUDIO);
	lcd_puts(x + MARGEN_SIMBOLO*2, y + MARGEN_SIMBOLO, BLACK, "GRABANDO");
}

//Muestra el boton de reproduciendo en el MenuAudios
void pintarBotonEjecutarAudio(uint16 x, uint16 y){
	pintarBoton(x, y, BLACK, "", X_TAM_BOTON_AUDIO);
	lcd_puts(x + MARGEN_SIMBOLO*3, y + MARGEN_SIMBOLO, BLACK, "PLAYING");
}

//Muestra el boton de reproducir en el MenuAudios
void pintarBotonPlay(uint16 x, uint16 y){
	pintarBoton(x, y, BLACK, "", X_TAM_BOTON_AUDIO);
	lcd_puts(x + MARGEN_SIMBOLO*5, y + MARGEN_SIMBOLO, BLACK, "PLAY");
}

//Muestra el boton de reproducir X2 de vlocidad en el MenuAudios
void pintarBotonX2Play(uint16 x, uint16 y){
	pintarBoton(x, y, BLACK, "", X_TAM_BOTON_AUDIO);
	lcd_puts(x + MARGEN_SIMBOLO*3, y + MARGEN_SIMBOLO, BLACK, "X2-PLAY");
}

//Pinta el boton de borrar audio en el MenuAudios
static void pintarBotonBorrar(uint16 x, uint16 y){
	pintarCheckBox(x, y, TRUE);
}

//Muestra el mensaje de alerta
static void pintarAlerta(uint16 x, uint16 y){
	lcd_puts(x, y, BLACK, "!ALERTA");
}

//Muestra los marcos de la pantalla junto al nombre del reloj
static void pintarTitulo(uint16 x, uint16 y){
	//screenBox();
	lcd_draw_box(0, 0, LCD_WIDTH - FRAME_WIDTH, LCD_HEIGHT - FRAME_WIDTH , BLACK, FRAME_WIDTH ); //Marco de la pantalla
	lcd_draw_box(0, 0, LCD_WIDTH - FRAME_WIDTH, 42, BLACK, FRAME_WIDTH ); //Marco superior
	lcd_draw_box(235, 0, LCD_WIDTH - FRAME_WIDTH, 42, BLACK, FRAME_WIDTH );//Marco de la hora
	lcd_puts(x, y, BLACK, "J-WATCH MINI");
}

//Muestra los marcos del MenuPrincipal
static void pintarMarcosMenuPrincipal(){
	//Marco de las estadisticas
	lcd_draw_box(0, 40, LCD_WIDTH / 2, 175, BLACK, FRAME_WIDTH );

	//Marco derecho de los avisos y menu reloj
	lcd_draw_box(158, 40, LCD_WIDTH - FRAME_WIDTH, 175, BLACK, FRAME_WIDTH );
}

void pintarMenuPrincipal(const Estadisticas_Strings* est){

	pintarTitulo(100, Y_MARGEN_FRAME);
	pintarMarcosMenuPrincipal();
	int y = Y_ESTAD_MARGEN;
	pintarBotonReloj(X_POS_BOTON_RELOJ, y, X_TAM_BOTON_RELOJ);

	int i = 0;
	int j = 0;
	while (j < est->numFavoritas && i < 6) {
		if (est->favoritas[i] == TRUE) {
			switch (i) {
			case PASOS : pintarEstadistica(X_MARGEN_ESTAD, y, "Pasos:", est->pasos);
				break;
			case VELOCIDAD : pintarEstadistica(X_MARGEN_ESTAD, y, "Velocidad:", est->velocidad);
				break;
			case PULSACIONES : pintarEstadistica(X_MARGEN_ESTAD, y, "Pulsaciones:", est->pulsaciones);
				break;
			case CALORIAS : pintarEstadistica(X_MARGEN_ESTAD, y, "Calorias:", est->calorias);
				break;
			case DISTANCIA : pintarEstadistica(X_MARGEN_ESTAD, y, "Distancia:", est->distancia);
				break;
			case VELOCIDAD_MAX : pintarEstadistica(X_MARGEN_ESTAD, y, "Velocidad M:", est->velocidad_max);
				break;
			}
			y += Y_ESTAD_SPACE;
			j++;
		}
		i++;
	}

	pintarBotonMenuAudio(X_MARGEN_BOTON, Y_POS_BOTONES, "MenuAudio");
	pintarBotonAjustes(210, Y_POS_BOTONES, "Ajustes");
}

void pintarMenuAjustes(const Estadisticas_Strings* est){

	pintarTitulo(100, Y_MARGEN_FRAME);

	//Marco del menuAjustes
	lcd_draw_box(0, 0, LCD_WIDTH - FRAME_WIDTH, LCD_HEIGHT - FRAME_WIDTH, BLACK, FRAME_WIDTH);

    //Se dibujan las checkBox segun su estado
	pintarCheckBox(X_CHECKBOX_COL1, Y_CHECKBOX_SPACE,  est->favoritas[PASOS]);
	lcd_puts(X_NAME_CHECKBOX_COL1, Y_CHECKBOX_SPACE + Y_BOTON_NAME_SPACE, BLACK, "Pasos");

	pintarCheckBox(X_CHECKBOX_COL1, Y_CHECKBOX_SPACE * 2, est->favoritas[CALORIAS]);
	lcd_puts(X_NAME_CHECKBOX_COL1, Y_CHECKBOX_SPACE * 2 + Y_BOTON_NAME_SPACE, BLACK, "Calorias");

	pintarCheckBox(X_CHECKBOX_COL2, Y_CHECKBOX_SPACE, est->favoritas[VELOCIDAD]);
	lcd_puts(X_NAME_CHECKBOX_COL2, Y_CHECKBOX_SPACE + Y_BOTON_NAME_SPACE, BLACK, "Velocidad");

	pintarCheckBox(X_CHECKBOX_COL2, Y_CHECKBOX_SPACE * 2, est->favoritas[DISTANCIA]);
	lcd_puts(X_NAME_CHECKBOX_COL2, Y_CHECKBOX_SPACE * 2 + Y_BOTON_NAME_SPACE, BLACK, "Distancia");

	pintarCheckBox(X_CHECKBOX_COL3, Y_CHECKBOX_SPACE, est->favoritas[VELOCIDAD_MAX]);
	lcd_puts(X_NAME_CHECKBOX_COL3, Y_CHECKBOX_SPACE + Y_BOTON_NAME_SPACE, BLACK, "Velocidad Maxima");

	pintarCheckBox(X_CHECKBOX_COL3, Y_CHECKBOX_SPACE * 2, est->favoritas[PULSACIONES]);
	lcd_puts(X_NAME_CHECKBOX_COL3, Y_CHECKBOX_SPACE * 2 + Y_BOTON_NAME_SPACE, BLACK, "Pulsaciones");

	pintarBotonReturn(X_CHECKBOX_COL1,Y_POS_BOTONES , "Return");
}

void pintarMenuAudios(const Estadisticas_Strings* est)
{
	pintarTitulo(100, Y_MARGEN_FRAME);
	int i;
	for(i = 0; i < 3; i++){
		clearSection(X_AUDIO_COL1, Y_AUDIO_ROW_SPACE + Y_AUDIO_ROW_SPACE * i, X_TAM_BOTON_AUDIO, Y_BOTON_LENGTH);
		switch (est->audiosEstado[i]) {
		case LIBRE : pintarBotonAudio(X_AUDIO_COL1, Y_AUDIO_ROW_SPACE + Y_AUDIO_ROW_SPACE * i, "LIBRE", i+1);
			break;
		default: pintarBotonAudio(X_AUDIO_COL1, Y_AUDIO_ROW_SPACE + Y_AUDIO_ROW_SPACE * i, "AUDIO", i+1);
			break;
		}
	}
	for(i = 0; i < 3; i++){
		clearSection(X_AUDIO_COL2, Y_AUDIO_ROW_SPACE + Y_AUDIO_ROW_SPACE * i, X_TAM_BOTON_AUDIO, Y_BOTON_LENGTH);
		switch (est->audiosEstado[i]) {
		case LIBRE : pintarBotonSinAudio(X_AUDIO_COL2, Y_AUDIO_ROW_SPACE + Y_AUDIO_ROW_SPACE * i);
			break;
		default : pintarBotonPlay(X_AUDIO_COL2, Y_AUDIO_ROW_SPACE + Y_AUDIO_ROW_SPACE * i);
			break;
		}
	}

	for(i = 0; i < 3; i++){
		clearSection(X_AUDIO_COL3, Y_AUDIO_ROW_SPACE + Y_AUDIO_ROW_SPACE * i, X_TAM_BOTON_AUDIO, Y_BOTON_LENGTH);
		switch (est->audiosEstado[i]) {
		case LIBRE : pintarBotonSinAudio(X_AUDIO_COL3, Y_AUDIO_ROW_SPACE + Y_AUDIO_ROW_SPACE * i);
			break;
		default : pintarBotonX2Play(X_AUDIO_COL3, Y_AUDIO_ROW_SPACE + Y_AUDIO_ROW_SPACE * i);
			break;
		}
	}

	for(i = 0; i < 3; i++){
		clearSection(X_AUDIO_COL4, Y_AUDIO_ROW_SPACE + Y_AUDIO_ROW_SPACE * i, x_TAM_BOTON_LIBRE, Y_BOTON_LENGTH);
		switch (est->audiosEstado[i]) {
		case LIBRE : pintarBotonSinAudio(X_AUDIO_COL4, Y_AUDIO_ROW_SPACE + Y_AUDIO_ROW_SPACE * i);
			break;
		default: pintarBotonBorrar(X_AUDIO_COL4, Y_AUDIO_ROW_SPACE + Y_AUDIO_ROW_SPACE * i);
			break;
		}
	}
	pintarBotonReturn(X_AUDIO_COL1, Y_POS_BOTONES, "Return");
}

void pintarMenuReloj(const Estadisticas_Strings* est)
{
	lcd_puts(X_NAME_CHECKBOX_COL1, Y_CHECKBOX_SPACE * 2 + Y_BOTON_NAME_SPACE, BLACK, "Alarma 5s   Alarma 30s   Alarma 60s");

	pintarCheckBox(X_CHECKBOX_COL1 + 5, Y_CHECKBOX_SPACE * 2, est->alarmaSeleccionada == ALARMA_5);

	pintarCheckBox(X_CHECKBOX_COL2 + 20, Y_CHECKBOX_SPACE * 2, est->alarmaSeleccionada == ALARMA_30);

	pintarCheckBox(X_CHECKBOX_COL3 + 20, Y_CHECKBOX_SPACE * 2, est->alarmaSeleccionada == ALARMA_60);

	pintarBotonReturn(X_CHECKBOX_COL1,Y_POS_BOTONES , "Return");
}

void pintarHora(EstadoPrograma estadoPrograma, char *time)
{
	if(estadoPrograma!=RELOJ)
	{
		lcd_puts(245,Y_MARGEN_FRAME,BLACK,time);
	}
	else
	{
		lcd_puts_x2(85,25,BLACK,time);
	}
}

//cambia el simbolo del audio correspondiente a reproduciendo
void pintarSimboloEjecutarAudio(uint8 idAudio)
{
	clearSection(X_AUDIO_COL2, Y_AUDIO_ROW_SPACE + Y_AUDIO_ROW_SPACE * idAudio, X_TAM_BOTON_AUDIO, Y_BOTON_LENGTH);
	pintarBotonEjecutarAudio(X_AUDIO_COL2, Y_AUDIO_ROW_SPACE + Y_AUDIO_ROW_SPACE * idAudio);

	clearSection(X_AUDIO_COL3, Y_AUDIO_ROW_SPACE + Y_AUDIO_ROW_SPACE * idAudio, X_TAM_BOTON_AUDIO, Y_BOTON_LENGTH);
	pintarBotonEjecutarAudio(X_AUDIO_COL3, Y_AUDIO_ROW_SPACE + Y_AUDIO_ROW_SPACE * idAudio);
}

void pintarSimboloGrabar()
{
	pintarBotonGrabandoAudio(X_AUDIO_COL3, Y_POS_BOTONES);
}

void quitarSimboloGrabar()
{
	clearSection(X_AUDIO_COL3, Y_POS_BOTONES, X_TAM_BOTON_AUDIO, Y_BOTON_LENGTH);
}

//pinta alerta cardiaca si es necesario
void pintarAlertaCardiaca(int latidos)
{
	static boolean alertaActivada = FALSE;

	if(latidos >= VALOR_LATIDOS_ALTOS){
		alertaActivada = TRUE;
		pintarAlerta(X_POS_ALERTA, Y_POS_ALERTA); //190
		lcd_puts(163, Y_POS_ALERTA + 20, BLACK, "LATIDOS MUY ALTOS");

	}else if(latidos <= VALOR_LATIDOS_BAJOS){
		alertaActivada = TRUE;
		pintarAlerta(X_POS_ALERTA, Y_POS_ALERTA);
		lcd_puts(163, Y_POS_ALERTA + 20, BLACK, "LATIDOS MUY BAJOS");
	}
	else
	{
		if(alertaActivada)
		{
			clearSection(162, Y_POS_ALERTA, 150, 55);
			alertaActivada = FALSE;
		}

	}
}
