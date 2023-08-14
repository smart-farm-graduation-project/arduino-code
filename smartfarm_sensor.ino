#include "DHT.h"
#include <SoftwareSerial.h>

#define DHTPIN 4     // DHT11이 연결된 핀
#define DHTTYPE DHT11   // DHT 11, DHT시리즈중 11을 선택합니다.

const int AirValue = 620;   //습도 하한
const int WaterValue = 310;  //습도 상한

DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial mySerial(13, 11);                     // RX 13, TX 11, 이산화탄소 핀
unsigned char Send_data[4] = {0x11,0x01,0x01,0xED};    // Read Command, 이산화탄소 커맨드
unsigned char Receive_Buff[8];                         // data buffer
unsigned char recv_cnt = 0;

char cmd;//라즈베리파이에서 시리얼로 오는 커맨드

int getSoilMoisturePercent()//토양습도 퍼센트값 계산 함수
{
  int soilMoistureValue = 0;
  int soilmoisturepercent=0;
  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  if(soilmoisturepercent >= 100)
  {
    return 100;
  }
  else if(soilmoisturepercent <=0)
  {
    return 0;
  }
  else if(soilmoisturepercent >0 && soilmoisturepercent < 100)
  {
    return soilmoisturepercent;
  }
}
void Send_CMD(void)                                    // 이산화탄소 커맨드
{
  unsigned int i;
  for(i=0; i<4; i++)
  {
    mySerial.write(Send_data[i]);
    delay(1);
  }
}
unsigned char Checksum_cal(void)                       // 이산화탄소 버퍼 체크섬
{
  unsigned char count, SUM=0;
  for(count=0; count<7; count++)
  {
     SUM += Receive_Buff[count];
  }
  return 256-SUM;
}

unsigned int getPPM(void)//이산화탄소ppm 계산 함수
{
  
  while(1)
  {
    if(mySerial.available())
    { 
       Receive_Buff[recv_cnt++] = mySerial.read();
      if(recv_cnt ==8){recv_cnt = 0; break;}
    }
  } 
  
  if(Checksum_cal() == Receive_Buff[7])  // CS 확인을 통해 통신 에러 없으면
  {
     unsigned int PPM_Value = Receive_Buff[3]<<8 | Receive_Buff[4];  // CALCULATE PPM
     return PPM_Value;
  }
  else
  {
    Serial.write("CHECKSUM Error");
  }
}



void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(13,INPUT);
  pinMode(11,OUTPUT);
  while (!Serial) ;
  mySerial.begin(9600);
  while (!mySerial);
  Serial.println("sensor init");  
}

void loop() {
  delay(2000);

  Send_CMD();  // Send Read Command

  int h = dht.readHumidity();// 습도를 측정합니다.

  int t = dht.readTemperature();// 온도를 측정합니다.

  int smPercent = getSoilMoisturePercent();//토양 습도를 측정합니다

  unsigned int PPM_Value = getPPM();                                // 농도(PPM) 저장 변수


  // 값 읽기에 오류가 있으면 오류를 출력합니다.
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // 컴퓨터로부터 시리얼 통신이 전송되면, 한줄씩 읽어와서 cmd 변수에 입력
  if(Serial.available()){
    cmd = Serial.read(); 

    if(cmd=='a'){

      Serial.print(h);//humidity %
      Serial.print("/");

      Serial.print(t);//Temperature *C
      Serial.print("/");
      
      Serial.print(PPM_Value);//Co2 ppm
      Serial.print("/");

      Serial.print(smPercent);//soil moisture %

      Serial.println();
      delay(100);
    }
    else{}
  }

  
  while(Serial.available() > 0){
        Serial.read();
  }
  delay(1000);        // 1000ms
}
