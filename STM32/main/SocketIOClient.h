#ifndef SOCKETIOCLIENT_H
#define SOCKETIOCLIENT_H

#include <ArduinoJson.h>
#include <Ethernet.h> // Include the Ethernet library

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
    EthernetClient client; // Ethernet client for communication
    struct EventCallback {
        const char* eventName;
        void (*callback)(const char* payload);
    };
    static constexpr size_t MAX_EVENTS = 10;
    EventCallback callbacks[MAX_EVENTS];
    size_t callbackCount;

    void handleIncomingMessage(const String& message);
};

#endif
