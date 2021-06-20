
/*
   ================================================================================================== 
                                              PANTALLA
   ================================================================================================== 

   necesita de:
    #include "variables.h"
    #include "enumeradores.h"
*/


/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        IMPORTACION DE LIBRERIAS 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

#include  <Arduino.h>
#include <Wire.h>                     // libreria para comunicaciones I2C, necesaria para el LCD
#include <LiquidCrystal_I2C.h> 



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        SECCION DE DEFINICIONES 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

#define LCD_AZUL_ADDR    0x3F   // Direccion I2C de nuestro LCD color azul  <-- Este es el que vamos a usar
#define LCD_VERDE_ADDR   0x27   // Direccion I2C de nuestro LCD color verde


/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//         CREACION DE OBJETOS
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/


LiquidCrystal_I2C LCD1602 ( LCD_VERDE_ADDR , 16 , 2 );

//LCD1602.begin();
//LCD1602.clear();
//LCD1602.setBacklight(true); 
//LCD1602.setBacklight(estado_luz);  
//return LCD1602.getBacklight();
//LCD1602.setCursor(x,y);


/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        PROTOTIPADO DE FUNCIONES
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

void Setup_pantalla( void );

void pantallaInfo( uint8_t _codigo_info );

void refrescaCuentaAtras( uint8_t *horas,  uint8_t *minutos,  uint8_t *segundos );
void refrescaReloj( uint8_t *horas, uint8_t *minutos , uint8_t *segundos );
void animacionRUN( uint8_t _contadorAnimacion );



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        FUNCIONES PANTALLA
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

//========================================================
//  SETUP PANTALLA
//========================================================

void Setup_pantalla()
{
  LCD1602.begin();
  LCD1602.clear();
  LCD1602.setBacklight(true); 
}


//========================================================
//  MOSTAR PANTALLAS DE INFORMACION
//========================================================

void pantallaInfo(uint8_t _codigo_info)
{
  //if( estado_actual != MQ_REPOSO || estado_actual != COD_INFO_INICIO_RIEGO ){ LCD1602.clear(); }
  
  switch (_codigo_info)
  {
  case COD_INFO_BIENVENIDA:
      LCD1602.clear();
      LCD1602.setCursor(0,0);   
      //             ("0123456789ABCDEF")   // guia apara escribir sin pasarnos del espacio disponible  :)
      LCD1602.print(F("CONTROL DE RIEGO")); 
      LCD1602.setCursor(0,1);   
      LCD1602.print(F(" inopya Systems")); 
      break;

  case COD_INFO_AMBIENTE:     
      LCD1602.setCursor(0, 0);                    // posicionar cursor en 1º caracter de la primera linea (0)
      LCD1602.print(F("AMBIENTE> Luz:"));
      LCD1602.print(float(luz_solar), 0);         // en debu muestro TIME_OUT
      LCD1602.print(F("    "));                   // para borrar espureas de impresiones anteriores
      LCD1602.setCursor(0, 1);                    // posicionar cursor en 1º caracter de la segunda linea (1)
      LCD1602.print(F("Ta:"));
      LCD1602.print(float(temp_ambiente),0);      // , 0 <<< sin  decimales
      LCD1602.write(1);
      LCD1602.print(F("C  Ha:"));
      LCD1602.print(float(humedad_ambiente), 0);  // ojo, la libreria del lcd solo entinde float!!  :(
      LCD1602.print(F("%   ")); 
      break;  
                       
  case COD_INFO_SUELO: 
      LCD1602.setCursor(0, 0);                    // posicionar cursor en 1º caracter de la primera linea (0)
      LCD1602.print(F("SUELO>          "));
      LCD1602.setCursor(0, 1);                    // posicionar cursor en 1º caracter de la segunda linea (1)
      LCD1602.print(F("Ts:"));
      LCD1602.print(temp_suelo, 0);               // , 0 <<< sin  decimales
      LCD1602.write(1);
      LCD1602.print(F("C  Hs:"));
      LCD1602.print(float(humedad_suelo), 0);  // ojo, la libreria del lcd solo entinde float!!  :(
      LCD1602.print(F("%      "));    
      break;

  case COD_INFO_INICIO_RIEGO:
      LCD1602.clear();
      LCD1602.print(F("Humedad BAJA "));
      LCD1602.setCursor(0, 1); 
      LCD1602.print(F("Iniciando RIEGO"));
      break; 
      
  case COD_INFO_RIEGO:
      if( estado_actual == MQ_RIEGO_START ) { LCD1602.clear(); }
      LCD1602.setCursor(0, 0);  
      LCD1602.print(F("Humedad: ")); 
      LCD1602.print(float(humedad_suelo), 0);  // ojo, la libreria del lcd solo entinde float!!  :(
      LCD1602.print(F("%")); 
      LCD1602.setCursor(0, 1); 
      refrescaReloj(&contadorHoras, &contadorMinutos, &contadorSegundos);
      break; 
      
  case COD_INFO_FIN_RIEGO: 
      LCD1602.clear();
      LCD1602.print(F("Humedad OPTIMA "));
      LCD1602.setCursor(0, 1); 
      LCD1602.print(F("Riego TERMINADO"));
      break;   
          
  case ERROR_TIEMPO_MAXIMO:
      LCD1602.clear();
      LCD1602.print(F("RIEGO OFF "));
      LCD1602.setCursor(0, 1); 
      LCD1602.print(F("LIMITE T. MAX"));
      break; 
      
  case ERROR_MOTOR:
      LCD1602.clear();
      LCD1602.print(F(" Error Motor "));
      break; 
               
  case ERROR_SENSOR_H_SUELO:
      LCD1602.clear();
      LCD1602.print(F(" Error s.Humedad"));
      break; 
      
  case ERROR_SENSOR_LUZ:
      LCD1602.clear();
      LCD1602.print(F(" Error s.Luz "));
      break; 
      
  default:
      LCD1602.clear();
      LCD1602.print(F("ERROR DESCONOCIDO"));
      LCD1602.setCursor(0, 1); 
      LCD1602.print(F("STOP x SEGURIDAD"));
      break;
  }
}


 
//========================================================
//  SIN USO
//========================================================

