#ifndef ETHERNETSETUP_H
#define ETHERNETSETUP_H

#include <stm32h7xx_hal.h>
#include <Ethernet.h> // Include Ethernet library

class EthernetSetup {
public:
    EthernetSetup(byte* mac, IPAddress ip); // Update the constructor
    bool initialize();
    IPAddress getLocalIP();
    
private:
    byte* mac;
    IPAddress ip;
};

#endif // ETHERNETSETUP_H