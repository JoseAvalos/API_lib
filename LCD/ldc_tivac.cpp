#include <Arduino.h>
#include <Wire.h>
#include <lcd_tivac.h>
//uint8_t addr = 0x3c;
lcd_tivac::lcd_tivac(void){
}
void lcd_tivac::start(byte addr){
  int int_delay = 40;
  writing(addr, 0x00, 0x00);//..
  delay(int_delay);
  writing(addr, 0x00, 0x38);//FUNCTION SET
  delay(int_delay);
  writing(addr, 0x00, 0x39);//FUNCTION SET
  delay(int_delay);
  writing(addr, 0x00, 0x14);//BIAS
  delay(int_delay);
  writing(addr, 0x00, 0x78);//CONTRAST SET
  delay(int_delay);
  writing(addr, 0x00, 0x5e);//POWER/ICON/CONTRAST CONTROL
  delay(int_delay);
  writing(addr, 0x00, 0x6d);//FOLLOWER CONTROL
  delay(299);//DELAY FOR POWER STABILIZATION
  delay(int_delay);
  writing(addr, 0x00, 0x0c);//DISPLAY ON
  delay(int_delay);
  writing(addr, 0x00, 0x01);//CLEAR DISPLAY
  delay(int_delay);
  writing(addr, 0x00, 0x06);//ENTRY MODE SET
  delay(int_delay);
  writing(addr, 0x00, 0x02);//Set cursor on 0,0
  delay(int_delay);
  //Serial.println("lcd_tivac ready");
}
void lcd_tivac::text(char input[]){//, int n) {
  int n = strlen(input)-1;
  uint8_t addr = 0x3c;
  for (int i = 0; i <= n; i++)
  {
    writing(addr, 0x40, input[i]);//(write on DDRAM command, ASCII code for character)
  }
}

void lcd_tivac::put_num(byte number)
{
  byte addr = 0x3c;
  byte uni=0x00, dec=0x30, cen=0x30;
  byte temp=number;
  while(temp >= 100){
    temp-=100;
    cen=cen+0x01;
  }

  //Serial.println(cen, HEX);
  //Serial.println("cen");
  while(temp >= 10){
    temp-=10;
    dec=dec+0x01;
  }
  //Serial.println(temp);
  //Serial.println(dec, HEX);
  //Serial.println("dec");
  uni = 0x30+temp;
  //Serial.println(uni, HEX);
  //Serial.println("uni");
//  text((char*)cen,1);
  if(cen>0x30) writing(addr, 0x40, cen);
//  text((char*)dec,1);
  if(dec>0x30) writing(addr, 0x40, dec);
  else if((dec==0x30)&(cen>0x30)) writing(addr, 0x40, dec);
//  text((char*)uni,1);
  writing(addr, 0x40, uni);
}

void lcd_tivac::writing(byte my_addr, byte cmd, byte data) {
  Wire.beginTransmission(my_addr);
  Wire.write(cmd);
  Wire.write(data);
  Wire.write(data);
  Wire.write(data);
  //Wire.write(data);
  Wire.endTransmission(true);
  delayMicroseconds(15);
}

void lcd_tivac::position(int row, int col){
  byte addr = 0x3c;
  byte a = 0x00;
  if (col < 20){
  if (row == 1)
    a = 0x00+col;
  else if (row == 2)
    a = 0x40+col;
  writing(addr, 0x00, 0x80|a);
  }
}
