#include "SocketIOClient.h"

SocketIOClient::SocketIOClient(const char* serverIP, uint16_t serverPort)
    : serverIP(serverIP), serverPort(serverPort) {}

bool SocketIOClient::connect() {
    return client.connect(serverIP, serverPort); // Use the client object
}

void SocketIOClient::monitor() {
    if (client.available()) {
        // Handle incoming messages
        char c = client.read();
        Serial.print(c); // Debugging: Print incoming data
    }
}

void SocketIOClient::on(const char* eventName, void (*callback)(const char* payload)) {
    // Register an event callback
}

void SocketIOClient::emit(const char* eventName, const char* message) {
    DynamicJsonDocument doc(512);
    doc["event"] = eventName;
    doc["data"] = message;
    serializeJson(doc, client); // Use the client object to send data
    client.flush();
}
