#ifndef SOCKETIOCLIENT_H
#define SOCKETIOCLIENT_H

#include <EthernetClient.h>
#include <ArduinoJson.h>

class SocketIOClient {
public:
    SocketIOClient(const char* serverIP, uint16_t serverPort, const char* endpoint);
    void begin();
    void loop();
    String readEvent();
    bool available();

private:
    EthernetClient client;
    const char* serverIP;
    uint16_t serverPort;
    const char* endpoint;

    void handleSocketEvents();
    void rebootSTM32();
};

#endif // SOCKETIOCLIENT_H
