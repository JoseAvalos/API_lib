#ifndef PinTiva_h
#define PinTiva_h

#include "Energia.h"

//!  Clase PIN . 
/*!
  Esta clase nos permite emplear los pines del uC TIVA con mayor flexivilidad.
*/
class PIN
{
  public:
    
    //! Constructor PIN.
      /*!
        \param puerto se emplea la denominacion dada por el uC.
      */
    PIN(int puerto);

    //! Funcion que permite encender digitalmente el pin. 
      /*!
        \return true si se realizo la configuracion, false si no se realizo.
      */
    int on();
    
    //! Funcion que permite apagar digitalmente el pin.
      /*!
        \return true si se realizo la configuracion, false si no se realizo.
      */
    int off();
  
  private:  
    int _port;
};

#endif
