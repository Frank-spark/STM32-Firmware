#ifndef SOCKETIOCLIENT_H
#define SOCKETIOCLIENT_H

#include <ArduinoJson.h>
#include <Ethernet_Generic.h>



class SocketIOClient {
public:
    SocketIOClient(const char* serverIP, uint16_t serverPort);
    bool connect();
    void monitor();
    void on(const char* eventName, void (*callback)(const char* payload));
    void emit(const char* eventName, const char* message);

private:
    const char* serverIP;
    uint16_t serverPort;
    EthernetClient client;
};

#endif
