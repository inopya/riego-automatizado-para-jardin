
/*
   ================================================================================================== 
                                              VARIABLES
   ================================================================================================== 

   necesita de:
    #include "enumeradores.h"
*/




/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        SECCION DE DECLARACION DE CONSTANTES  Y  VARIABLES GLOBALES
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

volatile int16_t incrementoEncoder = 0; //int8_t es suficiente. mas grande para debug

volatile uint32_t contadorMedioSegundo = 0;   // control de medios segundos. cada dos ciclos modifica los segundos de nuestro reloj 

uint32_t last_update_time = 0;                //para el control de actualizacion de variables del entorno
  
int estado_actual = MQ_REPOSO;                // maquina de estados de nuestro sistema de riego
//int ESTADO_ANTERIOR = -1;                   //sin uso en esta implementacion de maquina de estados imple
 
volatile bool FLAG_run_clock = false;

uint8_t timer_operation_mode = OP_MODE_CLOCK;   // OP_MODE_TIMER / OP_MODE_CLOCK 

uint32_t XTAL_SEGUNDOS = 0;
uint32_t time_out_counter=0;

 
//control de tiempo del reloj
int8_t contadorHoras=0;
int8_t contadorMinutos=0;
int8_t contadorSegundos=0;


uint32_t momento_inicio_riego=0;                // control para no sobrepasar el tiempo maximo de operacion del motor

volatile boolean FLAG_estado_pulsador = false;  //sin uso


//variables de nuestro ambiente. Iniciadas con valores al azar para debug
int16_t luz_solar         = 40;
int    temp_ambiente      = 21.3;
int    humedad_ambiente   = 65;
float   temp_suelo        = 15.6;
uint8_t humedad_suelo     = 89;

uint16_t milivoltios;


bool FLAG_error_sondaDS18B = false;   //sin uso, para implementar error por problemas con la sonda


//  utilidad para generar caracteres personalizados
//  https://www.aladuino.com.mx/blog/generador-de-caracteres-especiales-para-lcd-alfanumericas/
//
//  https://maxpromer.github.io/LCD-Character-Creator/


const byte customChar_flecha1[]  = {  
  B10000,
  B11000,
  B11100,
  B11110,
  B11100,
  B11000,
  B10000,
  B00000
};

const byte customChar_flecha2[] = {
  B10000,
  B11000,
  B01100,
  B00110,
  B01100,
  B11000,
  B10000,
  B00000
};


const byte customChar_grado[] = {
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000,
  B00000
 };
