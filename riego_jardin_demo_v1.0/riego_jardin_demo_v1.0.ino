/*
#       _\|/_   A ver..., ¿que tenemos por aqui?
#       (O-O)        
# ---oOO-(_)-OOo---------------------------------
 
 
##########################################################
# ****************************************************** #
# *           DOMOTICA PARA PRINCIPIANTES              * #
# *          DEMO riego para jardin/maceta             * #
# *          Autor:  Eulogio López Cayuela             * #
# *                                                    * #
# *       Versión 1.0       Fecha: 19/01/2021          * #
# ****************************************************** #
##########################################################
*/


/*
      ===== NOTAS DE LA VERSION ===== 

     Miniriego automatizado, 
     Muy , muy sencillo pero ampliable obviamente.
     Es solo un ejercicio practico de como usar Arduio para el control de tareas
     Es a us vez un ejemplo de uso de maquinas de estado simples.
     y de como organizar nuestros programas en distintos ficheros 
     para hacer mas facil su mantenimemiento.

*/




/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        IMPORTACION DE LIBRERIAS 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/


//respetar el orden de importacion porque hay dependencia entre ellas

#include  <Arduino.h>

//#include <Temporizador_inopya.h>   //uso simple de temporizaciones (sin uso)   https://github.com/inopya/Temporizador_Virtual
//#include <ClickButton.h>           //uso simple de pulsadores (sin uso)        https://github.com/inopya/inopya_ClickButton_v2_pullup

#include "definiciones.h"
#include "enumeradores.h"
#include "variables.h"
#include "pinout.h"

#include "pantalla.h"         // funciones de control del display y muestra de mensajes
#include "encoder.h"          // funciones para la lectura del encoder, tanto el giro como su pulsador
#include "temporizadorISR.h"  // funciones de temporizacion usando Timer1
#include "led_rgb.h"          // sin uso. Por si en el futuro incluimos un led de estado.
#include "sensores.h"         // Aglutina las funciones de acceso a los sensores de humedad, temperatura...




/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        SECCION DE CREACION DE OBJETOS
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

//Temporizador_inopya cronometroVirtual;



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        PROTOTIPADO DE FUNCIONES
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

void mostarDatosSerialDebug( void );



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
   ****************************************************************************************************** 
                                    FUNCION DE CONFIGURACION
   ****************************************************************************************************** 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

void setup(void)
{

  SERIAL_BEGIN(115200);
  PRINTLN (F(__VERSION__)); 

  pinMode(PIN_ENCODER_SW, INPUT_PULLUP);
  pinMode(PIN_ENCODER_CLK, INPUT);
  pinMode(PIN_ENCODER_DT, INPUT);  

  
  pinMode(PIN_LED_OnBoard, OUTPUT);				  // declarar el pin 13 como salida
  digitalWrite(PIN_LED_OnBoard, LOW); 			// apagar el led del pin 13
  
  Setup_pantalla();
  delay(200);
  LCD1602.createChar(0, customChar_flecha1);
  LCD1602.createChar(1, customChar_grado);


  start_Timer1();    //iniciamos nuestro reloj/temporizador software (a falta de un RTC)

  pantallaInfo(COD_INFO_BIENVENIDA);
  
  actualizar_variables(SENSOR_ALL);  //  actualizacioj inicial de todas las variables
  delay(PAUSA_MENSAJES*1000);
  

  // Es suficiente con leer la interrupcion de la linea CLK
  attachInterrupt(digitalPinToInterrupt(BIT_ENCODER_CLK), updateEncoder_ISR, CHANGE);  //CLK
   
  //pantallaInfo(COD_INFO_AMBIENTE);
  last_update_time = 0;
  estado_actual = MQ_REPOSO;
}



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
   ****************************************************************************************************** 
                                  BUCLE PRINCIPAL DEL PROGRAMA
   ****************************************************************************************************** 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

