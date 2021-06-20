
/*
   ================================================================================================== 
                                             MAPA DE PINES 
   ================================================================================================== 


*/



#define PIN_sensorNTC           A1    // Como sensor "casero" de temperatura de suelo (mejor usar sonda DS18B20)
#define PIN_HUMEDAD_SUELO       A0    // higrometro de suelo FC-28    
#define PIN_SENSOR_LUZ          A2    // LDR como sensor "casero" de luz solar 


//#define PIN_SDA                 A4    // reservada para el I2C
//#define PIN_SCL                 A5    // reservada para el I2C 

#define PIN_ENCODER_CLK          2
#define PIN_ENCODER_DT           3
#define PIN_ENCODER_SW           4  
 

#define PIN_DHT                  7 
#define PIN_BOMBA                8   // salida a rele o transistor para controlar la bomba de riego
#define PIN_TIRA_LED             9   // patilla para la tira deld RGB  para luces de estado (sin uso)
#define PIN_DS18B20             11   // pin de la sonda de temperatura ds18b20

#define PIN_LED_OnBoard         13   // Led on Board



#define PUERTO_ENCODER        PIND   //puerto de lectura del encoder
#define BIT_ENCODER_CLK          2
#define BIT_ENCODER_DT           3 
#define BIT_ENCODER_SW           4  
