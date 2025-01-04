#include <Arduino.h>
#include "EthernetSetup.h"
#include "SocketIOClient.h"
#include "OTWUpdate.h"

// Ethernet configuration
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // Replace with your MAC address
IPAddress ip(192, 168, 1, 177); // Replace with your static IP

// Socket.IO configuration
const char* serverIP = "192.168.1.221"; // Replace with your Socket.IO server IP
uint16_t serverPort = 5000;            // Replace with your Socket.IO server port
const char* endpoint = "/socket.io/?EIO=4";

// OTW update configuration
const uint32_t firmwareStartAddress = 0x08020000; // Flash memory address for application firmware
const uint32_t hashStorageAddress = 0x0801F000;   // Flash memory address to store firmware hash
const char* firmwarePath = "/firmware.bin";       // Path to firmware on SD card

// Global instances
EthernetSetup ethernet;
SocketIOClient socketIO(serverIP, serverPort, endpoint);
OTWUpdate otwUpdater(firmwareStartAddress, hashStorageAddress);

void setup() {
    Serial.begin(115200);
    while (!Serial);

    // Initialize Ethernet
    if (ethernet.initialize()) {
        Serial.println("Ethernet initialized.");
        Serial.print("IP Address: ");
        Serial.println(ethernet.getLocalIP());
    } else {
        Serial.println("Ethernet initialization failed!");
        while (1); // Halt on failure
    }

    // Connect to Socket.IO server
    if (socketIO.connect()) {
        Serial.println("Connected to Socket.IO server.");
    } else {
        Serial.println("Socket.IO connection failed!");
        while (1); // Halt on failure
    }

    // Perform OTW firmware update if required
    if (otwUpdater.checkAndUpdateFirmware(firmwarePath)) {
        Serial.println("Firmware update complete. Restarting...");
        otwUpdater.jumpToApplication(); // Restart the application
    } else {
        Serial.println("Firmware is up-to-date or update failed.");
    }
}

void loop() {
    // Handle Socket.IO communication
    socketIO.loop();

    // Add periodic checks or logic here if needed
    delay(100); // Reduce CPU usage
}
