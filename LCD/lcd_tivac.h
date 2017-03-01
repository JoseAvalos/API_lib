#ifndef lcd_tivac_h
#define lcd_tivac_h
//libreria wire para conexión con i2c
#include "Arduino.h"
#include <Wire.h>
#include <AD9854.h>
#include "Tiva.h"
//declacarión de clase lcd_tivac para instanciarlo desde el código
class LCD
{
public:
  ///////////////// INICIO ///////////////////////
  /*En el código fuente incluir la línea (sin las comillas)
      " lcd_tivac lcd_tivac; "
  */
  ///////////////// START ///////////////////////
  /*En el código fuente incluir la línea (sin las comillas)
      " lcd_tivac.start(direccion_i2c); "
  */
  LCD(DDS*, API*);
  void init(uint8_t);
  ///////////////// TEXTO ///////////////////////
  /*función entrada de texto del tipo char
    Ejemplos:
    lcd_tivac.text("Hola mundo");
  */
  void text(char input[]);


  ///////////////// NUMEROS 0 - 255 ///////////////////////
  /*funcion para colocar números de variables del tipo byte y uint8
    Ejemplos:
    lcd_tivac.put_num(0x01); -> imprime el número 1 en la pantalla en la posición x,y
  */
  void put_num(byte);


  ///////////////// COMUNICACION I2C ///////////////////////
  /*función para escribir a través de i2c en los registros de cualquier dispositivo i2c
    requisitos:
      1) definir la dirección del dispositivo
      2) comando para indicar escritura o lectura de un registro de acuerdo a la configuración por defecto del dispositivo
      3) data a enviar, puede ser el registro en el cual se escribira/leera o la data a escribir
  */
  void writing(byte, byte, byte);


  ///////////////// POSICIÓN DE CURSOR(por defecto 0,0) ///////////////////////
  /*función para definir la posición del cursor, donde:
    row = 1, 2
    col = 0 - 19

    Ejemplo:
      lcd_tivac.position(1,0); sonfigura el cursos en la esquina superior izquierda (primera celda de caracter)
      lcd_tivac.position(2,19); configura el cursor en la esquina inferior derecha (ultima celda de caracter)
  */
  void position(int, int);
  void clear();

  void showinformation();

private: 

    DDS* _DDS_JRO;
    API* _API_JRO;
};

#endif
