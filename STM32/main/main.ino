#include <Arduino.h>
#include "EthernetSetup.h"
#include "SocketIOClient.h"
#include "OTWUpdate.h"

// Ethernet configuration
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; // Replace with your MAC address
IPAddress ip(192, 168, 1, 177);                    // Replace with your static IP

// OTWUpdate configuration
const uint32_t firmwareSlotAddress = 0x08080000; // Flash address for the firmware slot
const uint32_t firmwareSize = 512 * 1024;        // Firmware slot size (512 KB)
const uint32_t hashStorageAddress = 0x0807F000; // Reserved address for firmware hash
const uint32_t expectedChecksum = 0x12345678;    // Example checksum for validation

// Global objects
EthernetSetup ethernet(mac, ip);
SocketIOClient socketIO("192.168.1.221", 5000, "/socket.io/?EIO=4");//replace with server IP
OTWUpdate otwUpdater(firmwareSlotAddress, firmwareSize, hashStorageAddress);

void setup() {
    Serial.begin(115200);

    // Initialize Ethernet
    if (ethernet.initialize()) {
        Serial.println("Ethernet initialized.");
        Serial.print("IP Address: ");
        Serial.println(ethernet.getLocalIP());
    } else {
        Serial.println("Ethernet initialization failed!");
        while (1); // Halt on failure
    }

    // Initialize Socket.IO
    socketIO.begin();

    // Perform firmware update check on boot
    Serial.println("Checking for firmware updates...");
    const uint8_t* firmwareData = nullptr; // Replace with actual mechanism to fetch firmware data
    const uint32_t firmwareSizeActual = 0; // Replace with actual size of firmware data
    if (otwUpdater.checkAndUpdateFirmware(firmwareData, firmwareSizeActual, expectedChecksum)) {
        Serial.println("Firmware updated successfully. Rebooting...");
        otwUpdater.jumpToFirmware(firmwareSlotAddress);
    } else {
        Serial.println("No firmware update required.");
    }
}

void loop() {
    // Handle Socket.IO communication
    socketIO.loop();
}
