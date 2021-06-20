
/*
   ================================================================================================== 
                                SENSORES DE TEMPERATURA Y HUMEDAD
   ================================================================================================== 

   necesita de:
    #include "pinout.h"
*/


/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        IMPORTACION DE LIBRERIAS 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

#include <OneWire.h>
#include <DallasTemperature.h>


/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        SECCION DE DEFINICIONES 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//     CREACION DE OBJETOS
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

DeviceAddress sondaADDR = { 0x28, 0x9E, 0x4C, 0x07, 0xD6, 0x01, 0x3C, 0xE3 };
OneWire oneWireBus(PIN_DS18B20);
DallasTemperature sondaDS18B20(&oneWireBus);
 

/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        PROTOTIPADO DE FUNCIONES
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/


void sensorDHT11(int pin, int *H, int *T);  //acceso sin librerias
float read_NTC( uint8_t pinNTC );
int comprobar_estado_bateria( byte modo=0 ); 

void actualizar_variables(uint8_t _sensor);  //actualiza las variables ambientales
void leer_luz_solar( void );
void leer_humedad_suelo( void );
void leer_temperatura_suelo( void );
void leer_ambiente( void );
void leer_bateria( void ); 
void variables_demo( void ); 


/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        SETUP SENSOR HUMEDAD AMBIENTE
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        SETUP SENSOR TEMPERATURA SUELO
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

void Setup_ds18b20() 
{
  sondaDS18B20.begin();
  sondaDS18B20.setResolution(sondaADDR, 10);
}




/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//    ACTUALIZAR VARIABLES AMBIENTALES
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

void actualizar_variables(uint8_t _sensor)
{
  //variables_demo();
  //return;
  
  switch(_sensor){
    case SENSOR_LUZ_SOLAR:
      leer_luz_solar();
      break;
    case SENSOR_H_SUELO:
      leer_humedad_suelo();
      break;
    case SENSOR_T_SUELO:
      leer_temperatura_suelo();
      break;
    case SENSOR_HT_AMBIENTE:
      leer_ambiente();
      break;
    case SENSOR_BATERIA:
      leer_bateria(); 
      break;
    case SENSOR_ALL:
      leer_luz_solar();
      leer_humedad_suelo();
      leer_temperatura_suelo();
      leer_ambiente();
      leer_bateria(); 
      break; 
    default:           
      break;   
  }    
}

//========================================================
//  variables en modo DEMO
//========================================================

void variables_demo()
{
  leer_luz_solar();
  //leer_humedad_suelo(); --> usamos el encoder
  leer_temperatura_suelo();
  leer_ambiente();
  leer_bateria();   
}


//========================================================
//  LUZ SOLAR
//========================================================

void leer_luz_solar()
{    
  // luz solar
  luz_solar = analogRead(PIN_SENSOR_LUZ);
  luz_solar  = map(luz_solar, LIMITE_ADC,0, 0,100);
  luz_solar = constrain(luz_solar, 0, 99);
}


//========================================================
//  HUMEDAD SUELO
//========================================================

void leer_humedad_suelo()
{
  // humedad suelo
  humedad_suelo = analogRead(PIN_HUMEDAD_SUELO);
  humedad_suelo  = map(humedad_suelo, LIMITE_ADC, 200, 0,100);
  humedad_suelo = constrain(humedad_suelo, 0, 99);
}   


//========================================================
//  TEMPERATURA SUELO
//========================================================

void leer_temperatura_suelo()
{
  // temperatura suelo
  temp_suelo = read_NTC(PIN_sensorNTC)* ERROR_ntc;  // si usamos nuestro sensor casero basado en una NTC

  //FLAG_error_sondaDS18B = false;
  //sondaDS18B20.requestTemperatures();             // si usamos sonda dallas DS18B20
  //temp_suelo = sondaDS18B20.getTempC(sondaADDR);
  //if( temp_suelo==-127 ){ FLAG_error_sondaDS18B = true; }
}



//========================================================
//  TEMPERATURA Y HUMEDAD DEL AIRE
//========================================================

void leer_ambiente()
{
  sensorDHT11(PIN_DHT, &humedad_ambiente, &temp_ambiente);
}  


//========================================================
//  LECTURA DE LA BATERIA
//========================================================

void leer_bateria() 

{   
  milivoltios = comprobar_estado_bateria(0);
}




/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//    SENSOR NTC
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

float read_NTC(uint8_t pinNTC) 
{
  //const int Vcc = 5;
  const int Rc = 10000;       //valor de la resistencia en serie con la NTC
  float K = 2.5;              //factor de disipacion en mW/C
  float A = 1.11492089e-3;
  float B = 2.372075385e-4;
  float C = 6.954079529e-8;
  
  //------------------------------//
  
  float raw = analogRead(pinNTC);
  float Vcc = comprobar_estado_bateria(0)/1000;
  //if( Vcc>5.0 ){ Vcc=5.0; }

  float Vntc =  (raw * Vcc) / float(LIMITE_ADC);
  float Rntc = ( Vntc * Rc ) / ( Vcc - Vntc );
  
  //Serial.print(F("raw=")); Serial.println(raw); 
  //Serial.print(F("Vntc=")); Serial.println(Vntc);
  //Serial.print(F("Rntc=")); Serial.println(Rntc);
  
  float logR  = log(Rntc);
  float R_th = 1.0 / (A + B * logR + C * logR * logR * logR );
 
  float kelvin = R_th - 1000*(Vntc*Vntc)/(K * Rntc);
  float celsius = kelvin - 273.15;
  
  return celsius;
}



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//     CONTROL DEL ESTADO DE LA BATERIA / ALIMENTACION
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

