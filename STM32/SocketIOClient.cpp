#include "SocketIOClient.h"

SocketIOClient::SocketIOClient(const char* serverIP, uint16_t serverPort, const char* endpoint)
    : serverIP(serverIP), serverPort(serverPort), endpoint(endpoint) {}

bool SocketIOClient::connect() {
    if (!client.connect(serverIP, serverPort)) {
        Serial.println("Socket.IO connection failed.");
        return false;
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

    return true;
}

void SocketIOClient::loop() {
    if (client.available()) {
        String response = client.readStringUntil('\n');
        Serial.println("Socket.IO response: " + response);
    }
}

void SocketIOClient::emit(const char* event, const JsonObject& payload) {
    DynamicJsonDocument doc(1024);
    JsonArray array = doc.to<JsonArray>();
    array.add(event);
    array.add(payload);

    String message;
    serializeJson(doc, message);

    client.print(message);
}
