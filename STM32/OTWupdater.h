#ifndef OTWUPDATE_H
#define OTWUPDATE_H

#include <SD.h>
#include <FlashIAP.h>

class OTWUpdate {
public:
    OTWUpdate(uint32_t firmwareStartAddress, uint32_t hashStorageAddress);
    bool checkAndUpdateFirmware(const char* firmwarePath);
    bool validateFirmware(const char* firmwarePath);
    void storeHash(const uint8_t* hash);
    void jumpToApplication();

private:
    uint32_t firmwareStartAddress;
    uint32_t hashStorageAddress;
};

#endif // OTWUPDATE_H
