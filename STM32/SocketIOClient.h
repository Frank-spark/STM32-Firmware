#ifndef SOCKETIOCLIENT_H
#define SOCKETIOCLIENT_H

#include <EthernetClient.h>
#include <ArduinoJson.h>

class SocketIOClient {
public:
    SocketIOClient(const char* serverIP, uint16_t serverPort, const char* endpoint);
    bool connect();
    void loop();
    void emit(const char* event, const JsonObject& payload);

private:
    EthernetClient client;
    const char* serverIP;
    uint16_t serverPort;
    const char* endpoint;
};

#endif // SOCKETIOCLIENT_H
