

//------------------------enthernet-----------------------------------
void ethernet() {
  Serial.print("connecting ethernet :");
  Ethernet.begin(mac,IP);
  while(Ethernet.linkStatus()!=LinkON){                                       //https://www.arduino.cc/en/Reference/EthernetLinkStatus
    Serial.println(Ethernet.linkStatus());
    for(int i=1; i<=20; i++){
        Serial.print("Failed ethernet-waiting for 3s, times: " + String(i));
        delay(3000);
        Ethernet.begin(mac,IP);
        delay(200);
        if (Ethernet.linkStatus() == Unknown) {
            Serial.println("   check for your devices ??? ");
          }
        else if (Ethernet.linkStatus() == LinkON) {
            Serial.println("   enthernet connected Link status: On \n");
            break;
         }
        else if (Ethernet.linkStatus() == LinkOFF) {
            Serial.println("   Failed ethernet: link status = off \n");
        }
        if(i>=20){
            Serial.println("reset because Ethernet misstake ");
            delay(500);
            resetFunc();                                // ham reset
          } 
      }// end for
    } // end while
    
  Serial.println("ethernet connected");
  Serial.print("IP Address        : ");
  Serial.println(Ethernet.localIP());
  Serial.print("Subnet Mask       : ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("Default Gateway IP: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("DNS Server IP     : ");
  Serial.println(Ethernet.dnsServerIP()); 
  } //end ethetnet

// --------------------------MQTT------------------------------
  
void MQTTreconnect() {
    int j=0;
    while(!client.connected()) {
        j++;
        Serial.println("\nProgram of MQTT");
        Serial.println("count for connect MQTT: "+ String(j));
        Serial.print("connecting again MQTT...");
        if(client.connect("arduinoClient")) {
            Serial.println("client connected MQTT");
            client.subscribe("room/lamp");
            client.subscribe("room/relay2");
          }
        else {
            Serial.print("failed, src=" + String(client.state()) + String("\ntry again for 4s \n"));
            delay(4000);
          }
        if(j>=15) {
            Serial.println("reset board because MQTT misstake !!");
            delay(500);
            resetFunc();
          }
        
    } //end while
  } //end MQTTconnect


void checkethernet() {
   int h=0;
   while(1){  
        h++;
        Serial.print("");
    if (Ethernet.linkStatus() == Unknown) {
        Serial.println("- check for your devices ??? ");
        delay(500);
        }
     else if (Ethernet.linkStatus() == LinkON) {
        Serial.println("");
        delay(500);
        break;
        }
    else if (Ethernet.linkStatus() == LinkOFF) {
        Serial.println("\n Failed ethernet: link status = off");
        delay(500);
        Ethernet.begin(mac,IP);
         }
     if (h>=30){
        Serial.println("RESET board because E > 30");
        delay(500);
        resetFunc();
      }
  }
}
