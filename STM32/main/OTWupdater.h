#ifndef OTWUPDATE_H
#define OTWUPDATE_H

#include <stdint.h>

class OTWUpdate {
public:
    OTWUpdate(uint32_t firmwareSlotAddress, uint32_t firmwareSize, uint32_t hashStorageAddress);
    bool checkAndUpdateFirmware(const uint8_t* firmwareData, uint32_t size, uint32_t expectedChecksum);

private:
    uint32_t firmwareSlotAddress;
    uint32_t firmwareSize;
    uint32_t hashStorageAddress;

    void writeToFlash(uint32_t startAddress, const uint8_t* data, uint32_t size);
    bool validateFirmware(uint32_t startAddress, uint32_t size, uint32_t expectedChecksum);
    void storeFirmwareHash(uint32_t hash);
    uint32_t getStoredFirmwareHash();
    void jumpToFirmware(uint32_t address);
};

#endif // OTWUPDATE_H
