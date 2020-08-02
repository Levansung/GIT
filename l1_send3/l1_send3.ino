#include <Wire.h>
#include <UIPEthernet.h>
#include <PubSubClient.h> 
#include <LiquidCrystal_I2C.h>
          LiquidCrystal_I2C lcd(0x27, 16, 2);
#include "RTClib.h"
          RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
#include <DallasTemperature.h>
#include <OneWire.h>
#define ds1 50
          OneWire onewire(ds1);
          DallasTemperature sensords(&onewire);
#include "DHT.h"
const int dhtpin = 49;                                        // pin dht chân 49 mega
const int dhttype = DHT11;
          DHT dht(dhtpin,dhttype);

byte mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x35 };
uint8_t IP[]={172,26,80,67};

const char* mqtt_server = "172.26.80.97";                      //172.26.80.17      172.26.80.100         
          EthernetClient Ethclient;
          PubSubClient client(Ethclient);

unsigned int tsensor=0;                     
void(* resetFunc) (void) = 0;//cài đặt hàm reset

// ------------------------------setup------------------------------------------

void setup() {
    Serial.begin(9600);
    lcd.begin();
    lcd.backlight();
    lcd.print("   start lcd");
    Serial.println("\nserial started in setup");
    pinMode(46, OUTPUT);
    pinMode(13,OUTPUT);
    Wire.begin();
    dht.begin();
    if (! rtc.begin()) {  Serial.print("Couldn't find RTC");  lcd.clear();  lcd.print("cannot find RTC");   while (1); }
    if (! rtc.isrunning())  {Serial.print("RTC is NOT running! \n");  lcd.clear();  lcd.print("RTC not run");}
// --------------???----------???-----------------------------------------------------
    //rtc.adjust(DateTime(2020, 7, 31, 7, 3, 0));
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
// --------------!!!------------!!!---------------------------------------------------
    delay(500);
    client.setServer(mqtt_server, 1883);
    
    while(Ethernet.linkStatus()!=LinkON && !client.connected()){
        Serial.println("\nconnecting again ethernet & MQTT");
        lcd.clear();
        lcd.print("conect E+M again");
        ethernet();
        delay(200);
        MQTTreconnect();
          }
     Serial.println("connected ethernet && MQTT");
     lcd.clear();
     lcd.print("conected E+M");
     DHT11sensor();
     delay(200);
}// end set up

//-----------------------------------------loop---------------------------------

void loop() {
   delay(300);
   checkethernet();
   delay(500);
   Serial.print("truoc disconnect: ");
   Serial.println(client.state());
   client.disconnect();
   delay(500);
   Serial.print("sau disconnect: ");
   Serial.println(client.state());
   checkMQTT();
   
   if(!client.loop()) client.connect("arduinoClient");
   
   if((millis()-tsensor)>=8000){
       DHT11sensor();
       tsensor=millis();
       delay(200);
        }
   DateTime now = rtc.now();
   if((now.hour()==14 || now.hour()==2) && now.minute()==1){
      Serial.println("reset board after for 12h");
      client.publish("arduino","arduino taking a rest");
      lcd.clear();
      lcd.print("reset board 12h");
      lcd.setCursor(0,1);
      lcd.print("reset board 12h");
      delay(20000);
      client.publish("arduino","arduino taking a rest");
      delay(10000);
      client.publish("arduino","arduino taking a rest");
      delay(15000);
      client.publish("arduino","arduino taking a rest");
      delay(1000);
      resetFunc();
    }
    delay(1000);
} // end loop

//------------------------kiểm tra mạng ethernet------------------------------

void checkethernet() {
   int h=0;
   while(1){  
        h++;
        Serial.print("\nCheck times : "+String(h));
    if (Ethernet.linkStatus() == Unknown) {
        Serial.println("- check for your devices ??? ");
        lcd.clear();
        lcd.print("check E loop");
        lcd.setCursor(0,1);
        lcd.print("check device's");
        delay(500);
        }
     else if (Ethernet.linkStatus() == LinkON) {
        Serial.println("  success ethernet Link status: On");
        delay(500);
        break;
        }
    else if (Ethernet.linkStatus() == LinkOFF) {
        Serial.println("\n Failed ethernet: link status = off");
        lcd.clear();
        lcd.print("check E loop");
        lcd.setCursor(0,1);
        lcd.print("status: link OFF");
        delay(500);
        Ethernet.begin(mac,IP);
         }
     if (h>=30){
        Serial.println("RESET board because E > 30");
        lcd.clear();
        lcd.print("called reset");
        lcd.setCursor(0,1);
        lcd.print("reset by E>30");
        delay(500);
        resetFunc();
      }
  }
}
