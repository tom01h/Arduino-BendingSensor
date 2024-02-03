#include <U8g2lib.h>

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R2, /* reset=*/ U8X8_PIN_NONE);

int8_t zero_init = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(5, OUTPUT);
  pinMode(4, INPUT);
  pinMode(3, INPUT);
  pinMode(2, INPUT);

  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0,  7, "Hello world!");
  u8g2.drawStr(0, 17, "OLED screen TEST");
  u8g2.drawStr(0, 27, "with U8g2lib");
  u8g2.sendBuffer();

  zero_init = 10;
}

int bend_base[3] = {0};

void loop() {
  // Read HX711
  int atai[3] = {0};

  for (int i = 0; i < 24; i++) {
    digitalWrite(5, HIGH);
    __ASM volatile ("NOP");
    digitalWrite(5, LOW);
    __ASM volatile ("NOP");
    atai[0] = (atai[0] << 1) | (digitalRead(4));
    atai[1] = (atai[1] << 1) | (digitalRead(3));
    atai[2] = (atai[2] << 1) | (digitalRead(2));
  }
  digitalWrite(5, HIGH);
  __ASM volatile ("NOP");
  digitalWrite(5, LOW);
  __ASM volatile ("NOP");

  for(int i=0; i<3; i++){
    if(atai[i]>=(1<<23)){
      atai[i] -= (1<<24);
    }
  }

  if(zero_init>0){
    zero_init--;
    for(int i=0; i<3; i++){
      bend_base[i] += atai[i];
      if(zero_init==0){
        bend_base[i] /= 10;
      }
    }
    delay(100);
    return;
  }

  // put your main code here, to run repeatedly:
  Serial.println("hello world");
  Serial.println(atai[0]);
  Serial.println(atai[1]);
  Serial.println(atai[2]);
  
  for(int i=0; i<3; i++){
    atai[i] -= bend_base[i];
    atai[i] /= -100;
  }

  char s[10];
  u8g2.clearBuffer();
  itoa(atai[0],s,10);
  u8g2.drawStr(0,  7, s);
  itoa(atai[1],s,10);
  u8g2.drawStr(0, 17, s);
  itoa(atai[2],s,10);
  u8g2.drawStr(0, 27, s);
  u8g2.sendBuffer();
  
  delay(100);                      // wait for a second

}
