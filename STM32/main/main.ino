#include <Arduino.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include "OTWUpdate.h"
#include "SocketIOClient.h"
#include "EthernetSetup.h"

// Ethernet configuration
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; // Replace with your MAC address
IPAddress ip(192, 168, 1, 177);                    // Replace with your static IP

// Create instances of required classes
EthernetSetup ethernetSetup(mac, ip); // Declare the object with parameters
OTWUpdate updater(0x08080000, 0x40000, 0x080C0000); // Provide slot address, size, and hash storage address 
SocketIOClient socketIO("192.168.1.200", 3000); //

void setup() {

  
    Serial.begin(115200);
    updater.checkAndPerformUpdate();
    while (!Serial);
    
    // Initialize Ethernet
    if (!ethernetSetup.initialize()) {
        Serial.println("Failed to initialize Ethernet");
        while (true); // Halt execution
    }

    // Connect to the socket server
    if (socketIO.connect()) {
        Serial.println("SocketIO connected successfully.");
    } else {
        Serial.println("Failed to connect to SocketIO server.");
    }
}

void loop() {
    // Monitor socket communication
    socketIO.monitor();

    
}
