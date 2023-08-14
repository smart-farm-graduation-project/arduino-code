#define FANRELAY 6 //팬 릴레이모듈 핀
#define LEDRELAY 3 //생장등 릴레이모듈 핀
#define WATERRELAY 5 //급수기 릴레이모듈 핀

bool fanRelayCommand = true;//팬 릴레이모듈 상태(true가 작동)
bool ledRelayCommand = false;//생장등 릴레이모듈 상태(flase가 켜짐)
bool waterRelayCommand = false;//급수기 릴레이모듈 상태(true가 작동)

char cmd;//라즈베리파이에서 시리얼로 오는 커맨드

void relayOnOff(bool relayCommand,short relay){//릴레이모듈 함수
  if(relayCommand){
    digitalWrite(relay,HIGH);     // 조건 만족하면 1채널 릴레이 ON   
  }else{
    digitalWrite(relay,LOW);     // 조건 만족하면 1채널 릴레이 ON 
  }
}

void setup() {
  Serial.begin(9600);
    
  pinMode(FANRELAY,OUTPUT);         // 릴레이를 출력으로 설정
  pinMode(LEDRELAY,OUTPUT);
  pinMode(WATERRELAY,OUTPUT);
  Serial.println("actor init");
}

void loop() {
  delay(200);

  relayOnOff(fanRelayCommand,FANRELAY);
  relayOnOff(ledRelayCommand,LEDRELAY);
  relayOnOff(waterRelayCommand,WATERRELAY);

  // 컴퓨터로부터 시리얼 통신이 전송되면, 한줄씩 읽어와서 cmd 변수에 입력
  if(Serial.available()){
    cmd = Serial.read(); 

    if(cmd=='a'){
      fanRelayCommand = true;
      Serial.println("fan turn on");
      delay(100);
    }
    else if(cmd=='b'){
      fanRelayCommand = false;
      Serial.println("fan turn off");
      delay(100);
    }else if(cmd=='c'){
      ledRelayCommand = false;
      Serial.println("led turn on");
      delay(100);
    }else if(cmd=='d'){
      ledRelayCommand = true;
      Serial.println("led turn off");
      delay(100);
    }
    else if(cmd=='e'){
      waterRelayCommand = true;
      relayOnOff(waterRelayCommand,WATERRELAY);
      delay(3000);
      waterRelayCommand = false;
      relayOnOff(waterRelayCommand,WATERRELAY);
      Serial.println("watered");
      delay(100);
    }
    else{}
  }

  
  while(Serial.available() > 0){
        Serial.read();
  }
  delay(1000);        // 1000ms
}