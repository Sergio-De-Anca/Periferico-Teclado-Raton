#include "ControladorTeclado.h"

// Constantes de joystick
const int DEADZONE = 800;
const int center = 4096 / 2;

//Banderas interrupciones
volatile bool jy1Activo = false;
volatile bool jy2Activo = false;

//Banderas interrupciones 
volatile bool flagClickIzq = false;
volatile bool flagClickDer = false;
volatile bool flagEspacio = false;
volatile bool flagBack = false;
volatile bool flagShift = false;
volatile bool flagCtrl = false;

 //Valores de la ruleta
volatile int ruletaJy1 = 0;
volatile int ruletaJy2 = 0;

const uint8_t ruleta1_0[] = {'a', 'e', 'i', 'o', 'u', 0x28/*ENTER*/, KEY_BACKSPACE, KEY_DELETE};
const uint8_t ruleta1_1[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
const uint8_t ruleta1_2[] = {'<', ',', '.', '-', '+', 'º', '?', '¡'};

const uint8_t ruleta2_0[] = {'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k'};
const uint8_t ruleta2_1[] = {'l', 'm', 'n', 'p', 'q', 'r', 's', 't'};
const uint8_t ruleta2_2[] = {'v', 'w', 'x', 'y', 'z', KEY_INSERT, KEY_BACKSPACE, KEY_DELETE};

//Evitar debounce en ruletas
volatile unsigned long lastDebounceTimeJy1 = 0;
volatile unsigned long lastDebounceTimeJy2 = 0;
const unsigned long debounceDelay = 150; // milisegundos

//Bandera para modificar pantalla cuando se pulsa la ruleta
volatile bool flagJy1 = false;  // global
volatile bool flagJy2 = false;  // global
 
  /*******************************************/
  /***Inicialización pines e interrupciones***/
  /*******************************************/

void inicializarInterrupcionesTeclado(){

    //Inicializo pines teclas
    pinMode(CLICK_IZQ_PIN, INPUT_PULLUP);
    pinMode(CLICK_DER_PIN, INPUT_PULLUP);

    pinMode(TECLA_ESPACIO_PIN, INPUT_PULLUP);
    pinMode(TECLA_BACK_PIN, INPUT_PULLUP);
    pinMode(TECLA_SHIFT_PIN, INPUT_PULLUP);
    pinMode(TECLA_CTRL_IZQ_PIN, INPUT_PULLUP);

    // Inicializa pines joysticks
    pinMode(EjexJy1, INPUT);
    pinMode(EjeyJy1, INPUT);
    pinMode(EjexJy2, INPUT);
    pinMode(EjeyJy2, INPUT);
    pinMode(SWITCH_JY1_PIN, INPUT_PULLUP);
    pinMode(SWITCH_JY2_PIN, INPUT_PULLUP);
    
    //Vincula cada tecla y Joystick a su interrupcion (incluido clics raton)
    attachInterrupt(digitalPinToInterrupt(CLICK_IZQ_PIN), ISR_ClickIzq, CHANGE);
    attachInterrupt(digitalPinToInterrupt(CLICK_DER_PIN), ISR_ClickDer, CHANGE);
    
    attachInterrupt(digitalPinToInterrupt(TECLA_ESPACIO_PIN), ISR_TeclaEspacio, CHANGE);
    attachInterrupt(digitalPinToInterrupt(TECLA_BACK_PIN), ISR_TeclaBack, CHANGE);
    attachInterrupt(digitalPinToInterrupt(TECLA_SHIFT_PIN), ISR_TeclaShift, CHANGE);
    attachInterrupt(digitalPinToInterrupt(TECLA_CTRL_IZQ_PIN), ISR_TeclaCtrlIzq, CHANGE);
    
    attachInterrupt(digitalPinToInterrupt(SWITCH_JY1_PIN), ISR_PulsacionJy1, FALLING);
    attachInterrupt(digitalPinToInterrupt(SWITCH_JY2_PIN), ISR_PulsacionJy2, FALLING);
}
  /*****************************************/
  /***Funciones asociadas a los Joysticks***/
  /*****************************************/
//Calcular hacia que dirección apunta el Joystick
int calcularDireccion(int x, int y, int totaldirecciones = 8) {
  float angulo = atan2(y, x); // ángulo en radianes
  if (angulo < 0) angulo += 2 * PI; // normaliza a 0–2π

  int direccion = ((int)round((angulo / (2 * PI)) * totaldirecciones)) % totaldirecciones;
  return direccion; // valores de 0 a 7
}
//Escribir la tecla en el pc y mostrar la que se señala en la pantalla TFT
void escribirTecla(int x, int y, int ruleta, int Joystick){

  uint8_t caracter = ' ';
  int dir = calcularDireccion(x, y);
  if (Joystick == 1){
    switch(ruleta){
      case 0: 
        caracter = ruleta1_0[dir];  
        dibujarCaracteres(ruleta1_0, dir);
        break;
      case 1: 
        dir = calcularDireccion(x, y, 10);
        caracter = ruleta1_1[dir];  
        dibujarCaracteres(ruleta1_1, dir, 10);
        break;
      case 2: 
        caracter = ruleta1_2[dir];
        dibujarCaracteres(ruleta1_2, dir);
        break;
      default: break;
    }  
  }else if (Joystick == 2){
    switch(ruleta){
      case 0: 
        caracter = ruleta2_0[dir];
        dibujarCaracteres(ruleta2_0, dir);
        break;
      case 1: 
        caracter = ruleta2_1[dir];
        dibujarCaracteres(ruleta2_1, dir);
        break;
      case 2: 
        caracter = ruleta2_2[dir];
        dibujarCaracteres(ruleta2_2, dir);
        break;
      default: break;
    }
  }  
  
  Serial.println("La direccion seleccionada es: "+ String(dir));
  Serial.println("La tecla que va a escribit es: "+ String((char)caracter));
  
  if (Keyboard.isConnected()) {
      Keyboard.press(caracter);
      delay(5);
      Keyboard.release(caracter);
    }else{
      Serial.println("El teclado no esta conectado");
    }
}
//Calcula si el joystick se ha movido
bool fueraDeCentro(int x, int y) {
  return !(x * x + y * y < DEADZONE * DEADZONE) ; // Verificar zona muerta  
}
//Mira si se esta utilizando un Jy y si es así, se va a escribir tecla
void Joystick(int EjeX, int EjeY, int ruletaJy, int Joystick){
    int x = analogRead(EjeX);
    int y = analogRead(EjeY);
    //Serial.println("El eje x es: " + String(x) + " El eje y es: " + String(y));
    int dx = static_cast<int>(x) - center; //Ahora el valor esta entre -2048 y 2048
    int dy = static_cast<int>(y) - center; //Ahora el valor esta entre -2048 y 2048
    //Serial.println("El eje dx es : "+ String(dx) + " El eje dy es:"+ String(dy));

    if (fueraDeCentro(dx, dy)){
      if (!jy1Activo) {
        jy1Activo = true;
        escribirTecla(dx, dy, ruletaJy, Joystick);  // Interrupción simulada
      } else {
        jy1Activo = false;  // Vuelve a estar en centro
      }
    }
}
  /***************************************/
  /***Interrupciones de las pulsaciones***/
  /***************************************/
void IRAM_ATTR ISR_PulsacionJy1() {
  unsigned long now = millis();
  if (now - lastDebounceTimeJy1 > debounceDelay) {
    ruletaJy1++;
    if (ruletaJy1 > 2) ruletaJy1 = 0;
    lastDebounceTimeJy1 = now;
    flagJy1= true;
  }
}
void IRAM_ATTR ISR_PulsacionJy2() {
  unsigned long now = millis();
  if (now - lastDebounceTimeJy2 > debounceDelay) {
    ruletaJy2++;
    if (ruletaJy2 > 2) ruletaJy2 = 0;
    lastDebounceTimeJy2 = now;
    
    flagJy2 = true;
  }
}

void IRAM_ATTR ISR_ClickIzq() {
  flagClickIzq = true;
}
void IRAM_ATTR ISR_ClickDer() {
  flagClickDer = true;
}
void IRAM_ATTR ISR_TeclaEspacio() {
  flagEspacio = true;
}
void IRAM_ATTR ISR_TeclaBack() {
  flagBack = true;
}
void IRAM_ATTR ISR_TeclaShift() {
  flagShift = true;
}
void IRAM_ATTR ISR_TeclaCtrlIzq() {
  flagCtrl = true;
}
  /***************************************************/
  /***Funciones que se ejecutan al activar banderas***/
  /***************************************************/
void clickIzquierdo(){
  if (flagClickIzq) {
    flagClickIzq = false;
    bool estadoClickIzq = digitalRead(CLICK_IZQ_PIN);

    if (Keyboard.isConnected()) {
      if(estadoClickIzq == LOW){
        Mouse.press(MOUSE_LEFT); 
        Serial.println("Click izquierdo pulsado");
      }else{
        Mouse.release(MOUSE_LEFT); 
        Serial.println("Click izquierdo soltado");
        delay(5);
        }
      }
    }
}
void clickDerecho(){
  if (flagClickDer) {
    flagClickDer = false;
    bool estadoClickDer = digitalRead(CLICK_DER_PIN);

    if (Keyboard.isConnected()) {
      if(estadoClickDer == LOW){
        Mouse.press(MOUSE_RIGHT); 
        Serial.println("Click derecho pulsado");
      }else{
        Mouse.release(MOUSE_RIGHT); 
        Serial.println("Click derecho soltado");
        delay(5);
        }
      }
    }
}
void espacio(){
  if (flagEspacio) {
    flagEspacio = false;
    bool estadoEspacio = digitalRead(TECLA_ESPACIO_PIN);

    if (Keyboard.isConnected()) {
      if(estadoEspacio == LOW){
        Keyboard.press(' '); 
        Serial.println("Espacio pulsado");
      }else{
        Keyboard.release(' '); 
        Serial.println("Espacio soltado");
        delay(5);
        }
      }
    }
}
void back(){

  if (flagBack) {
    flagBack = false;
    bool estadoBack = digitalRead(TECLA_BACK_PIN);

    if (Keyboard.isConnected()) {
      if(estadoBack == LOW){
        Keyboard.press(KEY_BACKSPACE); 
        Serial.println("BackSpace pulsado");
      }else{
        Keyboard.release(KEY_BACKSPACE); 
        Serial.println("BackSpace soltado");
        delay(5);
        }
      }
    }
    
}
void shiftIzquierdo(){
  if (flagShift) {
    flagShift = false;
    bool estadoShiftIzquierdo = digitalRead(TECLA_SHIFT_PIN);  // HIGH = sin pulsar, LOW = pulsado
    if (Keyboard.isConnected()) {
      if (estadoShiftIzquierdo == LOW) {
        Keyboard.press(KEY_LEFT_SHIFT);  // se ha pulsado
        Serial.println("Tecla shift pulsada");
      } else {
        Keyboard.release(KEY_LEFT_SHIFT);  // se ha soltado
        Serial.println("Tecla shift soltada");
        delay(5);
      }
    }  
  }
}
void ctrlIzquierdo(){
  if (flagCtrl) {
    flagCtrl = false;
    bool estadoCtrlIzquierdo = digitalRead(TECLA_CTRL_IZQ_PIN);  // HIGH = sin pulsar, LOW = pulsado
    if (Keyboard.isConnected()) {
      if (estadoCtrlIzquierdo == LOW) {
        Keyboard.press(KEY_LEFT_CTRL);  // se ha pulsado
        Serial.println("Tecla ctrl pulsada");
      } else {
        Keyboard.release(KEY_LEFT_CTRL);  // se ha soltado
        Serial.println("Tecla ctrl soltada");
        delay(5);
      }
    }  
  }
}