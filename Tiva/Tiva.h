#ifndef Tiva_h
#define Tiva_h

#include "Energia.h"
#include <ArduinoHttpServer.h>
#include <Ethernet.h>
#include <AD9854.h>


//!  Clase API . 
/*!
  Esta clase contiene las funciones que permitan usar la interface con el ethernet empleando un mensaje JSON.
*/
class API 
{
  public:
  //! Constructor API.
      /*!
        \param DDS incluye el puntero de la variable externa del DDS.
        \param IP  constituye el valor inicial de la IP para el API.
        \param mac corresponde al dato del uC TIVA.
      */
  API(DDS* DDS,IPAddress IP , byte[] mac);

   //! Esta funcion lee el cabezal del mensaje JSON: START, STOP, WRITE y otros para realiazar su funcion correspondinte.
      /*!
        \param client incluye la variable etherneth.
        \return valor de que caso se dio.
      */
  int readcommand(EthernetClient client);

  //! Esta funcion se encarga de generar el mensaje por default para todas las solicitudes JSON.
      /*!
        \return String con el mensaje.
      */
  String status_DDS();

  //! Esta funcion busca determinar si al usar WRITE los valores insertados son los correctos.
      /*!
        \return true si se realizo la configuracion, false si no se realizo.
      */
  int validation(double _vclock, int _vmultiplier, double _vfrequency1, double _vfrequency2, float _vamplitudeI, float _vamplitudeQ, float _vphaseA, float _vphaseB, int _vmode);
  
  //! Contiene el valor IP actual.
      /*!
      */
  IPAddress _set_ip;

  //! Contiene el valor DNS actual.
      /*!
      */
	IPAddress _set_dns;
	
  //! Contiene el valor Gateway actual.
      /*!
      */
  IPAddress _set_gateway;
	
  //! Contiene el valor Subnet actual.
      /*!
      */
  IPAddress _set_subnet;
  
  private:  
  
  DDS* _DDS_JRO;
  	
};

#endif
