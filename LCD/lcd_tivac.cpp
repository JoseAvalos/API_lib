#include "Energia.h"
#include <Wire.h>
#include <lcd_tivac.h>
#include <AD9854.h>
#include "Tiva.h"
#include <ArduinoHttpServer.h>


LCD::LCD(DDS* _DDS, API* _API )
{
  _DDS_JRO=_DDS;
  _API_JRO=_API;


}

void LCD::init(byte addr)
{
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
}

void LCD::text(char input[]){//, int n) {
  int n = strlen(input)-1;
  if (n>19)
    n=19;
  
  uint8_t addr = 0x3c;
  for (int i = 0; i <= n; i++)
  {
    writing(addr, 0x40, input[i]);//(write on DDRAM command, ASCII code for character)
  }
}

void LCD::put_num(byte number)
{
  byte addr = 0x3c;
  byte uni=0x00, dec=0x30, cen=0x30;
  byte temp=number;
  while(temp >= 100)
  {
    temp-=100;
    cen=cen+0x01;
  }
  while(temp >= 10)
  {
    temp-=10;
    dec=dec+0x01;
  }
  uni = 0x30+temp;
  if(cen>0x30) writing(addr, 0x40, cen);
  if(dec>0x30) writing(addr, 0x40, dec);
  else if((dec==0x30)&(cen>0x30)) writing(addr, 0x40, dec);
  writing(addr, 0x40, uni);
}

void LCD::writing(byte my_addr, byte cmd, byte data) {
  Wire.beginTransmission(my_addr);
  Wire.write(cmd);
  Wire.write(data);
  Wire.write(data);
  Wire.write(data);
  Wire.endTransmission(true);
  delayMicroseconds(15);
}

void LCD::position(int row, int col){
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

void LCD::clear()
{
  position(1,0);
  text("                    ");
  position(2,0);
  text("                    ");
}


void LCD::showinformation()
{
  String f1=String(_DDS_JRO->binary2freq(_DDS_JRO->rdFrequency1())*_DDS_JRO->getMultiplier());
  String f2=String(_DDS_JRO->binary2freq(_DDS_JRO->rdFrequency2())*_DDS_JRO->getMultiplier());
  clear();
  
  f1="Freq1: "+f1;
  f2="Freq1: "+f2;

  char freq1[32];
  strcpy(freq1, f1.c_str());

  char freq2[32];
  strcpy(freq2, f1.c_str());

  position(1,0);
  text(freq1);
  position(2,0);
  text(freq2);

  delay(100);

  IPAddress lcd_ip = _API_JRO->_set_ip;
  IPAddress lcd_gateway = _API_JRO->_set_gateway;

  String ip= "IP:"+String(lcd_ip[0])+"."+String(lcd_ip[1])+"."+String(lcd_ip[2])+"."+String(lcd_ip[3]);
  String gateway = "IP:"+String(lcd_gateway[0])+"."+String(lcd_gateway [1])+"."+String(lcd_gateway [2])+"."+String(lcd_gateway [3]);

  char cad_ip[32];
  strcpy(cad_ip, ip.c_str());

  char cad_gateway[32];
  strcpy(cad_gateway, gateway.c_str());

  position(1,0);
  text(cad_ip);
  position(2,0);
  text(cad_gateway);

}