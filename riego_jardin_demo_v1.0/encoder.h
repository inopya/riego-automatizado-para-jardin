
/*
   ================================================================================================== 
                                         ENCODER
   ================================================================================================== 

   necesita de:
    #include "definiciones.h"
    #include "pinout.h"
*/



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        PROTOTIPADO DE FUNCIONES
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

uint8_t leer_pulsador_encoder( void );
void updateEncoder_ISR( void );           // configurable para simple o doble resolucion


/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//       FUNCIONES ENCODER
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

//========================================================
//  FUNCION PARA LEER PULSADOR DEL ENCODER 
//========================================================

uint8_t leer_pulsador_encoder()
{
  static uint8_t estadoSW_old = 1;
  uint8_t estadoSW;
  
  //estadoSW = digitalRead(PIN_ENCODER_SW)?0:1;  
  estadoSW = getBit(PUERTO_ENCODER, BIT_ENCODER_SW)?0:1;
  if(estadoSW && !estadoSW_old){
    estadoSW_old=estadoSW;
    return 1;
  }
  estadoSW_old=estadoSW;
  return 0;
}



//========================================================
//  FUNCION PARA LEER ENCODER ROTATORIO (solo para ISR) (USANDO PUERTOS)
//========================================================

void updateEncoder_ISR()
{ 
  int8_t estadoPuerto=PUERTO_ENCODER; // dejamos leido el puerto y evitamos posibles cambios durante las operaciones de comparacion

  //if( !getBit(estadoPuerto, BIT_ENCODER_SW) ){ return; }  //despreciar los giros en los que se pulsa el encoder simultaneamente

  int8_t currentStateCLK = getBit(estadoPuerto, BIT_ENCODER_CLK);
  static int8_t lastStateCLK = 0;
  
  if ( currentStateCLK != lastStateCLK ){

    if ( getBit(estadoPuerto, BIT_ENCODER_DT)!= currentStateCLK ) { incrementoEncoder++; }  
    else { incrementoEncoder--; }
  }
  /* Recordar el estado actual */
  lastStateCLK = currentStateCLK;
}
