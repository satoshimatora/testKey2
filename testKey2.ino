//２つ以上のキーの読み込みを行う場合
//※無保証※本ソースコートを使用したことによりいかなる事態も作成者はその責を負いません

//PORT
#define KEY1PORT 2     //24pin
#define KEY2PORT 3     //25pin

//KEY
#define KEY_COUNT 2 //number of keys
#define KEY1 0
#define KEY2 1
#define KEY_SHORTTIMEFROM1 30 // from n ms
#define KEY_SHORTTIMETO1 300 // to n ms judge short push
#define KEY_SHORTTIMEFROM2 30 // from n ms
#define KEY_SHORTTIMETO2 300 // to n ms judge short push
#define KEY_LONGTIME1 1000 // more than n ms judge long push
#define KEY_LONGTIME2 1000 // more than n ms judge long push

bool longPushFlg[KEY_COUNT];

void setup() {
  byte i;
  //Port setup
  pinMode(KEY1PORT, INPUT_PULLUP); 
  pinMode(KEY2PORT, INPUT_PULLUP); 
  
  //Buildin LED
  pinMode(LED_BUILTIN, OUTPUT);
  
  //Serial
  Serial.begin(115200);

  for (i = 0; i < KEY_COUNT; i++) {
    longPushFlg[i] = false;
  }
}

void loop() {
  while (true) {
     keyCheck();
    if (longPushFlg[KEY1]) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}


/*
   Key Check Routine
*/
void key_Short(byte key_) {
  if (key_ == KEY1) {
    //KEY1 SHORT
    Serial.println("KEY1 SHORT");
  } else {
    //KEY2 SHORT
    Serial.println("KEY2 SHORT");
  }
}

void key_Long(byte key_) {
  if (key_ == KEY1) {
    //KEY1 LONG
    Serial.println("KEY1 LONG");
  } else {
    //KEY2 LONG
    Serial.println("KEY2 LONG");
  }
}

void keyCheck(void) {
  keyCheck_(KEY1PORT, KEY1, KEY_SHORTTIMEFROM1, KEY_SHORTTIMETO1, KEY_LONGTIME1); //KEY1
  keyCheck_(KEY2PORT, KEY2, KEY_SHORTTIMEFROM2, KEY_SHORTTIMETO2, KEY_LONGTIME2); //KEY2
}

void keyCheck_(byte port_, byte key_, unsigned int shortTimeFrom_, unsigned int shortTimeTo_, unsigned int longTime_) {
  unsigned long nowTime;
  unsigned int port;
  static unsigned long keyPushTime[KEY_COUNT];
  static byte longPush[KEY_COUNT];

  port = digitalRead(port_);
  nowTime = millis();

  if (longPush[key_] == 1) {
    key_Long(key_);
    longPush[key_] = 2;
    longPushFlg[key_] = true;
  } else if (port == HIGH) {
    if ((keyPushTime[key_] != 0) and (longPush[key_] != 2)) {
      if (((nowTime - keyPushTime[key_]) > shortTimeFrom_) and ((nowTime - keyPushTime[key_]) < shortTimeTo_)) {
        key_Short(key_);
      }
    }
    keyPushTime[key_] = 0;
    longPush[key_] = 0;
    longPushFlg[key_] = false;
  } else if (port == LOW) {
    if (keyPushTime[key_] == 0) {
      keyPushTime[key_] = nowTime;
    } else {
      if (((nowTime - keyPushTime[key_]) > longTime_) and (longPush[key_] != 2) ) {

        longPush[key_] = 1;
      }
    }
  }
}
