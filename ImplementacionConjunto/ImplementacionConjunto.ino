#include "ControladorIMU.h"
#include "ControladorTeclado.h"
#include "ControladorPantalla.h"
#include <BleCombo.h>

void setup() {
  Serial.begin(115200);
  delay(500); 
  /***Inicialización BLE_HID***/
  Serial.println("Starting BLE work!");
  Keyboard.begin();
  Mouse.begin();
  delay(500);
  /***Inicialización del sensor IMU***/
  inicializarIMU();
  /***Inicialización interrupciones***/
  inicializarInterrupcionesTeclado();
  inicializarPantalla();
  /***Inicialización tareas***/
  // Tarea raton en núcleo 0
  xTaskCreatePinnedToCore(Raton, "Raton", 2048, NULL, 1, NULL, 0);
  // Tarea teclado en núcleo 1
  xTaskCreatePinnedToCore(Teclado, "Teclado", 2048, NULL, 1, NULL, 1);
  // Tarea pantalla en núcleo 1
  xTaskCreatePinnedToCore(Pantalla, "Pantalla", 2048, NULL, 1, NULL, 1);
}

void loop(){
}

void Teclado(void *pvParameters) {
  while (1) {
    
    // Joysticks
    Joystick(EjexJy1, EjeyJy1, ruletaJy1, 1);
    Joystick(EjexJy2, EjeyJy2, ruletaJy2, 2);
    // Clicks raton
    clickIzquierdo();
    clickDerecho();
    //Teclas
    espacio();
    back();
    shiftIzquierdo();
    ctrlIzquierdo();

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void Pantalla(void *pvParameters) {
  while (1) {
    //Muestra ruleta al clicar el joystick
    if (flagJy1) {
      flagJy1 = false;  // reseteo el flag
      Serial.println("La ruleta 1 es ahora: " + String(ruletaJy1));
      if(ruletaJy1 == 0){
        dibujarCaracteres(ruleta1_0, 11);
      }else if (ruletaJy1 == 1){
        dibujarCaracteres(ruleta1_1, 11, 10);
      }else if (ruletaJy1 == 2){
        dibujarCaracteres(ruleta1_2, 11);
      }
    }else if (flagJy2){
      flagJy2 = false; 
      Serial.println("La ruleta 2 es ahora: " + String(ruletaJy2));
      if(ruletaJy2 == 0){
        dibujarCaracteres(ruleta2_0, 11);
      }else if (ruletaJy2 == 1){
        dibujarCaracteres(ruleta2_1, 11);
      }else if (ruletaJy2 == 2){
        dibujarCaracteres(ruleta2_2, 11);
      }
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void Raton(void *pvParameters) {
  while (1) {
    if(Keyboard.isConnected()){
      moverRatonSuavizado();
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
