#include <Servo.h>
#include <Wire.h>

#define ID 7        //아두이노 id

Servo myServo;

int angle;          //각도 변수
int pattern = 0;    //마스터로부터 받은 값
int before = -10000; //제설장치 가동제한을 위한 변수

void setup(){
  Wire.begin(ID);   //통신 연결
  Wire.onReceive(readSerial); //마스터로부터 값을 받아오게 하는 함수
  Serial.begin(9600); //시리얼통신 열기

  myServo.attach(12);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

void loop(){
  if(pattern == 2 && pattern != before) {
    analogWrite(5,250);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    for (angle=90; angle>=30;angle-=1){
      myServo.write(angle);
      delay(15);
    }
    for (angle=30; angle<=90;angle+=1){
      myServo.write(angle);
      delay(15);
    }
    delay(5000);
    before = pattern;
  }
  else {
    analogWrite(5,0);  //dc모터 끄는 건데 검증이 필요함
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
  }
  if(before != pattern) before = -10000; //만약 pattern 값이 변경되었다면 초기화하기
  delay(1000);
}

void readSerial() {
  pattern = Wire.read();      //시리얼통신으로 값 읽음
  Serial.println(pattern);    //패턴출력
}
