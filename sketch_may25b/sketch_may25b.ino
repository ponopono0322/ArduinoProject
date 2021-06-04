#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <swRTC.h>
#include <Wire.h>

#define DHTPIN1 11  //센서1 핀 설정
#define DHTPIN2 10  //센서2 핀 설정
#define DHTPIN3 9   //센서3 핀 설정

#define SLAVE1 8    //슬레이브1 아이디
#define SLAVE2 7    //슬레이브2 아이디

#define DHTTYPE    DHT11                //센서기종

DHT_Unified dht1(DHTPIN1, DHTTYPE);     //센서넘버링1
DHT_Unified dht2(DHTPIN2, DHTTYPE);     //센서넘버링2
DHT_Unified dht3(DHTPIN3, DHTTYPE);     //센서넘버링3

uint32_t delayMS;   //딜레이 시간 지정
swRTC rtc;          //타임스테프 하기위한것

void setup() {
  Wire.begin(); //통신 열기
  Serial.begin(9600); //시리얼통신 열기
  dht1.begin(); dht2.begin(); dht3.begin(); //온습도센서 열기

  sensor_t sensor;  //온습도센서 변수
  
  dht1.temperature().getSensor(&sensor);  //온도1
  dht2.temperature().getSensor(&sensor);  //온도2
  dht3.temperature().getSensor(&sensor);  //온도3
  dht1.humidity().getSensor(&sensor);     //습도1
  dht2.humidity().getSensor(&sensor);     //습도2
  dht3.humidity().getSensor(&sensor);     //습도3
  
  delayMS = sensor.min_delay / 1000;      //딜레이타임
  //타임스테프를 만들기 위한 함수들
  rtc.stopRTC();            //정지
  rtc.setTime(16, 00, 00);  //시, 분, 초
  rtc.setDate(3, 6, 2021); //일, 월, 년
  rtc.startRTC();           //시작
}

void loop() {
  delay(delayMS);   //딜레이
  //타임스테프 출력
  Serial.print(rtc.getYear(), DEC);     Serial.print(" ");
  Serial.print(rtc.getMonth(), DEC);    Serial.print(" ");
  Serial.print(rtc.getDay(), DEC);      Serial.print(" ");
  Serial.print(rtc.getHours(), DEC);    Serial.print(":");
  Serial.print(rtc.getMinutes(), DEC);  Serial.print(":");
  Serial.print(rtc.getSeconds(), DEC);  Serial.print(",");
  //dth센서 받아오는 변수
  sensors_event_t event1;
  sensors_event_t event2;
  sensors_event_t event3;
  //각 센서별 온도 값 받아오는 함수
  dht1.temperature().getEvent(&event1); 
  dht2.temperature().getEvent(&event2);
  dht3.temperature().getEvent(&event3);
  float temp1, temp2, temp3;  //온도를 저장하기 위한 변수
  if(isnan(event1.temperature)) temp1 = -10000;
  else temp1 = event1.temperature;
  if(isnan(event2.temperature)) temp2 = -10000;
  else temp2 = event2.temperature;
  if(isnan(event3.temperature)) temp3 = -10000;
  else temp3 = event3.temperature;
  //각 센서별 습도 값 받아오는 함수
  dht1.humidity().getEvent(&event1);
  dht2.humidity().getEvent(&event2);
  dht3.humidity().getEvent(&event3);
  float hum1, hum2, hum3;   //습도를 저장하기 위한 변수
  if(isnan(event1.relative_humidity)) hum1 = -10000;
  else hum1 = event1.relative_humidity;
  if(isnan(event2.relative_humidity)) hum2 = -10000;
  else hum2 = event2.relative_humidity;
  if(isnan(event3.relative_humidity)) hum3 = -10000;
  else hum3 = event3.relative_humidity;

  byte x; //통신으로 보낼 값을 저장하는 변수
    temp3 = 4;
  //우선적으로 데이터 출력함
  Serial.print("1");    Serial.print(",");
  Serial.print(temp1);  Serial.print(",");
  Serial.print(temp2);  Serial.print(",");
  Serial.print(temp3);  Serial.print(",");
  Serial.print("2");    Serial.print(",");
  Serial.print(hum1);   Serial.print(",");
  Serial.print(hum2);   Serial.print(",");
  Serial.println(hum3);

  temp1 = 2; temp2 = 3; temp3 = 5; hum1 = 88; hum2 = 88; hum3 = 70;

  if(temp3 > 5) {
    Serial.println("기온이 5 도 이상입니다");
    x = 0;
    Wire.beginTransmission(SLAVE1);   Wire.write(x);   Wire.endTransmission();
    Wire.beginTransmission(SLAVE2);   Wire.write(x);   Wire.endTransmission();
  }
  else {
    if(temp2 < 5 && hum2 > 82 && temp1 < 5 && temp1 > 3 && hum1 > 82) {
      Serial.println("블랙 아이스가 일어난 것 같습니다");
      x = 1;
      Wire.beginTransmission(SLAVE1);   Wire.write(x);   Wire.endTransmission();
      Wire.beginTransmission(SLAVE2);   Wire.write(x);   Wire.endTransmission();
    }
    else if(temp2 < 5 && hum2 > 82 && temp1 < 3 && hum1 > 82) {
      Serial.println("제설을 시작합니다");
      x = 2;
      Wire.beginTransmission(SLAVE1);   Wire.write(x);   Wire.endTransmission();
      Wire.beginTransmission(SLAVE2);   Wire.write(x);   Wire.endTransmission();
    }
    else {
      Serial.println("기온이 높거나 또는 습도가 낮아 결빙이 일어나지 않습니다");
      x = 0;
      Wire.beginTransmission(SLAVE1);   Wire.write(x);   Wire.endTransmission();
      Wire.beginTransmission(SLAVE2);   Wire.write(x);   Wire.endTransmission();
    }
  }

}
