#include "SocketIOClient.h"

SocketIOClient::SocketIOClient(const char* serverIP, uint16_t serverPort)
    : serverIP(serverIP), serverPort(serverPort), callbackCount(0) {}

bool SocketIOClient::connect() {
    if (Ethernet.localIP() == INADDR_NONE) {
        Serial.println("[Error] Ethernet not initialized. Call EthernetSetup::initialize first.");
        return false;
    }
    if (!client.connect(serverIP, serverPort)) {
        Serial.println("[Error] Connection to server failed.");
        return false;
    }
    Serial.println("[Info] Connected to server.");
    return true;
}

void SocketIOClient::monitor() {
    while (client.available()) {
        String message = client.readStringUntil('\n'); // Read incoming message
        Serial.println("[Debug] Incoming Message: " + message); // Debugging
        handleIncomingMessage(message); // Process the message
    }
}

void SocketIOClient::on(const char* eventName, void (*callback)(const char* payload)) {
    if (callbackCount < MAX_EVENTS) {
        callbacks[callbackCount].eventName = eventName;
        callbacks[callbackCount].callback = callback;
        callbackCount++;
        Serial.print("[Info] Registered event: ");
        Serial.println(eventName);
    } else {
        Serial.println("[Error] Maximum number of event callbacks reached.");
    }
}

void SocketIOClient::emit(const char* eventName, const char* message) {
    DynamicJsonDocument doc(512);
    doc["event"] = eventName;
    doc["data"] = message;
    serializeJson(doc, client); // Send the event as JSON
    client.flush();
    Serial.println("[Debug] Event emitted: " + String(eventName));
}

void SocketIOClient::handleIncomingMessage(const String& message) {
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, message);

    if (error) {
        Serial.println("[Error] Failed to parse incoming message.");
        return;
    }

    const char* event = doc["event"];
    const char* data = doc["data"];

    if (event && data) {
        for (size_t i = 0; i < callbackCount; i++) {
            if (strcmp(callbacks[i].eventName, event) == 0) {
                callbacks[i].callback(data);
                return;
            }
        }
        Serial.println("[Info] No callback registered for event: " + String(event));
    } else {
        Serial.println("[Error] Malformed message received.");
    }
}
