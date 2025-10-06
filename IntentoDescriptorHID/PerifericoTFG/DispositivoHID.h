#pragma once
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEHIDDevice.h>
#include "DescriptorHID.h"

class DispositivoHID {
public:

  DispositivoHID(); //constructor de clase 
  bool deviceConnected;  

  void inicializar();

  void pulsarTecla(uint8_t keycode);
  void soltarTecla();
  void mantenerTecla(uint8_t keycode);
  void pulsarTeclaModificada(uint8_t modifier, uint8_t keycode);

  void moverRaton(int8_t dx, int8_t dy);
  void clickIzquierdo();
  void clickDerecho();
  void scrollRueda(int8_t scrollAmount);

  
private:

  BLEHIDDevice* hid;
  BLECharacteristic* inputKeyboard;
  BLECharacteristic* inputMouse;
  
};

