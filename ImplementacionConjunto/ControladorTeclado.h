#ifndef CONTROLADOR_TECLADO_H
#define CONTROLADOR_TECLADO_H

//#include <Arduino.h>
#include <BleCombo.h>
#include <math.h>
#include "ControladorPantalla.h"

// Definir pines para las teclas
#define CLICK_IZQ_PIN     12
#define CLICK_DER_PIN       13 

#define TECLA_ESPACIO_PIN    27
#define TECLA_BACK_PIN         14
#define TECLA_SHIFT_PIN       26
#define TECLA_CTRL_IZQ_PIN    25

//Definir pines de los JoySticks
#define EjexJy1               32  
#define EjeyJy1               33  
#define EjexJy2               34
#define EjeyJy2               35

#define SWITCH_JY1_PIN        15  
#define SWITCH_JY2_PIN        4

// Joystick esta siendo usado
extern volatile bool jy1Activo;
extern volatile bool jy2Activo;
// Valor de la ruleta del Joystick
extern volatile int ruletaJy1;
extern volatile int ruletaJy2;
// Banderas que se activan con las interrupciones 
extern volatile bool flagJy1;  
extern volatile bool flagJy2;  

extern volatile bool flagClickIzq;  
extern volatile bool flagClickDer; 
extern volatile bool flagEspacio; 
extern volatile bool flagBack;
extern volatile bool flagShift;
extern volatile bool flagCtrl;

// Valores de las ruletas
extern const uint8_t ruleta1_0[];
extern const uint8_t ruleta1_1[];
extern const uint8_t ruleta1_2[];
extern const uint8_t ruleta2_0[];
extern const uint8_t ruleta2_1[];
extern const uint8_t ruleta2_2[];

// Inicializacion de interrupciones
void inicializarInterrupcionesTeclado();

// Interrupciones que activan banderas
void IRAM_ATTR ISR_ClickIzq();
void IRAM_ATTR ISR_ClickDer();

void IRAM_ATTR ISR_TeclaEspacio();
void IRAM_ATTR ISR_TeclaBack();
void IRAM_ATTR ISR_TeclaShift();
void IRAM_ATTR ISR_TeclaCtrlIzq();

void IRAM_ATTR ISR_PulsacionJy1();
void IRAM_ATTR ISR_PulsacionJy2();

// Funciones que se ejecutan al activar banderas
void Joystick(int EjeX, int EjeY, int ruleta, int Joystick);
void clickIzquierdo();
void clickDerecho();
void espacio();
void back();
void shiftIzquierdo();
void ctrlIzquierdo();
#endif