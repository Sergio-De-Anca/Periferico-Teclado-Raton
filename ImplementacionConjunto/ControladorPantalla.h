#ifndef CONTROLADOR_PANTALLA_H
#define CONTROLADOR_PANTALLA_H

#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>
#include <SPI.h>
#include <math.h>

//Pines Pantalla
#define TFT_DC    2
#define TFT_CS    -1

extern Adafruit_GC9A01A tft;// = Adafruit_GC9A01A(TFT_CS, TFT_DC);
void inicializarPantalla();
void dibujarCaracteres(const uint8_t ruleta[], int posicionSeleccionada/*Direccion*/,uint8_t ndivisiones = 8);
#endif