#include "DispositivoHID.h"

DispositivoHID dispositivoHID;


void setup() {
  Serial.begin(115200);
  delay(5000);
  dispositivoHID.inicializar();
  Serial.println("Inicializado");
}

void loop() {
  if (!dispositivoHID.deviceConnected) return;

  // 1. Pulsar y soltar una tecla (tecla "A", keycode 0x04)
  dispositivoHID.pulsarTecla(0x04);
  delay(100);
  dispositivoHID.soltarTecla();
  delay(500);

  // 2. Pulsar Ctrl + C
  dispositivoHID.pulsarTeclaModificada(0x01, 0x06);  // Ctrl + C
  delay(100);
  dispositivoHID.soltarTecla();
  delay(500);

  // 4. Mover ratón en diagonal (derecha + abajo)
  dispositivoHID.moverRaton(20, 20);
  delay(500);

  // 5. Click izquierdo
  dispositivoHID.clickIzquierdo();
  delay(500);

  // 6. Click derecho
  dispositivoHID.clickDerecho();
  delay(500);

  // 7. Scroll hacia abajo (−1)
  dispositivoHID.scrollRueda(-1);
  delay(300);

  // 8. Scroll hacia arriba (+3)
  dispositivoHID.scrollRueda(+3);
  delay(300);

  // 9. Mantener tecla "W" presionada (ej. para un juego)
  dispositivoHID.mantenerTecla(0x1A);  // W
  delay(1000);                         // Mantener 1s
  dispositivoHID.soltarTecla();
  delay(500);

  // Repetir solo una vez
  delay(1000);  // Detener el loop

  Serial.println("Iteración completada");
  
}
