/////// ผนวกไลบรารี่ ////////////
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
Adafruit_SSD1306 OLED(-1);

#include <Adafruit_MCP3008.h>
Adafruit_MCP3008 adc,adc2;

///////////ตั้งค่าปุ่มกด///////////////////
uint8_t button =  2; /// กำหนดปุ่มกดสวิตซ์ขา 2
uint8_t ss1 = 14;  // เปิดใช้งาน s1 ที่พอร์ต 10
uint8_t ss2 = 15;
Servo sv1;     // ประกาศการใช้งาน sv1
Servo sv2;
uint16_t angle_ss1 = 90;
uint16_t angle_ss2 = 90;

//////////////////////////////////////////////

////////////ตั้งค่าพอร์ตมอเตอร์////////////////////
#define DR1  7  /// กำหนดสัญญาณดิจิตอลขวาที่ 1 พอร์ต 7
#define DR2  8  /// กำหนดสัญญาณดิจิตอลขวาที่ 2 พอร์ต 8
#define PWMR 6 /// กำหนดสัญญาณ PWM ขวาพอร์ต 6

/////////////////////////////////
#define DL1  4  // กำหนดสัญญาณดิจิตอลซ้ายที่ 1 พอร์ต 9
#define DL2  9  // กำหนดสัญญาณดิจิตอลซ้ายที่ 2 พอร์ต 4
#define PWML 5  /// กำหนดสัญญาณ PWM ซ้ายพอร์ต 5

/////////////////ตั้งค่าพอร์ต Analog//////////////
#define a1 500 // ค่ากลางเซนเซอร์ A0 (ซ้ายสุด)
#define a2 500// ค่ากลางเซนเซอร์ A1 (ซ้ายกลาง)
#define a3 500 // ค่ากลางเซนเซอร์ A2 (ขวากลาง)
#define a4 500// ค่ากลางเซนเซอร์ A3 (ขวาสุด)
#define a5 500 // ค่ากลางเซนเซอร์ A0 (ซ้ายสุด)
#define a6 500// ค่ากลางเซนเซอร์ A1 (ซ้ายกลาง)
#define a7 500 // ค่ากลางเซนเซอร์ A2 (ขวากลาง)
#define a8 500// ค่ากลางเซนเซอร์ A3 (ขวาสุด)

#define b1 500// ค่ากลางเซนเซอร์ A1 (ซ้ายกลาง)
#define b2 500 // ค่ากลางเซนเซอร์ A2 (ขวากลาง)
#define b3 500// ค่ากลางเซนเซอร์ A3 (ขวาสุด)
#define b4 500 // ค่ากลางเซนเซอร์ A0 (ซ้ายสุด)
#define b5 500// ค่ากลางเซนเซอร์ A1 (ซ้ายกลาง)
#define b6 500 // ค่ากลางเซนเซอร์ A2 (ขวากลาง)
#define b7 500// ค่ากลางเซนเซอร์ A3 (ขวาสุด)
#define b8 500// ค่ากลางเซนเซอร์ A3 (ขวาสุด)

///////////////////////////////////////////////
uint16_t s1,s2,s3,s4,s5,s6,s7,s8 ;
uint16_t sb1,sb2,sb3,sb4,sb5,sb6,sb7,sb8 ;


void analogs()
{
  s1 = adc.readADC(0);
  s2 = adc.readADC(1);
  s3 = adc.readADC(2);
  s4 = adc.readADC(3);
  s5 = adc.readADC(4);
  s6 = adc.readADC(5);
  s7 = adc.readADC(6);
  s8 = adc.readADC(7);
}

void analogs_2(){
  sb1 = adc2.readADC(0);
  sb2 = adc2.readADC(1);
  sb3 = adc2.readADC(2);
  sb4 = adc2.readADC(3);
  sb5 = adc2.readADC(4);
  sb6 = adc2.readADC(5);
  sb7 = adc2.readADC(6);
  sb8 = adc2.readADC(7);
}

//////////////////////////////////////////////

void setup() {
  adc.begin(10);
  adc2.begin(3);
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C); // กำหนดแอดเดรสของพอร์ตจอเป็น 0x3C (for the 128x64)
  pinMode(2, INPUT); // ตั้งค่าขา 2 เป็น INPUT
  ////////กำหนดสัญญาณทุกขาเป็น OUTPUT /////////
  pinMode(DL1, OUTPUT);
  pinMode(DL2, OUTPUT);
  pinMode(PWML, OUTPUT);
  pinMode(DR1, OUTPUT);
  pinMode(DR2, OUTPUT);
  pinMode(PWMR, OUTPUT);
  sv1.attach(ss1); // เรียกใช้งานเตรียมตอบสนองเซอร์โว sv1
  sv2.attach(ss2); // เรียกใช้งานเตรียมตอบสนองเซอร์โว sv1
  sv_set(90);
}

