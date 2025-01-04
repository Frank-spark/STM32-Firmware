#ifndef ETHERNETSETUP_H
#define ETHERNETSETUP_H

#include <stm32h7xx_hal.h>

class EthernetSetup {
public:
    EthernetSetup();
    bool initialize();
    IPAddress getLocalIP();

private:
    void configurePins();
};

#endif // ETHERNETSETUP_H
