# Riego automatizado para jardin  
##(EN DESARROLLO...)
Practicas con Arduino.
Creacion de un riego automatizado a escala jardin domestico.

Podremos extender las funcionalidades de nuestro montaje tanto como queramos.
Las posibilidades son "infinitas", desde lo mas básico, un riego temporizado, a que este se produzca en funcion de la humedad del suelo o que tenga en cuenta variables climatologicas para asi evitar regar si llueve o hacerlo evitando las horas de sol...
Podremos tambien dotar a nuestro sistema de una opcion de riego manual, e incluso tener en cuenta esos periodos de riego manual para ajustar de esa forma los que e producen de forma automatica y/o periodica.
Quizás deseemos implementar comunicaciones BlueTooth o wifi para disponer de informacion y control desde una aplicacion movil o desde programas de mensajeria como telegram.

Lista de materiales:

- Arduino UNO (NANO/MEGA...)
- Mini bomba de riego para jardín (de entre 3 y 6V).
- Dado que la bomba que usaremos tiene un consumo de entre 150 y 200 mA, no será factible conectarla directamente a una salida de Arduino
  Necesitaremos por tanto un Rele para el control de la limentacion de la bomba o bien controlar este a traves de un transistor, 
  opcion muy interesante dado el no excesivo consumo de esta. Y un transistor nos brinda la opcion de una rapida conmutacion y nos "libera" del desgaste que no podriamos     evitar en los contactos de un relé. 
- Higrometro de suelo
- Sensor de humedad ambiente
- Sensor de Temperatura
- Sensor de luz
- Sensor BlueTooth
- ESP8266

**Posible esquema conexion para nuestra bomba**

![](./Arduino-Motor_DC-y-transistorPNP.png)
