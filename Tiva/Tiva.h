#ifndef Tiva_h
#define Tiva_h

#include "Energia.h"
#include <ArduinoHttpServer.h>
#include <Ethernet.h>
#include <AD9854.h>

class API 
{
  public:
  	API(DDS*,IPAddress, byte[]);
  	int readcommand(EthernetClient);
  	String status_DDS();
  	int validation(double, int , double , double , float , float , float , float , int);
  	IPAddress _set_ip;
	IPAddress _set_dns;
	IPAddress _set_gateway;
	IPAddress _set_subnet;
  private:  
  	DDS* _DDS_JRO;
  	
};

#endif
