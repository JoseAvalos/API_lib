#ifndef lcd_tivac_h
#define lcd_tivac_h

#include "Arduino.h"
#include <Wire.h>
#include <AD9854.h>
#include "Tiva.h"

//!  Clase LCD. 
/*!
  Esta clase contiene las funciones que permitan usar la interface LCD con la informacion del DDS.
*/

class LCD
{
  public:
  //! Constructor LCD.
      /*!
        \param _LCD_DDS incluye el puntero de la variable externa del DDS.
        \param _LCD_API incluye el puntero de la variable externa del API.
      */
  LCD(DDS* _LCD_DDS, API* _LCD_API);

  //! Función para iniciar la comunicacion I2C, estaba se debe ubicar en main.
      /*!
        \param my_addr_lcd Para el LCD JRO 0x30.
      */
  void init(uint8_t my_addr_lcd);

  //! Función entrada de texto para el LCD.
      /*!
        \param input[] incluye el mensaje a reproducir como "Hola mundo".
      */
  void text(char input[]);

  //! Función entrada de numeral byte para el LCD.
    /*!
      \param number incluye el valor HEX a reproducir en el LCD.
    */
  void put_num(byte number);

  //! Función general de comunicacion I2C con el LCD.
    /*!
      \param my_addr direccion del LCD.
      \param  cmd funcion del LCD.
      \param  data informacion a enviar.
    */
  void writing(byte my_addr, byte cmd, byte data);

  //! Función entrada para colocar la posicion del LCD.
    /*!
      \param row 1, 2.
      \param col 0-19
    */
  void position(int row, int col);

  //! Función para limpiar la pantalla LCD.
  void clear();

  //! Función que reproduce la informacion en el LCD.
  void showinformation();

private: 

    DDS* _DDS_JRO;
    API* _API_JRO;
};

#endif