void loop() {
  bool sw = digitalRead(button);     // ให้ sw อ่านค่า digital จากพอร์ต 2(button)
  uint16_t nob = analogRead(7);          // ให้ nob เทียบเท่าค่า A7
  uint16_t menu = map(nob, 0, 1023, 0, 9); // เทียบอัตราส่วนของพอร์ต A7 จาก 0-1023 เพื่อทำเป็นเมนู 0-12
  OLED.clearDisplay();              // ล้างค่าหน้าจอ
  OLED.setTextColor(WHITE, BLACK);  //สีอักษรเป็นสีขาว ,พื้นหลังดำ
  OLED.setCursor(0, 0);       // เซตตำแหน่ง 0,0
  OLED.setTextSize(2);        // เซตขนาดอักษรมีขนาดเป็น 2
  OLED.print(F("   "));         // วรรค
  OLED.println(menu);        // แสดงค่า Menu ที่ได้จากการ map nob ให้เหลือ 0-12
  OLED.setTextSize(1);        // เซตขนาดอักษรมีขนาดเป็น 2
  OLED.println(F("  KruRo"));     // พิมพ์คำว่า kruro
  OLED.print(F("  "));                      // วรรค
  OLED.print(nob);                     // แสดงค่าที่อ่านได้จาก nob หรือ Analog7
  OLED.println(F(" Robot"));     // พิมพ์คำว่า Robot
  OLED.display();
  #define mode 1
  if ((sw == mode) and (menu == 0)){sensor();}
  if ((sw == mode) and (menu == 1)){sv_knob();}
  if ((sw == mode) and (menu == 2)){menu2();}
  if ((sw == mode) and (menu == 3)){menu3();}
  if ((sw == mode) and (menu == 4)){menu4();}
  if ((sw == mode) and (menu == 5)){menu5();}
  if ((sw == mode) and (menu == 6)){menu6();}
  if ((sw == mode) and (menu == 7)){menu7();}
  if ((sw == mode) and (menu == 8)){menu8();}
  if ((sw == mode) and (menu == 9)){menu9();}
  delay(100);
}

void sensor(){
  while (true) {
    analogs();
    analogs_2();
    uint16_t vr = analogRead(A7);  // กำหนดตัวแปรจำนวนเต็มอ่านค่าอนาล็อกที่พอร์ต 7
    uint16_t nob = map(vr, 0, 1023, 0, -180); // ทำการ map อัตราส่วนจากสัญญาณ analog 0-1023 เป็น 0-180
    OLED.clearDisplay();
    OLED.setTextColor(WHITE, BLACK);  //สีอักษรเป็นสีขาว ,พื้นหลังดำ
    OLED.setCursor(0, nob);       // เซตตำแหน่ง 0,0
    OLED.setTextSize(1);        // เซตขนาดอักษรมีขนาดเป็น 1
    OLED.print(F("       a1 = ")); OLED.println(s1);  // แสดงค่าเซนเซอร์ S0
    OLED.print(F("       a2 = ")); OLED.println(s2);  // แสดงค่าเซนเซอร์ S1
    OLED.print(F("       a3 = ")); OLED.println(s3);  // แสดงค่าเซนเซอร์ S2
    OLED.print(F("       a4 = ")); OLED.println(s4);  // แสดงค่าเซนเซอร์ S3
    OLED.print(F("       a5 = ")); OLED.println(s5);  // แสดงค่าเซนเซอร์ S0
    OLED.print(F("       a6 = ")); OLED.println(s6);  // แสดงค่าเซนเซอร์ S1
    OLED.print(F("       a7 = ")); OLED.println(s7);  // แสดงค่าเซนเซอร์ S2
    OLED.print(F("       a8 = ")); OLED.println(s8);  // แสดงค่าเซนเซอร์ S3
    OLED.println(F(""));
    OLED.print(F("       b1 = ")); OLED.println(sb1);  // แสดงค่าเซนเซอร์ S0
    OLED.print(F("       b2 = ")); OLED.println(sb2);  // แสดงค่าเซนเซอร์ S1
    OLED.print(F("       b3 = ")); OLED.println(sb3);  // แสดงค่าเซนเซอร์ S2
    OLED.print(F("       b4 = ")); OLED.println(sb4);  // แสดงค่าเซนเซอร์ S3
    OLED.print(F("       b5 = ")); OLED.println(sb5);  // แสดงค่าเซนเซอร์ S0
    OLED.print(F("       b6 = ")); OLED.println(sb6);  // แสดงค่าเซนเซอร์ S1
    OLED.print(F("       b7 = ")); OLED.println(sb7);  // แสดงค่าเซนเซอร์ S2
    OLED.print(F("       b8 = ")); OLED.println(sb8);  // แสดงค่าเซนเซอร์ S3
    OLED.display();
    delay(50);
  }
}

