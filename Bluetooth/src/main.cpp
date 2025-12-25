/*
#include <Arduino.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it
#endif

BluetoothSerial SerialBT;

int led = Q0_0;


String message = "";
char char_received;
int Period = 250;
unsigned long Time;
int ledIndex = 0;



void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test");
  Serial.println("The device started, now you can pair it with Bluetooth!");

    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
  
}

void loop() {


  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }

  if (SerialBT.available()) {
    char_received = SerialBT.read();
    if (char_received != '\n') {
      message += String(char_received);
    } else {
      message.trim(); // clean up
      Serial.println("Received: " + message);
      if (message.equals("ON")) {
        digitalWrite(led,HIGH);
      }
      if(message.equals("OFF")){
        digitalWrite(led, LOW);
      }
      message = "";
    }
    Serial.write(char_received);
  }

  delay(20);
}
*/



#include <Arduino.h>
#include "BluetoothSerial.h"

// check bluetooth is enabled in its configuration
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it
#endif

BluetoothSerial SerialBT;

int relayPin = R0_0;    
int inputPin = I0_0;    


String message = "";
char char_received;
unsigned long lastSensorTime = 0;
uint16_t sensorValue = 0;
int sensorPeriod = 1000; 

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_PLC14");
  Serial.println("Classic Bluetooth PLC started. You can pair with the mobile app.");

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  pinMode(inputPin, INPUT);
}

void loop() {
  

  while (SerialBT.available()) {
    char_received = SerialBT.read();
    if (char_received != '\n') {
      message += String(char_received);
    } else {
      message.trim(); // remove spaces/newlines
      Serial.println("Received: " + message);


      if (message.equalsIgnoreCase("RELAY_ON")) {
        digitalWrite(relayPin, HIGH);
        Serial.println("Relay on command received");
      } 
      else if (message.equalsIgnoreCase("RELAY_OFF")) {
        digitalWrite(relayPin, LOW);
        Serial.println("Relay off command received");
        
      }
      else if (message.equalsIgnoreCase("READ_INPUT")) {
        Serial.println("INPUT read command received");
        int inputState = digitalRead(inputPin);
        SerialBT.println("INPUT:" + String(inputState));
      }
      else if (message.equalsIgnoreCase("READ_SENSOR")) {
        sensorValue++;
        Serial.println("Sensor value sent: " + String(sensorValue));
        SerialBT.println("SENSOR:" + String(sensorValue));
      }
      else{
        Serial.println("Invalid message received!");
      }

      message = ""; //reset message buffer
    }
  }

  
  // if (millis() - lastSensorTime > sensorPeriod) {
  //   sensorValue++;
  //   SerialBT.println("SENSOR:" + String(sensorValue)); 
  //   Serial.println("Sensor value sent: " + String(sensorValue));
  // }
}

