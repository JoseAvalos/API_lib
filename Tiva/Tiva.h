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
  	IPAddress _new_ip;
	IPAddress _new_dns;
	IPAddress _new_gateway;
	IPAddress _new_subnet;
  private:  
  	DDS* _DDS_JRO;
  	
};

#endif