////////////////////////////ฟังก์ชันเซอร์โว////////////////////////////////////////
void sv_knob() {
  while(true){
  uint16_t vr = analogRead(A7);  // กำหนดตัวแปรจำนวนเต็มอ่านค่าอนาล็อกที่พอร์ต 7
  uint16_t nob = map(vr, 0, 1023, 0, 180); // ทำการ map อัตราส่วนจากสัญญาณ analog 0-1023 เป็น 0-180
  OLED.clearDisplay();    // เคลียร์หน้าจอ oled
  OLED.setTextColor(WHITE, BLACK);  //สีอักษรเป็นสีขาว ,พื้นหลังดำ
  OLED.setCursor(0, 0);       // เซตตำแหน่ง 0,0
  OLED.setTextSize(2);        // เซตขนาดอักษรมีขนาดเป็น 2
  OLED.print(F("SV = "));     // พิมพ์คำว่า SV1 =
  OLED.println(nob);     // นำค่า nob มาแสดงใน oled
  OLED.display();        // เปิดฟังก์ชันแสดงผล
  sv1.write(nob);        // สั่งเซอร์โวมอเตอร์ให้หมุนไปตามค่าองศาที่ทำการ nob ไว้
  sv2.write(nob);        // สั่งเซอร์โวมอเตอร์ให้หมุนไปตามค่าองศาที่ทำการ nob ไว้
  delay(50);             // หน่วงเวลา 0.05 วินาที
  }
}

////////////////////////ฟังก์ชันเส้นทาง////////////////////////////////////////

void P()     ///////////// ฟังก์ชันแทกเส้นเจอคู่ดำหยุด/////////////////////////////
{
  while (1){
    analogs();
    if (((s1<a1)and(s2<a2))or((s7<a7)and(s8<a8))){
      run(100, 100);delay(180);run(0, 0);delay(100);break; // หยุดคำสั่ง
    }

    else if(s2<a2){ // S2
      run(-40,100);
    }
    else if(s3<a3){ // S3
      run(70,100);
    }
    else if(s6<a6){ // S6
      run(100,70);
    }
    else if(s7<a7){ // S7
      run(100,-40);
    }
    else{
      run(100,100);
    }
   
  }
}

//////////////////////////////////////////////////////////////////////////////////

void P(unsigned long t)
{
  unsigned long start = millis();
  while (1) {
    analogs();
    if (millis() - start >= t) {run(0, 0);break;} //หยุดคำสั่ง

    else if(s2<a2){ // S2
      run(-40,100);
    }
    else if(s3<a3){ // S3
      run(70,100);
    }
    else if(s6<a6){ // S6
      run(100,70);
    }
    else if(s7<a7){ // S7
      run(100,-40);
    }
    else{
      run(100,100);
    }

  }
}
/////////////////////////////////////////////////////////////////////////////////////////
void B()     ///////////// ฟังก์ชันแทกเส้นเจอคู่ดำหยุด/////////////////////////////
{
  while (1){
    analogs();
    if (((s1<a1)and(s2<a2))or((s7<a7)and(s8<a8))){
      run(100, 100);delay(180);run(0, 0);delay(100);break; // หยุดคำสั่ง
    }

    else if(s2<a2){ // S2
      run(-100,40);
    }
    else if(s3<a3){ // S3
      run(-100,-70);
    }
    else if(s6<a6){ // S6
      run(-70,-100);
    }
    else if(s7<a7){ // S7
      run(40,-100);
    }
    else{
      run(-100,-100);
    }
   
  }
}

//////////////////////////////////////////////////////////////////////////////////

void B(unsigned long t)
{
  unsigned long start = millis();
  while (1) {
    analogs();
    if (millis() - start >= t) {run(0, 0);break;} //หยุดคำสั่ง

    else if(s2<a2){ // S2
      run(-100,40);
    }
    else if(s3<a3){ // S3
      run(-100,-70);
    }
    else if(s6<a6){ // S6
      run(-70,-100);
    }
    else if(s7<a7){ // S7
      run(40,-100);
    }
    else{
      run(-100,-100);
    }

  }
}

//////////////////////////////////////////////////////////////////////////////////

void ST(uint16_t t){run(100,100);delay(t);run(0,0);delay(100);} // เดินหน้า
void BT(uint16_t t){run(-100,-100);delay(t);run(0,0);delay(100);} // ถอยหลัง
void LD(uint16_t t){run(-100,100);delay(t);run(0,0);delay(100);} // ถอยหลัง
void RD(uint16_t t){run(100,-100);delay(t);run(0,0);delay(100);} // ถอยหลัง
void S_slow(byte s,uint16_t t){run(s,s);delay(t);run(0,0);delay(100);} // ถอยหลัง
void B_slow(byte s,uint16_t t){run(-(s),-(s));delay(t);run(0,0);delay(100);} // ถอยหลัง