void refrescaCuentaAtras( uint8_t *horas, uint8_t *minutos , uint8_t *segundos )
{
    //mostrar tiempo restante de riego
     LCD1602.setCursor(8, 1); 
    if( *horas<10 ) { LCD1602.print(F("0")); }
    LCD1602.print(*horas);
    LCD1602.print(F(":"));
    if( *minutos<10 ) { LCD1602.print(F("0")); }
    LCD1602.print(*minutos);
    LCD1602.print(F(":"));
    if( *segundos<10 ) { LCD1602.print(F("0")); }
    LCD1602.print(*segundos);
    LCD1602.print(F(" "));
}


//========================================================
//  REFRESCAR Y MOSTRAR RELOJ
//========================================================

void refrescaReloj( uint8_t *horas, uint8_t *minutos , uint8_t *segundos )
{
  LCD1602.setCursor(8, 1);  //linea de abajo
  
  if( *horas<10 ) { LCD1602.print(F("0")); }
  LCD1602.print(*horas);
  
  LCD1602.print(F(":"));
  if( *minutos<10 ) { LCD1602.print(F("0")); }
  LCD1602.print(*minutos);
  
  LCD1602.print(F(":"));
  if( *segundos<10 ) { LCD1602.print(F("0")); }
  LCD1602.print(*segundos);
  LCD1602.print(F("  "));
}


//========================================================
//  REFRESCAR Y MOSTAR ANIMACION  "RUN >>>"
//========================================================

void animacionRUN(uint8_t _contadorAnimacion)
{
  LCD1602.setCursor(0, 1);  //linea de abajo
  switch (_contadorAnimacion)   //XTAL_SEGUNDOS
    {
      case 0:
        LCD1602.print(F("RUN "));  
        LCD1602.write(0);
        LCD1602.print(F("   ")); 
        break;
      case 1:
        LCD1602.print(F("RUN "));  
        LCD1602.write(0);
        LCD1602.write(0);
        LCD1602.print(F("  "));
        break;
      case 2:
        LCD1602.print(F("RUN "));  
        LCD1602.write(0);
        LCD1602.write(0);
        LCD1602.write(0);
        LCD1602.print(F(" "));
        break;
      case 3:
        LCD1602.print(F("RUN     "));
        break;
    }  
}


//========================================================
//  SIN USO
//========================================================

void animacionRUN_original()
{
  LCD1602.setCursor(0, 1);  //linea de abajo
  switch (contadorMedioSegundo%4)   //XTAL_SEGUNDOS
    {
      case 0:
        LCD1602.print(F("RUN >   ")); 
        break;
      case 1:
        LCD1602.print(F("RUN >>  "));
        break;
      case 2:
        LCD1602.print(F("RUN >>> "));
        break;
      case 3:
        LCD1602.print(F("RUN     "));
        break;
    }  
}
