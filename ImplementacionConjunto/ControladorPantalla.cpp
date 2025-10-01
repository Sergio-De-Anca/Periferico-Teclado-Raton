#include "ControladorPantalla.h"

Adafruit_GC9A01A tft(TFT_CS, TFT_DC);

const int anchoTFT = 240;
const int altoTFT = 240;
const int centerX = anchoTFT / 2;
const int centerY = altoTFT / 2;
const int radioTft = 120;

uint16_t colorFondo = tft.color565(200, 200, 200); //gris claro
uint16_t colorCaracter = GC9A01A_BLACK; //negro
uint16_t colorCaracterEspecial = tft.color565(255, 0, 0); //rojo

void inicializarPantalla(){
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(colorFondo);  
}
  /**************************/
  /***Dibujar los sectores***/
  /**************************/
void dibujarSeparaciones(uint8_t nSeparaciones) {
  float paso = 360.0 / nSeparaciones;  // Ángulo entre sectores
  float offset = paso / 2.0;           // Opcional: centra las divisiones

  for (int i = 0; i < nSeparaciones; i++) {
    float angle = (i * paso) + offset;
    float x1 = centerX + radioTft * cos(angle * M_PI / 180.0);
    float y1 = centerY + radioTft * sin(angle * M_PI / 180.0);
    tft.drawLine(centerX, centerY, x1, y1, GC9A01A_BLACK);
  }
}
  /****************************/
  /***Dibujar los caracteres***/
  /****************************/
void dibujarCaracteres(const uint8_t ruleta[], int posicionSeleccionada/*Direccion*/, uint8_t ndivisiones) {
  //Limpio la pantalla y dibujo las separaciones
  tft.fillScreen(tft.color565(200, 200, 200));
  dibujarSeparaciones(ndivisiones);
  tft.setTextSize(4);
  // Dibujo el caracter según parámetros de entrada
  for (int i = 0; i < ndivisiones; i++) {
    float paso = 360.0 / ndivisiones;
    float offset = paso / 2.0;
    float angle = (i * paso);
    int radioTexto = radioTft * 0.8;
    int textoX = centerX + radioTexto * cos(angle * M_PI / 180.0);
    int textoY = centerY + radioTexto * sin(angle * M_PI / 180.0);
    // La posición seleccionada se dibuja en rojo, resto en negro
    if (i == posicionSeleccionada) {
      tft.setTextColor(colorCaracterEspecial);
    } else {
      tft.setTextColor(colorCaracter);
    }
    tft.setCursor(textoX - 15, textoY - 15);
    tft.print((char)ruleta[i]);
   } 
}