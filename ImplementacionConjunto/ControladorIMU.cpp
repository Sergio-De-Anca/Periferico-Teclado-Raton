#include "ControladorIMU.h"

LSM6DSO myIMU; // I2C, dirección 0x6B
const float UMBRAL_GIRO = 5.0f;  // Umbral mínimo para movimiento (dps)
const float FACTOR_SENSIBILIDAD = 0.30f; // Ajusta la sensibilidad del ratón

// Variables para filtro paso bajo
float filtroX = 1.0f;
float filtroY = 1.0f;
float filtroZ = 1.0f;
const float ALFA = 0.8f; // factor de suavizado 

int orientacionActual;
int orientacionAnterior;

void inicializarIMU() {
  Wire.begin();
  delay(10);
  
  if (myIMU.begin()) {
    Serial.println("Ready.");
  } else {
    Serial.println("Could not connect to IMU.");
    Serial.println("Freezing");
    while (1); // Congela si falla
  }

  if (myIMU.initialize(BASIC_SETTINGS)) {
    Serial.println("Loaded Settings.");
  }
}
  /*********************************/
  /***Mueve el raton por pantalla***/
  /*********************************/
void moverRatonSuavizado() {
    // Leer velocidades angulares (dps)
    float gx = myIMU.readFloatGyroX();
    float gy = myIMU.readFloatGyroY();
    float gz = myIMU.readFloatGyroZ();
    // Aplicar filtro paso bajo para suavizar
    filtroX = ALFA * gx + (1 - ALFA) * filtroX;
    filtroY = ALFA * gy + (1 - ALFA) * filtroY;
    filtroZ = ALFA * gz + (1 - ALFA) * filtroZ;
    // Ignorar movimientos muy pequeños (ruido)
    if (abs(filtroX) < UMBRAL_GIRO) filtroX = 0;
    if (abs(filtroY) < UMBRAL_GIRO) filtroY = 0;
    if (abs(filtroZ) < UMBRAL_GIRO) filtroZ = 0;
    // Mapear velocidades del giroscopio a movimientos del ratón
    int moveX = int(filtroZ * FACTOR_SENSIBILIDAD);  // Giros Z → desplazamiento X del ratón (horizontal)
    int moveY = int(-filtroX * FACTOR_SENSIBILIDAD); // Giros X → desplazamiento Y del ratón (vertical) (invertido)
    int wheel = int(filtroY * 0.05);              // Giros Y → rueda del ratón (scroll)
    // Solo mover si hay movimiento significativo
    if (moveX != 0 || moveY != 0 || wheel != 0) {
        Mouse.move(-moveX, moveY, wheel);
    }
}
  /************************************/
  /***Muestra orientacion del sensor***/
  /************************************/
void mostrarOrientacion(int orientacionActual) {
  if (orientacionActual != 0) {
    orientacionAnterior = orientacionActual;
  }
  Serial.print("\nDirección de giro anterior: ");
  Serial.println(orientacionAnterior);
  Serial.print("\nDirección de giro actual: ");
  Serial.println(orientacionActual);
}
  /****************************************************************************/
  /***Funciones para obtener datos de acelerometro, giroscopio y temperatura***/
  /****************************************************************************/
static void leerAcelerometro() {
  Serial.print("\nAccelerometer:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readFloatAccelX(), 3);
  Serial.print(" Y = ");
  Serial.println(myIMU.readFloatAccelY(), 3);
  Serial.print(" Z = ");
  Serial.println(myIMU.readFloatAccelZ(), 3);
}
static void leerGiroscopio() {
  Serial.print("\nGyroscope:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readFloatGyroX(), 3);
  Serial.print(" Y = ");
  Serial.println(myIMU.readFloatGyroY(), 3);
  Serial.print(" Z = ");
  Serial.println(myIMU.readFloatGyroZ(), 3);
}
static void leerTemperatura() {
  Serial.print("\nThermometer:\n");
  Serial.print(" Degrees F = ");
  Serial.println(myIMU.readTempF(), 3);
}
void leerYMostrarDatosIMU() {
  leerAcelerometro();
  leerGiroscopio();
  leerTemperatura();
}