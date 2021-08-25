#include <SoftwareSerial.h>

#define BT_RXD 8
#define BT_TXD 7
SoftwareSerial bluetooth(BT_TXD, BT_RXD);

 // 13번 핀을 LED로 지정합니다.
int ledPin = 13;
// 디지털 2번핀을 불꽃감지센서로 지정합니다.
int inputPin = 2;
// 불꽃 감지 센서의 상태를 저장합니다.(처음 상태를 LOW로 설정)
int pirState = LOW;
// 센서 값을 읽기 위해 변수를 선언합니다. 
int val = 0;           
// PWM 핀에 스피커 혹은 피에조 센서를 연결합니다.(digital 9, 10, or 11)
int pinSpeaker = 10;
int MQ2pin = A0;   // 가스센서 입력을 위한 아날로그 핀

float val2;


void setup() {
   // LED 를 출력으로 설정합니다.
  pinMode(ledPin, OUTPUT);      
  // 센서를 입력으로 설정합니다.
  pinMode(inputPin, INPUT);     
  // 스피커를 출력으로 설정합니다.
  pinMode(pinSpeaker, OUTPUT); 
  // 시리얼 통신(9600)를 준비합니다.
  pinMode(MQ2pin ,INPUT);   // 아날로그 핀 A0를 입력모드로 설정

  Serial.begin(9600); 
  bluetooth.begin(9600);
}

void loop() {
  if (bluetooth.available()){
    Serial.write(bluetooth.read());
  }
  if (Serial.available()){
    bluetooth.write(Serial.read());
  }

  
  // 센서값을 읽어들입니다.
  val = digitalRead(inputPin); 
  Serial.println(analogRead(MQ2pin));   // 가스센서로부터 아날로그 데이터를 받아와 시리얼 모니터로 출력함
  bluetooth.println(analogRead(MQ2pin));
  delay(1000);  // 1s 대기 


  val2 = analogRead(MQ2pin);
  if(val2 > 500){
    Serial.print("연기감지!!!");
    bluetooth.write("연기감지!!!");
    delay(1000);
    playTone(300, 160);
    delay(150);
    Serial.println("");
  }
  delay(2000);
  
  // 만약 값이 HIGH 일때,
  if (val == HIGH) {            
    // 13번 핀(보드에 내장되어 있는 LED) 를 켭니다.
    digitalWrite(ledPin, HIGH);  
    playTone(300, 160);
    delay(150);
  
    
    if (pirState == LOW) {
      // 센서의 상태가 LOW일때
      Serial.println("FIRE!!!!");
      bluetooth.write("FIRE!!!!");
      delay(1000);
      // 다음의 문구를 시리얼 모니터로 출력합니다.
      pirState = HIGH;
    }
    
  } else {
    // LED를 끕니다.
    digitalWrite(ledPin, LOW); 
    playTone(0, 0);
    delay(30);    
    if (pirState == HIGH) {
      //센서값이 HIGH 일때 
      Serial.println("Beware of fire.");
      bluetooth.write("Beware of fire.");
      delay(1000);
      // 다음의 문구를 시리얼 모니터로 출력합니다.
      pirState = LOW;
    }
  }
}

// 경보음을 만들어 냅니다.
void playTone(long duration, int freq) {
  duration *= 1000;
  int period = (1.0 / freq) * 1000000;
  long elapsed_time = 0;
  while (elapsed_time < duration) {
    digitalWrite(pinSpeaker,HIGH);
    delayMicroseconds(period / 2);
    digitalWrite(pinSpeaker, LOW);
    delayMicroseconds(period / 2);
    elapsed_time += (period);
  }

}