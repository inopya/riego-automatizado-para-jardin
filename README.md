# Riego automatizado para jardin  

![](./en-construccion.png)

**Practicas con Arduino: Creacion de un riego automatizado a escala jardin domestico.**

(EN DESARROLLO...) Pendiente subir esquema de conexion para el codigo actual

   *  "ESTADO DEL ARTE":
   *  podriamos usar la pulsacion para iniciar un riego manual pero
   *  debariamos implemetar algunas opciones de seguridad para ello.
   *  O quizas implementar algun tipo de menu para cambiar ciertos parametros
   *  como el tiempo de riego automatico o manual, o los limites de humedad o temperatura...
   *  Por ahora el uso del encoder solo activa la luz del display, 
   *  ya que esta se apaga pasados unos segundos definidos por TIME_OUT en enumeradores.h
   *  Hemos usado el giro del encoder para simular la humedad del suelo durante las pruebas.
   *  Dichas funcionalidades han quedado comentadas aqui y en sensores.h 
   *  pero no se ha borrado el codigo correspondiente, por tanto se puede recurrir a dicho modo de operacion
   *  si se desea. Si no se dispone de un encoder, se puede sustituir por un potenciometro, con muy poco esfuerzo
   *  o utilizar un simple pulsador apra la funcion de activar la luz del display.




Podremos extender las funcionalidades de nuestro montaje tanto como queramos.
Las posibilidades son "infinitas". Desde lo mas básico: un riego temporizado, a que este se produzca en función de la humedad del suelo o que tenga en cuenta variables climatológicas para asi evitar regar si llueve o hacerlo evitando las horas de sol...
Podremos tambien dotar a nuestro sistema de una opción de riego manual e incluso tener en cuenta esos periodos de riego manual para ajustar de esa forma los que se han de producir de forma automática y/o periódica.
Quizás deseemos implementar comunicaciones BlueTooth o WiFi para disponer de información y control desde una aplicación móvil o desde programas de mensajeria como Telegram.

Lista de materiales (todo dependerá de complejo que deseemos hacerlo):

- Arduino UNO (NANO/MEGA...)
- Mini bomba de riego para jardín (de entre 3 y 6V).
- Dado que la bomba que usaremos tiene un consumo de entre 150 y 200 mA, no será factible conectarla directamente a una salida de Arduino
  Necesitaremos por tanto un Rele para el control de la limentacion de la bomba o bien controlar este a traves de un transistor, 
  opcion muy interesante dado el no excesivo consumo de esta. Y un transistor nos brinda la opcion de una rapida conmutacion y nos "libera" del desgaste que no podriamos     evitar en los contactos de un relé. 
- Higrometro de suelo
- Sensor de humedad ambiente
- Sensor de Temperatura
- Sensor de luz, podemos usar una LDR (o un simple diodo led: https://github.com/inopya/Diodo_led_como_sensor_de_luz)
- Módulo de comunicaciones BlueTooth
- ESP8266 (para comunicaciones WiFi)
- RTC DS3231 (o similar)
- Diodos rectificadores, leds, resistencias, transistores...
- .....


**Posible esquema conexión para nuestra bomba**

Podemos disponer de un pulsador para accionamiento manual de la bomba asi como un led indicador de que esta está en marcha.

![](./Arduino-Motor_DC-y-transistorPNP.png)


Podemos observar que junto a nuestro motor aparece un diodo en una "extraña" posición.
A esta configuración se la denomina *"Diodo en Antiparalelo"*

**DIODO EN ANTIPARALELO**

Su principal uso es como elemento de descarga para las bobinas.
Por tanto es muy interesante emplearlos junto a reles mecánicos y motores.

Cuando una bobina se conecta a una fuente de alimentación acumula energia en forma de campo magnético hasta igualar el potencial de la fuente.
Y cuando dejan de estar alimentadas ese campo magnético que almacenan como consecuencia del paso de corriente por ellas, se "disipa" devolviendose al circuito nuevamente en forma de una corriente eléctrica.
Pero el tiempo que lleva "eliminar" ese flujo de corriente de la bobina suele ser mayor que el tiempo que tarda en desconectarse la fuente de alimentación.
Asi que una vez abierto el circuito y durante unos breves instantes esa corriente puede crear elevadas tensiones de cientos de voltios en los componentes cercanos a ella que pueden superar el aislamiento del aire generando arcos eléctricos en el caso de los contactos de un rele o de un iterruptor e incluso superar el aislamiento de semiconductores como los transistores.
Tambien puede introducir ruido eléctrico en nuestro circuito y provocar que elementos como los microcontroladores se bloqueen o se reinicien.
Un diodo colocado en esta configuración entra en conduccion en el momento de la desconexión y se encarga de hacer retornar dicha corriente electrica sobre la propia bobina, evitando de esta forma que afecte a partes sensibles de nuestro circuito.

En nuestro caso es precisamente por eso que usamos esta configuracion de diodo en antiparalelo junto al motor de nuestra bomba de riego. Para evitar posibles daños en el transistor que usamos para el control de la alimentación y ademas evitar señales espurias que pudiesen interferir en el correcto funcionamiento de nuestro microcontrolador.