//========================================================
//   FUNCION PARA MONITORIZAR EL ESTADO DE VCC
//========================================================

int comprobar_estado_bateria(byte modo) 
{
  /* ----  0 devuelve milivoltios, >0 devuelve porcentaje de carga  ---- */
  /* leer la referecia interna de  1.1V  para calcular Vcc */
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
     ADMUX = _BV(MUX5) | _BV(MUX0) ;
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  
 
  delay(2);                         // pausa para que Vref se estabilice
  ADCSRA |= _BV(ADSC);              // iniciar medicion
  while (bit_is_set(ADCSRA,ADSC));  // proceso de medicion propiamente dicho
 
  uint8_t low  = ADCL; // leer ADCL
  uint8_t high = ADCH; // leer ADCH
 
  long lecturaACD = (high<<8) | low;
 
  long milivoltios = 1125300L / lecturaACD;           // Calcular Vcc en mV (1125300 = 1.1*1023*1000)
  float voltaje = milivoltios/1000.0;                 // estado de la bateria en mV 
  float porcentage_carga = (100-(4.2-voltaje)*100);   // una LiPo al 100% tiene 4'2V, al 0% --> 3'2V
  
  if(porcentage_carga<0){ porcentage_carga = 0;}      // ojito la bateria estaria por debajo de 3'2 voltios
  if(porcentage_carga>100){ porcentage_carga = 100;}  // ojito la bateria estaria por encima de 4'2 voltios

  if(modo == 0){ return int(milivoltios); }           // Voltaje de la bateria en milivoltios
  if(modo > 0){ return int(porcentage_carga); }       // carga restante en porcentaje  
}



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//    UTILIDAD para encontar la direccion del sensor Dallas DS18B20
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

void discoverOneWireDevices(void) 
{
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  
  Serial.println("Buscando dispositivos 1-Wire");
  while(oneWireBus.search(addr)) {
    Serial.println("Encontrado dispositivo 1-Wire en direccion");
    for( i = 0; i < 8; i++) {
      Serial.print("0x");
      if (addr[i] < 16) {
        Serial.print('0');
      }
      Serial.print(addr[i], HEX);
      if (i < 7) {
        Serial.print(", ");
      }
    }
    if ( OneWire::crc8( addr, 7) != addr[7]) {
        Serial.print("Error en dispositivo, CRC invalido!\n");
        return;
    }
  }
  Serial.println("\nBúsqueda finalizada");
  oneWireBus.reset_search();
}



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//    HIGROMETRO y TERMOMETRO, ACCEDIENDO AL SENSOR DHT11 SIN LIBRERIA ESTANDAR
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

void sensorDHT11(int pin, int *H, int *T)
{
  //secuencia para activacion del DHT11
  pinMode(pin, OUTPUT);  //configurar el pin como salida
  digitalWrite(pin, LOW);
  delay(20);  //mantener en (0)v al menos 18 ms  (20-23)
  digitalWrite(pin, HIGH);
  delayMicroseconds(25);  //mantener en (1) entre 20 y 40 us (25-30)

  //escuchar respuesta del sensor
  pinMode(pin, INPUT);  //configuramos la palilla como entrada
  //Respuesta cuando esta listo para transmitir
  while (digitalRead(pin) == 0);
  while (digitalRead(pin) == 1);
  //una vez la respuesta es OK (secuencia 0,1), comenzamos la escucha de los 40 bits de datos que nos da el sensor

  byte lecturaSensor[5];  //reserva de memoria para 5 bytes (40 bits)
  
  //bucle para leer los 5 bytes
  for (int i=0; i<5; i++){
    int tempByte = 0;  //ponemos a 0 la variable que sucesivamente ira conteniendo cada byte
    //bucle para la lectura de los 8 bits de cada byte
    for (int j=0; j<8; j++){
      while (digitalRead(pin) == 0);
      delayMicroseconds(40);
      if (digitalRead(pin) == 1){
        tempByte |= 1<<(7-j);  //asignamos cada bit 1 a su lugar correspondiente dentro del byte
      }
      while (digitalRead(pin) == 1);
    }
    lecturaSensor[i] = tempByte;
  }
  //comprobamos el checksum de los datos obtenidos y solo si no hay errores modificamos las lecturas anteriores.
  if (lecturaSensor[4] == ((lecturaSensor[0] + lecturaSensor[1] + lecturaSensor[2] + lecturaSensor[3]) & 0xFF)){
    *H = lecturaSensor[0];
    *T = lecturaSensor[2];
  } 
}
 
