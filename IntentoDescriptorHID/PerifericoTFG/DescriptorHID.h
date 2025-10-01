#pragma once

const uint8_t hidReportDescriptor[] = {
  // Teclado
  /*
  1 Byte indica id = 1
  1 Byte Teclas modificadoras	1 byte	Ctrl, Shift, Alt, etc. (8 bits para 8 teclas: E0-E7)
  1 Byte Relleno constante	
  1 Byte LEDs (NumLock, etc.)	5 bits usados, 3 de relleno
  6 Bytes Teclas presionadas, hasta 6 teclas normales presionadas simultáneamente
    */
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
  0x09, 0x06,                    // USAGE (Keyboard)
  0xA1, 0x01,                    // COLLECTION (Application)
  0x85, 0x01,                    //   REPORT_ID (1)
  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
  0x19, 0xE0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
  0x29, 0xE7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
  0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
  0x75, 0x01,                    //   REPORT_SIZE (1)
  0x95, 0x08,                    //   REPORT_COUNT (8)
  0x81, 0x02,                    //   INPUT (Data,Var,Abs)
  0x95, 0x01,                    //   REPORT_COUNT (1)
  0x75, 0x08,                    //   REPORT_SIZE (8)
  0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
  0x95, 0x05,                    //   REPORT_COUNT (5)
  0x75, 0x01,                    //   REPORT_SIZE (1)
  0x05, 0x08,                    //   USAGE_PAGE (LEDs)
  0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
  0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
  0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
  0x95, 0x01,                    //   REPORT_COUNT (1)
  0x75, 0x03,                    //   REPORT_SIZE (3)
  0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
  0x95, 0x06,                    //   REPORT_COUNT (6)
  0x75, 0x08,                    //   REPORT_SIZE (8)
  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
  0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
  0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
  0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
  0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
  0xC0,                          // END_COLLECTION

  // Ratón 
  /*
  1 Byte indica id = 2
  1 byte: botones (3 bits) + relleno (5 bits)
  1 byte: movimiento X
  1 byte: movimiento Y
  1 byte: rueda (scroll)
  */
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
  0x09, 0x02,                    // USAGE (Mouse)
  0xA1, 0x01,                    // COLLECTION (Application)
  0x85, 0x02,                    //   REPORT_ID (2)
  0x09, 0x01,                    //   USAGE (Pointer)
  0xA1, 0x00,                    //   COLLECTION (Physical)
  0x05, 0x09,                    //     USAGE_PAGE (Button)
  0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
  0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
  0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
  0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
  0x95, 0x03,                    //     REPORT_COUNT (3)
  0x75, 0x01,                    //     REPORT_SIZE (1)
  0x81, 0x02,                    //     INPUT (Data,Var,Abs)
  0x95, 0x01,                    //     REPORT_COUNT (1)
  0x75, 0x05,                    //     REPORT_SIZE (5)
  0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
  0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
  0x09, 0x30,                    //     USAGE (X)
  0x09, 0x31,                    //     USAGE (Y)
  0x09, 0x38,                    //     USAGE (Wheel)  <<<<<< Scroll vertical
  0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
  0x25, 0x7F,                    //     LOGICAL_MAXIMUM (127)
  0x75, 0x08,                    //     REPORT_SIZE (8)
  0x95, 0x03,                    //     REPORT_COUNT (3)  ; X, Y, Wheel
  0x81, 0x06,                    //     INPUT (Data,Var,Rel)
  0xC0,                          //   END_COLLECTION
  0xC0                           // END_COLLECTION
};
