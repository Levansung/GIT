
#include <UIPEthernet.h>
#include <PubSubClient.h> 
#include <Wire.h>
#include "RTClib.h"
          RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

byte mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x36 };
uint8_t IP[]={192,168,1,81};

const char* mqtt_server = "192.168.1.4";
          EthernetClient Ethclient;
          PubSubClient client(Ethclient);

unsigned int tdevice=0,tprogram=0;
const int light = 10;
const int fan = 8;

void(* resetFunc) (void) = 0;//cài đặt hàm reset
void MQTTreconnect();
void callback(char* topic, byte* message, unsigned int length);
          
void setup() {
    Serial.begin(9600);
    pinMode(8,OUTPUT);
    pinMode(13,OUTPUT);
    Wire.begin();
    //if (! rtc.begin()) {  Serial.print("Couldn't find RTC");   while (1); }
    //if (! rtc.isrunning())  {Serial.print("RTC is NOT running! \n");}
// --------------???----------???-----------------------------------------------------
    //rtc.adjust(DateTime(2020, 7, 31, 7, 59, 0));
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
// --------------!!!------------!!!---------------------------------------------------
    delay(500);
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    Serial.println("\nserial started in setup");
    while(Ethernet.linkStatus()!=LinkON && !client.connected()){
        Serial.println("\n connecting again ethernet & MQTT");
        ethernet();
        delay(200);
        MQTTreconnect();
    }
    Serial.println("connected ethernet && MQTT");
    updatestatus();
    delay(200);

}

void loop() {
    delay(200);
    checkethernet();
    if (!client.connected()) {
        MQTTreconnect();
        Serial.println(Ethernet.linkStatus());
      }
    if(!client.loop())
        if(!client.loop()) client.connect("arduinoClient",NULL,NULL,0,0,2,0,1);

//    if((millis()-tdevice)>=60000){
//        updatestatus();
//        tdevice = millis();
//        delay(2000);
//      }
    delay(1000);

} //end loop

//---------------------- callback from MQTT -------------------------------
void callback(char* topic, byte* message, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;
  
    for (int i = 0; i < length; i++) {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
      }
    Serial.println();
    Serial.println("Topic is: "+String(topic));
    if((String)topic=="room/lamp"){                    // room/lamp == relay1      room/relay2 == relay2
        Serial.print("Changing Room lamp to ");
        if(messageTemp == "on"){
            digitalWrite(13, HIGH);
            Serial.print(" led8 On");
        }
        else if(messageTemp == "off"){
            digitalWrite(13, LOW);
            Serial.print("led8 Off");
      }
  } // end if

  if((String)topic=="room/relay2"){
      Serial.print("Changing Room lamp to ");
      if(messageTemp == "on1"){
         digitalWrite(8, HIGH);
         Serial.print(" led13 On");
      }
      else if(messageTemp == "off1"){
         digitalWrite(8, LOW);
         Serial.print("led13 Off");
      }
  }
  Serial.println();
}// end callback






 
