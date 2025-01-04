#include "SocketIOClient.h"
#include "stm32h7xx_hal.h"
#include <Arduino.h>

// Constructor
SocketIOClient::SocketIOClient(const char* serverIP, uint16_t serverPort, const char* endpoint)
    : serverIP(serverIP), serverPort(serverPort), endpoint(endpoint) {}

// Begin connection
void SocketIOClient::begin() {
    if (!client.connect(serverIP, serverPort)) {
        Serial.println("Socket.IO connection failed!");
        return;
    }

    client.print("GET ");
    client.print(endpoint);
    client.println(" HTTP/1.1");
    client.println("Upgrade: websocket");
    client.println("Connection: Upgrade");
    client.println("Host: " + String(serverIP) + ":" + String(serverPort));
    client.println("Sec-WebSocket-Version: 13");
    client.println("Sec-WebSocket-Key: random_key_here");
    client.println();

    Serial.println("Socket.IO connected.");
}

// Main loop
void SocketIOClient::loop() {
    if (client.available()) {
        handleSocketEvents();
    }
}

// Check for incoming events
bool SocketIOClient::available() {
    return client.available() > 0;
}

// Read an incoming event
String SocketIOClient::readEvent() {
    String event = client.readStringUntil('\n');
    return event;
}

// Handle specific events
void SocketIOClient::handleSocketEvents() {
    String event = readEvent();

    if (event == "reboot") {
        Serial.println("Reboot command received via Socket.IO.");
        rebootSTM32();
    } else if (event == "updateFirmware") {
        Serial.println("Firmware update command received. Rebooting...");
        rebootSTM32();
    }
}

// Reboot the STM32
void SocketIOClient::rebootSTM32() {
    NVIC_SystemReset(); // Perform a system reset
}
