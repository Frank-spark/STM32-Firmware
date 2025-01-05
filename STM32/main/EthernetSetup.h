#ifndef ETHERNETSETUP_H
#define ETHERNETSETUP_H

#include <Ethernet.h>
#include <Ethernet_Generic.h>


class EthernetSetup {
public:
    EthernetSetup();
    bool initialize();
    IPAddress getLocalIP();




private:
    byte mac[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
    IPAddress ip = IPAddress(192, 168, 1, 100);
};
extern EthernetSetup ethernetSetup;
#endif // ETHERNETSETUP_H
