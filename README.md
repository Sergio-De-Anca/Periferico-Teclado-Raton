## Periferico-Teclado-Raton
Este proyecto es un Trabajo de Fin de Grado, hecho en la Universidad de Sevilla en el “Grado
de Ingeniería Informática de Computadores” por Sergio Isidoro De Anca Garabito. Este trabajo se
realiza dentro del departamento de “Arquitectura y tecnología de computadores” y está tutorizado
por Juan Manuel Montes Sanchez y cotutorizado por Javier Civit Masot.

Consiste en la creación de un periférico con funciones de teclado-ratón que puede ser
utilizado con una sola mano. Para ello se utiliza un microcontrolador que maneja una serie de
componentes electrónicos como lo son “switches” o “joysticks”.

## Estructura del proyecto
- [Videos demostrativos](./FuncionamientoPrototipo) — En esta carpeta se encuentran imágenes del prototipo del periférico y vídeos sobre su funcionamiento.
- [Firmware](./ImplementacionConjunto) — Contiene el firmware completo del periférico
- [PCB](./PCB_TFG) — Contiene el esquemático y la huella de la PCB del periférico
- [Intento descriptor HID](./IntentoDescriptorHID/PerifericoTFG) — Contiene el intento de crear una librería para que el microcontrolador actue como un dispositivo HID y se concete por BLE al computador. **No funciona
