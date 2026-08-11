// Required libraries
#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <ESP32Servo.h>

// For the servo
Servo servo1;
int servoPos = 1;

// For the RSSI
int rssi = 0;
// RSSI scan time is in seconds 
// Scan time cannot go lower than 1 second
int scanTime = 1;  
int counter = 0;
BLEScan *pBLEScan;

// Alter this based on a randomly generated UUID 
// UUID has to be the same as the one in the nRF Connect application
// Target UUID that the ESP32 will scan for
static BLEUUID targetUUID("67208c5b-1610-49de-b477-aa1bfd49a306"); // *Change this code based on your generated UUID

// Network credentials
// This can be altered, just keep in mind the SSID (wifi name) and the password
const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

WiFiServer server(80);

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        // Checks if found device advertises target UUID
        if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(targetUUID)) {
            rssi = advertisedDevice.getRSSI();
            Serial.print("Target found! RSSI: ");
            Serial.println(rssi);
        }
    }
}; 

void setup() {
  // Syntax that declares what pin the servo is attached to
  servo1.attach(22); // *Change this code based on the pin your servo is attached to
  // Begins serial monitor at 115200
  // Serial monitor can remain open and can be used for debugging
  Serial.begin(115200);

  // Setup for the Access Point
  Serial.print("Setting AP (Access Point)...");
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Setup for the BLE
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
  
  // Sets servo to the 0 degrees or initial position
  servo1.write(0);
  // Begins server
  server.begin();
}

void loop() {
  // Listens for Matlab
  WiFiClient client = server.available();   

  // Program starts if Matlab is detected
  if (client) { 
    Serial.println("MATLAB Connected!");
    
    while (client.connected()) {
      if(client.read() == 'd'){
        Serial.println("Received a message----------------------------------------------------");
        servoPos++;
        switch (servoPos){
          case 1:
            Serial.println("At right----------------------------------------------------");
            servo1.write(0);
            break;
          case 2:
            Serial.println("At forward----------------------------------------------------");
            servo1.write(90);
            break;
          case 3:
            Serial.println("At left----------------------------------------------------");
            servo1.write(180);
            break;
          default:
            Serial.println("One cycle complete----------------------------------------------------");
            servo1.write(0);
            // disconnects the Arduino from Matlab
            client.stop();
            break;
        }
      } else{ 
        // Scans for the mobile phone or BLE
        pBLEScan->start(scanTime, false);
        pBLEScan->clearResults();

        // Sends RSSI to Matlab
        client.println(rssi); 
        Serial.print("Sent RSSI to MATLAB: ");
        Serial.println(rssi);
      }
      // Brief pause to prevent flooding 
      delay(10); 
    }

    client.stop();
    Serial.println("MATLAB disconnected.");
  }
}
