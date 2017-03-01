#include <PinTiva.h>

void setup() 
{

  PIN LED1(PN_1);
  PIN LED2(PN_0);
  
  while(1)
  {
    LED2.off();
    LED1.on();
    delay(500);
    LED2.on();
    LED1.off();
    delay(500);
  }
}

void loop() {}
