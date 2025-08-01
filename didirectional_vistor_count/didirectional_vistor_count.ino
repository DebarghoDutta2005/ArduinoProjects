#include <LedControl.h>
int DIN=11;
int CS=12;
int CLK=13;
byte one[8]= {0x00,0x00,0x02,0x81,0xFF,0x80,0x00,0x00};
byte two[8]={0x00,0xC6,0xA1,0x91,0x89,0x86,0x00,0x00};
byte three[8]={0x00,0x42,0x81,0x89,0x89,0x76,0x00,0x00};
byte four[8]={0x18,0x14,0x12,0x11,0xFF,0x10,0x10,0x00};
byte five[8]={0x00,0x00,0x8F,0x89,0x89,0xF9,0x00,0x00};
byte six[8]={0x00,0x00,0xFF,0x91,0x91,0xF1,0x00,0x00};
byte seven[8]={0x00,0x00,0x01,0x01,0x01,0xFF,0x00,0x00};
byte eight[8]={0x00,0xFF,0x91,0x91,0x91,0xFF,0x00,0x00};
byte nine[8]={0x00,0x00,0x0F,0x09,0x09,0xFF,0x00,0x00};
byte ten[8]={0x02,0x81,0xFF,0x80,0x00,0x7E,0x81,0x7E};
byte zero[8]={0x00,0x7E,0x81,0x81,0x81,0x7E,0x00,0x00};
LedControl lc=LedControl(DIN,CLK,CS,0);
int ir_1=7;
int ir_2=5;
int relay=10;
int x;
int y;
int count=0;
void setup() {
  Serial.begin(9600);
  pinMode(7,INPUT); // for ir_1
  pinMode (5,INPUT); // for ir_2
  pinMode(10,OUTPUT); // for relay
  lc.shutdown(0,false);
  lc.setIntensity(0,2);
  lc.clearDisplay(0);
  delay(300);
}

void loop() 
{
  x=digitalRead(ir_1); // in(left)
  y=digitalRead(ir_2); // out(right)
  Serial.println(count);
   delay (1000); 
  lc.clearDisplay(0);
   
  if (x==1){
    count++;
  }
  if (y==1) {
    count--;
  }

  if (count>=1){
    
    if (count==1){
      printByte(one);
    }
     if (count==2){
      printByte(two);
     }
     if (count==3){
      printByte(three);
     }
     if (count==4){
      printByte(four);
     }
     if (count==5){
      printByte(five);
     }
     if (count==6){
      printByte(six);
     }
     if (count==7){
        printByte(seven);
     }
     if (count==8){
      printByte(eight);
     }
     if (count==9){
      printByte(nine);
     }
     if (count==10){
       printByte(ten);
     }
   
  
    digitalWrite (relay,LOW);
  }
  if (count==0 ){
      printByte(zero);
    digitalWrite (relay,HIGH);
  }
  
  
}
void printByte(byte character [])
  {
    int i = 0;
   for(i=0;i<8;i++)
    {
    lc.setRow(0,i,character[i]);

    }
  }
