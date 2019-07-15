 /*
  PubNub sample client

  This sample client will publish raw (JSON pre-encoded) PubNub messages.

  Circuit:
  * Ethernet shield attached to pins 10, 11, 12, 13
  * (Optional.) Analog sensors on pins A0 to A5.
  * (Optional.) LED on pin 9 for success indication.

  created 23 October 2012
  by Petr Baudis

  https://github.com/pubnub/pubnub-api/tree/master/arduino
  This code is in the public domain.
  */

#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubNub.h>
#include "pitches.h"
#include "string.h"

/* **************************************************
 The mac value MUST be unique. It must NOT clash
 with any other machine on the network
 converse and collaborate with people in the class
 to pick an appropriate unique value - one good
 choice would be your kit number for example
*/

SoftwareSerial serial_connection(10,11);
TinyGPSPlus gps;


const int buttonPin = 2;  
int buttonState = 0;
const int ledPin =  13; 

int melody [] = {NOTE_C4, NOTE_G3};
int noteDurations [] = {4,8};

byte mac[] = {0x00, 0x0E, 0xEF, 0x00, 0x00, 0x34}; 
// do not change the 0x00 0x0E 0xEF 0x00 0x00 prefix


char pubkey[] = "pub-c-2c220cbd-828d-4ead-b86d-07533e2034c9";
char subkey[] = "sub-c-71645706-99ca-11e9-9ac8-0ed882abeb26";
char channel[] = "iotchannel";

void setup()
{
  

  Serial.begin(9600);
  serial_connection.begin(9600);
  Serial.println("Serial set up");

  
  pinMode(buttonPin, INPUT);
  pinMode(ledPin,OUTPUT);

  PubNub.begin(pubkey, subkey);
  Serial.println("PubNub set up");
}



void loop()
{

  
  
  Ethernet.maintain();

  EthernetClient *client;


buttonState = digitalRead(buttonPin);
if (buttonState == HIGH) {
  digitalWrite(ledPin, HIGH);


    
    for (int thisNote = 0; thisNote <2; thisNote++){ 
      int noteDuration = 1000 / noteDurations [thisNote];
      tone (8,melody [thisNote], noteDuration);

      int pauseBetweenNotes = noteDuration * 1.30;
      delay (pauseBetweenNotes);
      noTone(2);
    }
    
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
  

 while(serial_connection.available())
  {
    gps.encode(serial_connection.read());
  }

  if(gps.location.isUpdated()  && buttonState == HIGH)
{

  Serial.println("Satellite Count:");
  Serial.println(gps.location.lat(),3);
  
 // char msg[64] = "{\"eon\":{\"sensor\":";
//char latt[64] = "{\0\":";


 char msg[24] = "{\"eon\":{\"latitude\":"; //[24] = length to save for lenght of string
 char mslng[17] = "{\"0";
 
 char msg2[24] = "{\"longitude\":";
 char msglat[13] = "{\"0";


 
    //sprintf(msg + strlen(msg), msa);
   //dtostrf(gps.location.lat(),2,2,msa);
   //strcat(msg, "}}");
// {"eon":{"sensor":{"eon":{"sensor":}}




  dtostrf(gps.location.lat(),2,2,msglat);
  dtostrf(gps.location.lng(),3,3,mslng);
  //dtostrf(gps.location.lat(),2,2,msg);


Serial.println(msg);

  
  
   //sprintf(msg2 + strlen(msg2), msglat);
   sprintf(msg + strlen(msg), msglat);
    
   strcat (msg, "}}");


  
// 51.49-2.59}}

//Serial.println(prefix);
    
  //dtostrf(gps.location.lat(),2,2,latt);
  //sprintf(msg + strlen(msg), latt);
 // strcat(msg, "}}");


while (!Ethernet.begin(mac)) 
  {
    Serial.println("ethernet setup eroor");
    delay(10);
  }




  Serial.print("publishing message: ");
  Serial.println(msg);
  client = PubNub.publish(channel, msg);
  if (!client) {
    Serial.println("publishing error");
  } else {
  digitalWrite(ledPin, LOW);
    client->stop();
  }

  
}

delay(100);

}
