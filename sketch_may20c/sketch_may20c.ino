#include <FrequencyTimer2.h>
#include <Wire.h>
//도트매트릭스에 출력할 모양 정의
#define SPACE { \
  {0, 0, 0, 0, 0, 0, 0, 0}, \
  {0, 0, 0, 0, 0, 0, 0, 0}, \
  {0, 0, 0, 0, 0, 0, 0, 0}, \
  {0, 0, 0, 0, 0, 0, 0, 0}, \
  {0, 0, 0, 0, 0, 0, 0, 0}, \
  {0, 0, 0, 0, 0, 0, 0, 0}, \
  {0, 0, 0, 0, 0, 0, 0, 0}, \
  {0, 0, 0, 0, 0, 0, 0, 0} \
}

#define WARNING { \
  {0, 0, 0, 1, 1, 0, 0, 0}, \
  {0, 0, 0, 0, 0, 0, 0, 0}, \
  {0, 0, 1, 0, 0, 1, 0, 0}, \
  {0, 0, 0, 0, 0, 0, 0, 0}, \
  {0, 1, 0, 0, 0, 0, 1, 0}, \
  {0, 0, 0, 0, 0, 0, 0, 0}, \
  {1, 0, 0, 0, 0, 0, 0, 1}, \
  {1, 1, 1, 1, 1, 1, 1, 1} \
}

#define SNOWREMOVEAL1 { \
  {0, 0, 0, 0, 1, 0, 0, 0}, \
  {0, 0, 0, 0, 1, 0, 0, 0}, \
  {0, 0, 0, 0, 1, 0, 0, 0}, \
  {1, 1, 1, 1, 1, 0, 0, 0}, \
  {0, 0, 0, 1, 1, 1, 1, 1}, \
  {0, 0, 0, 1, 0, 0, 0, 0}, \
  {0, 0, 0, 1, 0, 0, 0, 0}, \
  {0, 0, 0, 1, 0, 0, 0, 0} \
}

#define SNOWREMOVEAL2 { \
  {0, 0, 0, 1, 0, 0, 0, 0}, \
  {0, 0, 0, 1, 0, 0, 0, 0}, \
  {0, 0, 0, 1, 0, 0, 0, 0}, \
  {0, 0, 0, 1, 1, 1, 1, 1}, \
  {1, 1, 1, 1, 1, 0, 0, 0}, \
  {0, 0, 0, 0, 1, 0, 0, 0}, \
  {0, 0, 0, 0, 1, 0, 0, 0}, \
  {0, 0, 0, 0, 1, 0, 0, 0} \
}

#define SNOWREMOVEAL3 { \
  {1, 0, 0, 0, 0, 0, 0, 1}, \
  {0, 1, 0, 0, 0, 0, 1, 0}, \
  {0, 0, 1, 0, 0, 1, 0, 0}, \
  {0, 0, 0, 1, 1, 0, 0, 0}, \
  {0, 0, 0, 1, 1, 0, 0, 0}, \
  {0, 0, 1, 0, 0, 1, 0, 0}, \
  {0, 1, 0, 0, 0, 0, 1, 0}, \
  {1, 0, 0, 0, 0, 0, 0, 1} \
}

#define ID 8   //아두이노 id

byte col = 0; //가로
byte leds[8][8];  //led 배열
// 핀설정
int pins[17] = {-1,5,4,3,2,14,15,16,17,13,12,11,10,9,8,7,6};
int rows[8] = {pins[9], pins[14], pins[8], pins[12], pins[1], pins[7], pins[2], pins[5]};
int cols[8] = {pins[13], pins[3], pins[4], pins[10], pins[6], pins[11], pins[15], pins[16]};
// 표현하고 싶은 배열 수
const int numPatterns = 5;
// 표현하려는 배열
byte patterns[numPatterns][8][8] = {SPACE, WARNING, SNOWREMOVEAL1, SNOWREMOVEAL2, SNOWREMOVEAL3};

int pattern = 0;  //표현할 패턴 그리고 마스터로부터 받은 값

void setup() {
  Wire.begin(ID); //슬레이브 번호로 통신 열기
  Wire.onReceive(readSerial); //데이터 받는 함수
  Serial.begin(9600); //시리얼통신 열기

  //3개 for문 모두 핀 설정을 위한 용도
  for(int i = 1; i <= 16; i++) pinMode(pins[i],OUTPUT);
  for(int i = 1; i <= 8; i++) digitalWrite(cols[i-1], HIGH);
  for(int i = 1; i <= 8; i++) digitalWrite(rows[i-1], LOW);
  
  clearLeds();  //led 모두 끔
  
  FrequencyTimer2::disable();
  FrequencyTimer2::setPeriod(2000);
  FrequencyTimer2::setOnOverflow(display);
}

void loop() {
  if(pattern == 2) {  //마스터로부터 받은 값이 2라면 제설장치 가동 패턴실행
    for(int i=-1;i<3;i++){
        setPattern(pattern+i);
        delay(1000);
        clearLeds();
    }
  }
  else setPattern(pattern); //그 외의 값이라면 그 값에 해당되는 패턴실행
}

void readSerial() {           //슬레이브 데이터 받는 함수
  pattern = Wire.read();      //시리얼통신으로 값 읽음
  Serial.println(pattern);    //시리얼보드에서 확인용
}

void clearLeds() {            //led 모두 끄는 함수
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++)
      leds[i][j] = 0;
}

void setPattern(int pattern) {//원하는 패턴 만드는 함수
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      leds[i][j] = patterns[pattern][i][j];
}

void display() {
  digitalWrite(cols[col], HIGH);
  col++;
  if (col == 8) col = 0;
  for (int row = 0; row < 8; row++) {
    if (leds[col][7 - row] == 1) digitalWrite(rows[row], HIGH);
    else digitalWrite(rows[row], LOW);
  }
  digitalWrite(cols[col], LOW);
}
