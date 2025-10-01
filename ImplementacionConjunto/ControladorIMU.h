#ifndef IMU_CONTROLLER_H
#define IMU_CONTROLLER_H

#include "SparkFunLSM6DSO.h"
#include "Wire.h"
#include <BleCombo.h>

extern LSM6DSO myIMU;

void inicializarIMU();
void moverRatonSuavizado();
#endif