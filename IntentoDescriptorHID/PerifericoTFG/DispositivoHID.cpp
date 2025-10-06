#include "DispositivoHID.h"


extern DispositivoHID dispositivoHID;

class MyCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    dispositivoHID.deviceConnected = true;
    Serial.println("Dispositivo conectado");

  }
  void onDisconnect(BLEServer* pServer) {
    dispositivoHID.deviceConnected = false;
    Serial.println("Dispositivo desconectado");
  }
};

void DispositivoHID::inicializar() {
    BLEDevice::init("PerifericoTFG");

   // Seguridad BLE básica
    BLESecurity* pSecurity = new BLESecurity();
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);                           //Emparejamiento con vinculacion (bonding)
    pSecurity->setCapability(ESP_IO_CAP_NONE);                                    //Empareja sin contraseña
    pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);  //Permite encriptacion y calve de identidad

    BLEServer* server = BLEDevice::createServer();      //Crea el servidor BLE
    server->setCallbacks(new MyCallbacks());

    hid = new BLEHIDDevice(server);                     //Crea el dispositivo HID BLE
    hid->manufacturer()->setValue("Sergio");            //Soy el fabricante 
    hid->hidInfo(0x00, 0x01);                           //Version de HID
    hid->pnp(0x02, 0x045E, 0x028E, 0x0110);             //pnp() define la ID del fabricante (como si emularas un dispositivo Microsoft, 0x045E).
    

   
    inputMouse = hid->inputReport(1);                   //Report con ID = 2 mouse 
    inputMouse->addDescriptor(new BLE2902());
    delay(500);
 
 
    hid->reportMap((uint8_t*)hidReportDescriptor, sizeof(hidReportDescriptor)); //cargo el descriptorHID
    hid->startServices();     //E iniccio el servicio HID

    BLEAdvertising* advertising = BLEDevice::getAdvertising();
    advertising->setAppearance(0x03C0);                         // Icono teclado/raton 
    advertising->addServiceUUID(hid->hidService()->getUUID());  //Permite que dispositivo sea detectado como HID  
    advertising->start();       //Comienza publicidad
  
}

//Constructor de clase 
DispositivoHID::DispositivoHID() {
    deviceConnected = false;  // Inicialización aquí
}

void DispositivoHID::pulsarTecla(uint8_t keycode) {
  if (!deviceConnected) return;

  uint8_t report[8] = {0};
  report[0] = 0x01;       // Report ID del teclado
  report[2] = keycode;    // Primer tecla (sin modificador)

  inputKeyboard->setValue(report, sizeof(report));
  inputKeyboard->notify();
}

void DispositivoHID::soltarTecla() {
  if (!deviceConnected) return;

  uint8_t report[8] = {0};
  report[0] = 0x01;       // Report ID del teclado

  inputKeyboard->setValue(report, sizeof(report));
  inputKeyboard->notify();
}
void DispositivoHID::mantenerTecla(uint8_t keycode) {
  if (!deviceConnected) return;

  uint8_t report[8] = {0};
  report[0] = 0x01;       // Report ID del teclado
  report[2] = keycode;    // Tecla a mantener presionada

  inputKeyboard->setValue(report, sizeof(report));
  inputKeyboard->notify();
}

void DispositivoHID::pulsarTeclaModificada(uint8_t modifier, uint8_t keycode) {
  if (!deviceConnected) return;

  uint8_t report[8] = {0};
  report[0] = 0x01;       // Report ID del teclado
  report[1] = modifier;   // Modificador (ej: 0x02 = Shift, 0x01 = Ctrl)
  report[2] = keycode;    // Tecla principal

  inputKeyboard->setValue(report, sizeof(report));
  inputKeyboard->notify();
}



void DispositivoHID::moverRaton(int8_t dx, int8_t dy) {
  if (!deviceConnected) return;

  uint8_t report[5] = {0};
  report[0] = 0x02;     // Report ID del ratón
  report[1] = 0x00;     // Botones (ninguno)
  report[2] = dx;       // Movimiento X
  report[3] = dy;       // Movimiento Y
  report[4] = 0x00;     // Scroll

  inputMouse->setValue(report, sizeof(report));
  inputMouse->notify();
}

void DispositivoHID::clickIzquierdo() {
  if (!deviceConnected) return;

  // Presionar
  uint8_t report[5] = {0x02, 0x01, 0, 0, 0};  // Botón izquierdo
  inputMouse->setValue(report, sizeof(report));
  inputMouse->notify();

  delay(10); // opcional, simula una pulsación corta

  // Soltar
  report[1] = 0x00;
  inputMouse->setValue(report, sizeof(report));
  inputMouse->notify();
}

void DispositivoHID::clickDerecho() {
  Serial.println(deviceConnected);
  if (!deviceConnected) return;

  // Presionar
  uint8_t report[5] = {0x02, 0x02, 0, 0, 0};  // Botón derecho

  for (int i = 0; i < 5; i++) {
    Serial.print(report[i]);
    Serial.print(" ");
  }
  Serial.println();
  
  inputMouse->setValue(report, sizeof(report));
  inputMouse->notify();
  Serial.println("Click derecho pulsado");

  delay(10); // opcional
//BTHLE\Dev_d48c496acd7a

  // Soltar
  report[1] = 0x00;
  inputMouse->setValue(report, sizeof(report));
  
  inputMouse->notify();

}

void DispositivoHID::scrollRueda(int8_t scrollAmount) {
  if (!deviceConnected) return;

  uint8_t report[5] = {0x02, 0x00, 0, 0, (uint8_t)scrollAmount};
  inputMouse->setValue(report, sizeof(report));
  inputMouse->notify();
}



