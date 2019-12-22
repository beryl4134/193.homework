#include <SoftwareSerial.h>
SoftwareSerial mySerial(3,2); // RX, TX
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Goodnight moon!");
  // 设置ESP8266的data rate
  mySerial.begin(115200);
  mySerial.println("Hello, world???");
}
void loop() { 
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
//串口监视器：
//AT   获得OK
//AT+CWMODE=3      获得OK
//AT+CWJAP="你的Wifi ID","你的Wifi密码"     获得OK
//AT+CIPMUX=0      获得OK
//AT+CIPMODE=1     获得OK
//AT+CIPSTART="TCP","183.230.40.33",80     获得OK
//AT+SEND
//获得“>”
//然后就可以透传了 
