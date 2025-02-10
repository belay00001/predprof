                                                    // Пины для моторов и датчиков линий
#define motor1Pin  5   // Пин для первого мотора (передний правый)
#define DIR_1        4
 
#define DIR_2        7
#define motor2Pin  6  // Пин для второго мотора (передний левый)
#include "Adafruit_TCS34725.h"
 
// Создаём объект для работы с датчиком
Adafruit_TCS34725 colorSensor;
 
// Назначаем пин для управления подсветкой
constexpr uint8_t PIN_BACKLIGHT = 3;
byte hsp = 56;
byte lsp=20;
bool p;
#include <Servo.h>
Servo srvf,srvb;

void mc(byte r_sp, byte l_sp , bool r_dir, bool l_dir){

  digitalWrite(4,l_dir);
  digitalWrite(7,r_dir);
  analogWrite(motor1Pin,l_sp);
  analogWrite(motor2Pin,r_sp);
}
int color() {
  // Переменные для хранения цветов R, G и B
  float r, g, b;
  // Считываем цвета с датчика цвета
  colorSensor.getRGB(&r, &g, &b);
  r=constrain(map(r,90,185,0,255),0,255);
  g=constrain(map(g,75,95,0,255),0,255);
  b=constrain(map(b,80,135,0,255),0,255);
  int c=0;
  // Вычисляем какой цвет преобладает в зоне видимости датчика
  if ((r<190) && (g<190) && (b<190))
    c=0;
  else if ((r > g) && (r > b))
    c=1;
  else if ((g > r) && (g > b))
    c=2;
  else if ((b > g) && (b > r))
    c=3;
  Serial.println(c);
  return(c);
}


bool gr = 0;
const int lineSensor1Pin = 8; // Пин для первого датчика линии
const int lineSensor2Pin = 9;
const int lineSensor3Pin = 10; // Пин для второго датчика линии

void line(){
  int i = 0;
  
  srvb.write(0);
  srvf.write(0);
  while (true){
    
    byte c = color();
    bool l1 = digitalRead(lineSensor1Pin);
    bool l = digitalRead(lineSensor3Pin);
    bool l2 = digitalRead(lineSensor2Pin);
    if (c == 0){
     if (l1 == 0 && l2 == 0 && l == 1 ){
      mc(hsp-6, hsp-6, 0,0);
    }
    else if(l == 0 && l1 == 1 && l2 == 0){
      mc(hsp-5, lsp+7,0, 1);
    }
    else if (l == 0 && l1 == 0 && l2 == 1){
      mc(lsp+12, hsp-10,1, 0);
    }
    else if(l == 1 && l1 == 0 && l2 == 1){                                   // false(0) - белое; true(1) - черное
      mc(0,0,0,0);
      delay(50);                                             
      mc(lsp, hsp+10, 1, 0);
      delay(800);
    }
    else if( l == 1 && l1 == 1 && l2 == 1){
      mc(0,0,0,0);
      delay(50);                                             
      mc(lsp, hsp+10, 1, 0);
      delay(800);
    }
    else if(l == 0 && l2 == 1 && l1 == 1){
      mc(hsp+10,hsp+10, 0, 1);
      delay(700);
    }
    }
    if(c == 1){
      redF();
    }
    if (c==2){
      greenF();
      hsp = hsp + 10;
      lsp = lsp + 10;
      
    }
    if (c==3){
      blueF();
    }
    



    
    delay(5);
  }



}

void redF(){
  mc(56+14,56+14,1,1);
  delay(300);
  mc(56+14, 56+14, 0, 1);
  delay(350);
  mc(0,0,0,0);
  delay(500);
  bool l;
  do{
    l = digitalRead(lineSensor3Pin);
    mc(20+35,56+45,0,0);
  }
  while(!l);
  mc(0,0,0,0);
  delay(200);
  mc(56+14, 56+14, 0, 1);
  delay(350);
  
}

void greenF(){
  if (!gr){
    mc(70,70,1,1);
  delay(400);
  mc(70,70, 0, 1);
  delay(700);
  mc(60,60,1,1);
  delay(350);
  srvb.write(180);
  delay(400);
  mc(60,60,0,0);
  delay(200);
  gr = 1;
  }
  else{
    mc(54, 54, 1, 1);
   delay(150);
   mc(70,10,1,0);
   delay(750);
   srvb.write(0);
  }
  
  
  
}
void blueF(){
  mc(0,0,0,0);
  srvf.write(180);
  delay(700);
  mc(hsp,hsp, 0, 1);
  delay(700);
  mc(hsp-6,hsp-6,0,0);
  delay(600);
  mc(0,0,0,0);
  srvf.write(0);
  delay(700);
  mc(hsp-6,hsp-6,1,1);
  delay(600);
  mc(0,0,0,0);
  mc(hsp,hsp, 1, 0);
  delay(400);
  }


void setup() {
  pinMode(PIN_BACKLIGHT, OUTPUT);
  digitalWrite(PIN_BACKLIGHT, HIGH);
  // Настройка пинов для моторов
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  Serial.begin(9600);
  
  srvf.attach(12);
  srvb.attach(11);

  line();

  // Настройка пинов для датчиков линии
  pinMode(lineSensor1Pin, INPUT);
  pinMode(lineSensor2Pin, INPUT);
  pinMode(lineSensor3Pin, INPUT);
  // Инициализация серийного порта для отладки



  Serial.begin(9600);
}







void loop() {
  // Считываем данные с датчиков линии
  bool l1 = digitalRead(lineSensor1Pin);  // Считываем первый датчик
  bool l2 = digitalRead(lineSensor2Pin); 
  bool l3 = digitalRead(lineSensor3Pin); // Считываем второй датчик
  Serial.println('левый',l1);
  Serial.println('правый',l2);
  Serial.println('центр',l3);
  


}
