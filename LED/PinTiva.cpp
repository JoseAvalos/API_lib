#include "Energia.h"

PIN::PIN (int port)
{
	pinMode(port, OUTPUT);
	_port= port;
};

int PIN::on()
{
	digitalWrite(_port, HIGH)
}

int PIN::off()
{
  digitalWrite(_port, LOW)
}