void loop(void)
{ 
  bool FLAG_update_lcd = false;           //controla si se ha de actualizar el lcd ante una accion del usuario
  static uint32_t medioSegundoAnterior;
  uint32_t momento_actual = millis();
  static uint8_t FLAG_flipflop_screen;


  /* ========================================================================== */
  /*  comprobar condiciones para toma de decisiones sobre operaciones de riego  */
  /* ========================================================================== */
  if( estado_actual == MQ_REPOSO && humedad_suelo < HUM_S_MINIMA && luz_solar < LUZ_SOLAR_MAXIMA ){
    if(temp_ambiente>TEMP_A_MINIMA && temp_ambiente<TEMP_A_MAXIMA){
      //se dan las condiciones para iniciar un riego automatico
      estado_actual = MQ_RIEGO_START;
    }
  }
  else if( estado_actual == MQ_RIEGO_ON && humedad_suelo> HUM_S_MAXIMA){
    //Parar riego si el suelo está suficientemente humedo
    estado_actual = MQ_NORMAL_STOP;
  }
  else if( estado_actual == MQ_RIEGO_ON && (momento_actual-momento_inicio_riego) > LIMITE_TIEMPO_RIEGO * 60000 ){
    //Limitar el tiempo de operacion de la bomba por seguridad
    estado_actual = MQ_EMERGENCY_STOP;
  }



  /* ==================================================== */
  /*     MAQUINA DE ESTADOS PARA OPERATIVA DE RIEGO       */
  /* ==================================================== */
  switch (estado_actual)  
    {     
      case MQ_REPOSO:
        /* actualizacion periodica del display con los datos de los sensores */ 
        if(contadorMedioSegundo!=medioSegundoAnterior){
          //alternar informacion mostrada cada 3 segundos. 
          //Durante 3 segundos mostramos datos de temperatura y humedad delaire 
          //y  los siguentes 3 segundos datos del suelo, ya que en el diplay no cabe todo con "comodidad"
          if(medioSegundoAnterior%6==0){   
            if(FLAG_flipflop_screen==false) { pantallaInfo(COD_INFO_AMBIENTE); FLAG_flipflop_screen=true; } 
            else{ pantallaInfo(COD_INFO_SUELO); FLAG_flipflop_screen=false; }
          }
          medioSegundoAnterior = contadorMedioSegundo;
        }
        /* actualizacion periodica de las variables del entorno (son globales)*/
        if( (last_update_time - momento_actual) > (UPDATE_INFO_TIME*60000) ){
          last_update_time = momento_actual + (UPDATE_INFO_TIME*60000); 
          actualizar_variables(SENSOR_ALL);    
          mostarDatosSerialDebug();   //DEBUG  
        }
        break;
      
      case MQ_RIEGO_START:
        /* se dan las condiciones para iniciar un riego automatico */
        pantallaInfo(COD_INFO_INICIO_RIEGO);
        delay(PAUSA_MENSAJES*1000);
        digitalWrite(PIN_BOMBA, HIGH);
        momento_inicio_riego = millis();
        contadorHoras=0;
        contadorMinutos=0;
        contadorSegundos=0;
        contadorMedioSegundo=0; //para asegurarnos el refresco de pantalla
        FLAG_run_clock = true;
        estado_actual = MQ_RIEGO_ON;
        PRINTLN(F("HUMEDAD BAJA > RIEGO ON"));
        FLAG_flipflop_screen=0;
        break;

      case MQ_RIEGO_ON:
        /* Riego activo, actualizacion periodica del display en modo riego */ 
        if(contadorMedioSegundo!=medioSegundoAnterior){ 
          medioSegundoAnterior = contadorMedioSegundo;
          if(FLAG_flipflop_screen%2==0){
            actualizar_variables( SENSOR_H_SUELO );       // cada segundo
            pantallaInfo(COD_INFO_RIEGO);                 // cada segundo
          }
          animacionRUN(FLAG_flipflop_screen);             // cada medio segundo 
          FLAG_flipflop_screen++;
          if(FLAG_flipflop_screen>3){FLAG_flipflop_screen=0;}   
        }
        break;
                   
      case MQ_NORMAL_STOP:
        /* Parar riego si el suelo está suficientemente humedo */
        digitalWrite(PIN_BOMBA, LOW);
        FLAG_run_clock = false;
        estado_actual = MQ_REPOSO;
        pantallaInfo(COD_INFO_FIN_RIEGO);
        PRINTLN(F("HUMEDAD OPTIMA > RIEGO OFF"));
        delay(PAUSA_MENSAJES*1000);
        last_update_time=0;  //asi permite mostrar enseguida la pantalla de datos generales
        break;
        
      case MQ_EMERGENCY_STOP:
        /* Limitar el tiempo de operacion de la bomba por seguridad */
        digitalWrite(PIN_BOMBA, LOW);
        FLAG_run_clock = false;
        pantallaInfo(ERROR_TIEMPO_MAXIMO); // mostrar mensaje de parada por rebasar limite temporal 
        estado_actual = MQ_REPOSO;
        PRINTLN(F("LIMITE USO MOTOR > STOP SEGURIDAD > RIEGO OFF"));
        delay(PAUSA_MENSAJES*1000);
        last_update_time=0;  //asi permite mostrar enseguida la pantalla de datos generales
        break;
      default:
        break;
    }  




  /* ===================================================================== */
  /*     Ver si hay  giro del encoder o pulsacion                          */  
  /* ===================================================================== */

  /* 
   *  "ESTADO DEL ARTE" 
   *  podriamos usar la pulsacion para iniciar un riego manual
   *  debariamos implemetar algunas opciones de seguridad para ello.
   *  O quizas implementar algun tipo de menu para cambiar ciertos parametros
   *  como el tiepo de riego automatico o manual, o los limites de humedad o temperatura...
   *  Por ahora el uso del encoder solo activa la luz del display, 
   *  ya que esta se apaga pasados unos segundos definidos por TIME_OUT en enumeradores.h
   *  Hemos usado el giro del encoder para simular la humedad del suelo durante las pruebas.
   *  Dichas funcionalidades han quedado comentadas aqui y en sensores.h 
   *  pero no se ha borrado el codigo correspondiente, por tanto se puede recurrir a dicho modo de operacion
   *  si se desea. Si no se dispone de un encoder, se puede sustituir por un potenciometro, con muy poco esfuerzo
   *  o utilizar un simple pulsador apra la funcion de activar la luz del display.
    */
     
  uint8_t pulsador = leer_pulsador_encoder(); 
  
  if(pulsador){ 
    FLAG_update_lcd = true;
    //¿¿ estado_actual = MQ_RIEGO_MANUAL ??;   
  }
  
  if(incrementoEncoder!=0){
    //humedad_suelo+=incrementoEncoder;                     //DEBUG
    //PRINT(F("humedad_suelo: "));PRINTLN(humedad_suelo);   //DEBUG
    incrementoEncoder=0;
    FLAG_update_lcd = true;
  }
  
 
  /* ====================================================================*/
  /*  actualizacion del display si se ha manipulado el encoder           */
  /* ====================================================================*/
  if(FLAG_update_lcd){ 
    FLAG_update_lcd=false;   
    time_out_counter = 0; //
    //reset de banderas y si lcd esta apagado, lo encendemos
    if( !LCD1602.getBacklight() ){ LCD1602.setBacklight(true); }
  }
  else if(time_out_counter > TIME_OUT_LCD && LCD1602.getBacklight() ){
    //si han pasado mas de "TIME_OUT_LCD" segundos y el lcd esta encendido, lo apagamos
    LCD1602.setBacklight(false); 
  }
}




/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
        BLOQUE DE FUNCIONES: LECTURAS DE SENSORES, COMUNICACION SERIE, INTERRUPCIONES...
   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/


/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//     DEBUG 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

void mostarDatosSerialDebug()
{ 
  PRINT("Humedad aire: \t\t");PRINT(humedad_ambiente);PRINTLN(" %");
  PRINT(F("Temperatura aire: \t")); PRINT(temp_ambiente); PRINTLN(F(" ºC")); 
  PRINT("Humedad suelo: \t\t");PRINT(humedad_suelo);PRINTLN(" %");
  PRINT(F("Temperatura suelo: \t")); PRINT(temp_suelo); PRINTLN(F(" ºC"));    
  PRINT(F("Bateria: \t\t")); PRINT(milivoltios/1000.0, 2); PRINTLN(F(" v")); 
  PRINTLN(F("*********************************\n"));   
}




//*******************************************************
//                    FIN DE PROGRAMA
//*******************************************************
