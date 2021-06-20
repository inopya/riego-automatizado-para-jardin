
/*
   ================================================================================================== 
                                             DEFINICIONES
   ================================================================================================== 

*/


#define __VERSION__ "Riego de jardin (demo) v1.0\n"

/*
 * DESCRIPCION DEL PROYECTO
  Riego automatico a escala maceta o pequeño jardin
  Practicando arduino. Sensores, toma de decisiones, control.
  
*/

//he aqui una manera  facil y comoda de activar y desactivar mensajes por puerto serial
//siemrpe que no estemos usando serial para comunicarnos con otros dispositivos, 
//en ese caso podemos dejar esos mensajes como "Serial.printXX" normales
//y usar solo nuestras macros para los mensajes de DEBUG.

/* permitir funciones serial (modo DEBUG) */
#define SERIAL_BEGIN  Serial.begin
#define PRINTLN  Serial.println
#define PRINT  Serial.print

/* Anular funciones serial para ahorrar memoria y dar velocidad (uso normal) */
//#define SERIAL_BEGIN  //Serial.begin
//#define PRINTLN  //Serial.println
//#define PRINT  //Serial.print




/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        ALGUNAS DEFINICIONES PERSONALES PARA MI COMODIDAD AL ESCRIBIR CODIGO
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

#define AND    &&
#define OR     ||
#define NOT     !
#define ANDbit  &
#define ORbit   |
#define XORbit  ^
#define NOTbit  ~

#define getBit(data,y)       ((data>>y) & 1)           // Obtener el valor  del bit (data.y)
#define setBit(data,y)       data |= (1 << y)          // Poner a 1 el bit (data.y) 
#define clearBit(data,y)     data &= ~(1 << y)         // Poner a 0 el bit (data.y)
#define togleBit(data,y)     data ^= (1 << y)          // Invertir el valor del bit (data.y)
#define togleByte(data)      data = ~data              // Invertir el valor del byte (data)


/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        SECCION DE DEFINICION DE CONSTANTES DEL PROGRAMA
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

#define ON                    1     // 
#define OFF                   0     // 
#define LIMITE_ADC         1023     // por comodidad, por ejemplo si usamos algunos clones de Arduino  
                                    // como los WAVGAT UNO, cuyo ADC tiene una resolucion de 12 bit, (4096) 


#define ERROR_ntc             1.04  // valor que se multiplica por la lectura nuestra NTC (valor empirico)



        
