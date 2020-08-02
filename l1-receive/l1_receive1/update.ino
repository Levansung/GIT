
//--------------- cap nhat trang thai thiet bi ----------------
 void updatestatus() {
    if(analogRead(8) == HIGH){
        Serial.println("relay1 ON");
        client.publish("room/lamp", "RELAY 1 ON");
        client.publish("room/lamp_status","system relay1 ok");
      }
    else {
        Serial.println("relay1 OFF");
        client.publish("room/lamp","RELAY 1 OFF");
        client.publish("room/lamp_status","system relay1 ok");
      }
   delay(2000);
   if(analogRead(13)==HIGH) {
      Serial.println("relay2 ON");
      client.publish("room/relay2","RELAY 2 ON");
      client.publish("room/lamp_status","system relay2 ok");
    }
   else {
      Serial.println("relay2 OFF");
      client.publish("room/relay2","RELAY 2 OFF");
      client.publish("room/lamp_status","system relay2 ok");
    }
    
  }
