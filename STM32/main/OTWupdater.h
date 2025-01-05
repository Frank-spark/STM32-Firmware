#ifndef OTWUPDATER_H
#define OTWUPDATER_H

#include <Ethernet_Generic.h>
#include <ArduinoJson.h>
bool performUpdate();

class OTWupdater {
public:
    OTWupdater(const char* serverIP, uint16_t serverPort);
    void checkForUpdate();
    void performUpdate();
    void rebootToNewFirmware();
private:
    EthernetClient client;
    const char* serverIP;
    uint16_t serverPort;
};

#endif
