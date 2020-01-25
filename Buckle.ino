
/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <ESP32Servo.h>


BLECharacteristic *pCharacteristic;
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define SERVO_A0 5
#define SENSOR 15
#define BOARD_LED 13
#define ON_S "ON"
#define OFF_S "OFF"

Servo myServo;//creates servo
int zero_pos = 0;
int final_pos = 60;

// 

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("Long name works now");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setValue("Hello World says Neil");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
  myServo.attach(SERVO_A0);
  pinMode(BOARD_LED, OUTPUT);
}

void loop() {
  if(digitalRead(SENSOR)){
      digitalWrite(BOARD_LED, HIGH);
      Serial.println("Sensor Touched");
    }

  Serial.println(pCharacteristic->getValue().c_str());
  if(*pCharacteristic->getValue().c_str() == 1) {
  //if(pCharacteristic->getValue() == 1) {
    
    myServo.write(final_pos);
    //Serial.println("IN ON LOOP");
  }
  else{
    //Serial.println("IN NOT 1st LOOP");
  }
  if(*pCharacteristic->getValue().c_str() == 0) {
  //if(pCharacteristic->getValue() == 0) {
  
    //Serial.println("IN OFF LOOP");
    myServo.write(zero_pos);
  }
  else{
    //Serial.println("NOT IN 2nd LOOP");
   }
  delay(1000);
}
