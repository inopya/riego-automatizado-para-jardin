
/*
   ================================================================================================== 
                                         TEMPORIZADOR ISR
   ================================================================================================== 

   necesita de:
    #include "enumeradores.h"
    #include "variables.h"
*/

#include  <Arduino.h>



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        PROTOTIPADO DE FUNCIONES
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

void start_Timer1( void );            
void stop_Timer1( void );             
void start_Timer2( void );             
void stop_Timer2( void );              
void Reloj_ISR( void );                
void CuentaAtras_ISR( void);               




/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//       INTERRUPCIONES  SOFTWARE  TIMER 1
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

//========================================================
// INICIAR TIMER 1
//========================================================

void start_Timer1()
{
  /* Setup Timer2  para que junto a la funcion de atencion a su interrupcion
   * poder obtener una 'señal util' cada 500 ms */
  TCCR1A = 0x00;        // registro de control A:  Wave Gen Mode normal
  TCCR1B = 0x00;        // deshabilita Timer1 mientras lo estamos ajustando
  TCNT1  = 0xE174; ;    //(57716) establecer el contador iniciandolo con este valor para contar 7820
  TIFR1  = 0x00;        // Timer2 INT Flag Reg: borrar la bandera de desbordamiento
  TIMSK1 = 0x01;        // Timer2 INT Reg: habilita la interrupcion de desbordamiento de Timer2
  TCCR1B |= (1 << CS10)|(1 << CS12);    // prescaler de timer 1 en 1024
}



//========================================================
// PARAR TIMER 1
//========================================================

void stop_Timer1()
{
 TCCR1B = 0x00;         // deshabilita Timer1 mientras lo estamos ajustando
 TIMSK1=0 ;             // Timer1 INT Reg: deshabilita la interrupcion de desbordamiento de Timer2
}




//========================================================
// RUTINA DE INTERRUPCION PARA DESBORDAMIENTO DE TIMER 2
// El vector de desbordamineto es -->  TIMER1_OVF_vect
//========================================================

ISR(TIMER1_OVF_vect) 
{
  /* 
     Con el preescaler a 1024, contando desde 57716 en TCNT1 para contabilizar 7820 ciclos 
     obtenemos una señal que permitetemporizar 500ms con bastante exactitud
     Este Reloj por software adelanta 0.1 segundos cada 1 hora
     ** +11 segundos en 100 horas Arduino UNO/NANO 5v/4.1v

  */
  
  contadorMedioSegundo++;
  if( timer_operation_mode == OP_MODE_CLOCK){ Reloj_ISR(); }
  else{ CuentaAtras_ISR(); }   

  TCNT1 = 0xE174;                 // 0xE174 reset del contador iniciandolo con el valor 57716 para contar 7820
  TIFR1 = 0x00;                   // Timer1 INT Flag Reg: borrar la bandera de desbordamiento
}



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//       TEMPORIZADORES SOFTWARE BASADOS EN INTERRUPCIONES TIMER1/TIMER2...
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

//========================================================
// RELOJ CON INTERRUPCION SOFTWARE
//========================================================

void Reloj_ISR()
{
  /*
  * Reloj relativametne preciso mediante software
  * y el uso de interrupciones intermas del timer02 
  * Podemos ponerlo en hora actuando sobre las variables globales 
  * 'horas', 'minutos' y 'segundos'  definidas al inicio del codigo
  * 
  * Usado para mostrar tiempo en pantalla unicamente como un contador creciente
  */


  if(contadorMedioSegundo%2==0){
    XTAL_SEGUNDOS++;
    time_out_counter++;
    if( FLAG_run_clock == false){ return; }  //permite hacer pausas en el reloj 
    contadorSegundos ++;
    if(contadorSegundos == 60){
      contadorSegundos = 0;
      contadorMinutos ++;
      if(contadorMinutos == 60){
        contadorMinutos = 0;
        contadorHoras ++;
        if(contadorHoras == 24){
          contadorHoras = 0;
        }
      }
    }  
  }
}



//========================================================
// CUENTA ATRAS CON INTERRUPCION SOFTWARE 
//========================================================

void CuentaAtras_ISR()
{
  /*
  * Contador descendente mediante el uso de interrupciones intermas del timer02
  */

  if( FLAG_run_clock == false){ return; }
  
  if(contadorHoras==0 AND contadorMinutos==0 AND contadorSegundos==0){
    /* por si se queda activo el servicio de ISR, salimos de esta rutina si el tiempo ha llegado a cero
     evitando asi que se produzcan valores negativos que generen erroes de representacion el el LCD */                                
    FLAG_run_clock = false;
    //digitalWrite(pin motor, LOW);  //por segurudad lo paro desde aqui, aunque lo haga tambien en el loop()

  return;
  }
  
  if(contadorMedioSegundo%2 == 0){                        // cada dos medios segundos, 
    contadorSegundos --;                                  // descontamos 1 segundo
    if(contadorSegundos == -1){                           // Despues del segundo CERO,...
      contadorSegundos = 59;                              // viene el 59
      contadorMinutos --;                                 // y con ello descontar 1 minuto
      if(contadorMinutos == -1){                          
        contadorMinutos = 59;                             // El paso de CERO minutos lleva al 59
        contadorHoras --;                                 // y con ello descontar 1 hora
      }
    }
  } 
}
