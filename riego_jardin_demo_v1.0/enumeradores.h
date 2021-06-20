
/*
   ================================================================================================== 
                                           ENUMERADORES
   ================================================================================================== 

   De este fichero dependen:
   --  xxxxx
*/


/* ENUM TIPOS DE PULSACION */
//Si usamos la libreria inopya para pulsadores (pudiendo distinguir pulsaciones simples, largas, dobles o mantenidas)
enum tipoPulsacion 
{
  PULSAC_NULA       =  0,   // estos valores son los codigos que devuelve la libreria, no cambiar
  PULSAC_CORTA      =  1,   // 
  PULSAC_DOBLE      =  2,   // 
  PULSAC_LARGA      =  3,   // 
  PULSAC_MANTENIDA  =  9,   // necesitamos un tiempo mayor a 5*PULSAC_LARGA para que se detecte como mantenida
};


/* ENUM OPERATION MODE */
enum  modo_operacion_timer
{
  OP_MODE_TIMER   = 0 ,
  OP_MODE_CLOCK   = 1 ,
};



/* ENUM ESTADOS MAQUINA */
enum estados_maquina
{
  MQ_REPOSO             ,   //  estado normal, mostrando estado de variables y a la espera de condiciones de riego    
  MQ_RIEGO_START        ,   //  se activan condiciones de riego
  MQ_RIEGO_ON           ,   //  en ejecucion de la tarea de riego
  MQ_NORMAL_STOP        ,   //  fin de una operacion de riego sin incidencias
  MQ_EMERGENCY_STOP     ,   //  interrupcion de un operacion de riego por algun problema
};



/* ENUM SELECCION DE SENSOR */
enum seleccionSensor
{
  SENSOR_LUZ_SOLAR       ,   // LDR para el control de luz solar
  SENSOR_H_SUELO         ,   // higrometro de suelo
  SENSOR_T_SUELO         ,   // NTC para el cotnrol de temperatura de suelo
  SENSOR_HT_AMBIENTE     ,   // DHT11cpara el control de temperatura y humedad del aire 
  SENSOR_BATERIA         ,   // control interno de la bateria mediante registros del uC
  SENSOR_ALL             ,   // activar la lectura de todos los sensores en bloque
};


/* ENUM UMBRALES y LIMITES UTILES */
enum umbral_y_limites
{
  HUM_S_MINIMA      =   55 ,  // umbral de humedad minima de suelo (para empezar un riego automatico)
  HUM_S_MAXIMA      =   90 ,  // umbral de humedad de suelo a la que terminar un riego automatico
  TEMP_A_MINIMA     =    5 ,  // no regar por debajo de esta temperatura ambiente
  TEMP_A_MAXIMA     =   35 ,  // no regar por encima de esta temperatura
  LUZ_SOLAR_MINIMA  =    0 ,
  LUZ_SOLAR_MAXIMA  =   50 ,  // no regar si luz solar superior a este umbral (para evitar hacerlo a horas de mucho sol)

  LIMITE_TIEMPO_RIEGO =  1 ,  // tiempo maximo de riego (tiempo en minutos)
  UPDATE_INFO_TIME    =  1 ,  // tiempo para ctualizar variables del cultivo (tiempo en minutos)
  PAUSA_MENSAJES      =  4 ,  // tiempo para mostras mensajes como bienvenida, inicio riego, fin, etc (segundos)
  TIME_OUT_LCD        = 10 ,  // tiempo que dura encendida la luz del LCD tras manipular botones (en segundos)
  
};



/* ENUM CODIGOS ERROR */
enum info_error_codes
{
  COD_INFO_BIENVENIDA     ,
  COD_INFO_AMBIENTE       ,
  COD_INFO_SUELO          ,
  COD_INFO_INICIO_RIEGO   ,
  COD_INFO_RIEGO          ,
  COD_INFO_FIN_RIEGO      ,
  ERROR_TIEMPO_MAXIMO     ,  // se ha excedido tiempo maximo de riego
  ERROR_MOTOR             ,  // sin uso
  ERROR_SENSOR_H_SUELO    ,  // sin uso
  ERROR_SENSOR_LUZ        ,  // sin uso
};


  
