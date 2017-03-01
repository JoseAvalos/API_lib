#ifndef PinTiva_h
#define PinTiva_h

#include "Energia.h"

class PIN
{
  public:
    PIN(int );
    int on();
    int off();
  private:  
    int _port;
};

#endif
