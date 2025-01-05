#include <Ethernet.h>
#include <Ethernet_Generic.h>
#include <EEPROM.h>
#include "OTWUpdate.h"
#include "SocketIOClient.h"

// Create instances of required classes
EthernetSetup ethernetSetup; // Declare the object
OTWUpdate updater(0x08080000, 0x40000, 0x080C0000); // Provide slot address, size, and hash storage address 
SocketIOClient socketIO("192.168.1.200", 3000); // Provide server IP and port

void setup() {
    Serial.begin(115200);
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

    // Check for over-the-wire updates
    if (updater.checkForNewFirmware()) {
        Serial.println("New firmware detected.");
        if (updater.performUpdate()) {
            Serial.println("Update successful. Rebooting...");
            updater.rebootToNewFirmware();
        } else {
            Serial.println("Update failed.");
        }
    }
}