/////////////////////// ฟังก์ชันเซฮร์โว //////////////////////////

void sv_set(uint8_t set_angle)   // เซตเซอร์โว
{
  sv1.write(set_angle); // ค่าองศาเซอร์โวตอนเปิดบอร์ด
  sv2.write(set_angle);
}


void up(){ // เซอร์โวยก

  uint8_t angle1 = 120;
  
  delay(100);
  for(uint8_t i=angle_ss1;i<=angle1;i++){ //ขึ้น
    sv1.write(i);
    i = angle_ss1;
    delay(3);
  }

}

void down(){ // เซอร์โววาง
  uint8_t angle1 = 50;
  for(uint8_t i=angle_ss1;i>=angle1;i--){ // ลง
    sv1.write(i);
    i = angle_ss1;
    delay(3);
  }
  delay(100);

}

void keep(){ // เซอร์โวคีบ
  uint8_t angle2 = 50;
  for(uint8_t i=angle_ss2;i>=angle2;i--){ // ปล่อย
    sv2.write(i);
    i = angle_ss2;
    delay(3);
  }
  delay(100);
}

void pause(){ // เซอร์โวปล่อย
  uint8_t angle2 = 120;
  for(uint8_t i=angle_ss2;i<=angle2;i++){ // คีบ
    sv2.write(i);
    i = angle_ss2;
    delay(3);
  }
  delay(100);
}


/////////////////////////เลี้ยวซ้าย///////////////////////////////////////////

void L(){L0();L1();L2();delay(100);} /////// เลี้ยวซ้าย
void L0(){while (1){analogs();run(-150,150);if(s1<a1){run(0,0);break;}}} //S0
void L1(){while (1){analogs();run(-150,150);if(s2<a2){run(0,0);break;}}} //S1
void L2(){while (1){analogs();run(-150,150);if(s4<a4){run(0,0);break;}}} //S2


//////////////////////เลี้ยวขวา//////////////////////////////////////////
void R(){R0();R1();R2();delay(100);} ///////// เลี้ยว
void R0(){while (1){analogs();run(150,-150);if(s8<a8){run(0,0);break;}}} //S0
void R1(){while (1){analogs();run(150,-150);if(s7<a7){run(0,0);break;}}} //S1
void R2(){while (1){analogs();run(150,-150);if(s5<a5){run(0,0);break;}}} //S2


/////////////////////////////ฟังก์ชันมอเตอร์///////////////////////////////////

void run(int spl, int spr)   // ประกาศฟังก์ชัน run(กำลังมอเตอร์ซ้าาย,กำลังมอเตอร์ขวา);
{
  if (spl > 0)
  {
    digitalWrite(DL1, LOW);
    digitalWrite(DL2, HIGH);
    analogWrite(PWML, spl);
  }
  else if (spl < 0)
  {
    digitalWrite(DL1, HIGH);
    digitalWrite(DL2, LOW);
    analogWrite(PWML, -spl);

  }
  else
  {
    digitalWrite(DL1, HIGH);
    digitalWrite(DL2, HIGH);
  }
  //////////////////////////////////////
  if (spr > 0)
  {
    digitalWrite(DR1, LOW);
    digitalWrite(DR2, HIGH);
    analogWrite(PWMR, spr);
  }
  else if (spr < 0)
  {
    digitalWrite(DR1, HIGH);
    digitalWrite(DR2, LOW);
    analogWrite(PWMR, -spr);
  }
  else
  {
    digitalWrite(DR1, HIGH);
    digitalWrite(DR2, HIGH);
  }
}

void motorTest(){
  run(100,100);delay(500);run(0,0);delay(500);
  run(-100,100);delay(500);run(0,0);delay(500);
  run(100,-100);delay(500);run(0,0);delay(500);
  run(-100,-100);delay(500);run(0,0);delay(500);
}


//////////////////////////////// เรียกใช้ code menu ////////////////////////////////////////
/*
  P();  , P(เวลา);          ST(เวลา);
  B();  , B(เวลา);          BT(เวลา);
  L();
  R();
  up();
  down();
  keep();
  pause();
*/

void menu2()   /// code 2 ที่นี่
{
  motorTest();
}
void menu3()   /// code 3 ที่นี่
{
  down();P();R();P();L();P();up();R();down();
}

void menu4()   /// code 4 ที่นี่
{
  P();
}

void menu5()   /// code 5 ที่นี่
{
  L();
}

void menu6()   /// code 6 ที่นี่
{
  R();
}

void menu7()   /// code 7 ที่นี่
{
  ST(500);
}

void menu8()   /// code 8 ที่นี่
{
  down();
}

void menu9()   /// code 9 ที่นี่
{
  up();
}


