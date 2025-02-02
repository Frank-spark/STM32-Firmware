#ifndef OTWUPDATE_H
#define OTWUPDATE_H

#include <stdint.h>
#include <Arduino.h>

class OTWUpdate {
public:
    OTWUpdate(uint32_t firmwareSlotAddress, uint32_t firmwareSize, uint32_t hashStorageAddress);
    bool checkAndPerformUpdate(); // Encapsulates the update process
    uint32_t calculateChecksum(const uint8_t* data, uint32_t size);
    void rebootToNewFirmware();

private:
    uint32_t firmwareSlotAddress;
    uint32_t firmwareSize;
    uint32_t hashStorageAddress;

    bool checkForNewFirmware();
    bool validatePayloadForUpdate(const char* payload);
    bool performUpdate();
    void writeToFlash(uint32_t startAddress, const uint8_t* data, uint32_t size);
    void writeFirmwareToFlash(const uint8_t* data, uint32_t length); // Correctly declared here
    bool validateFirmware(uint32_t startAddress, uint32_t size, uint32_t expectedChecksum);
    void storeFirmwareHash(uint32_t hash);
    uint32_t getStoredFirmwareHash();
    void jumpToFirmware(uint32_t address);
    bool checkAndUpdateFirmware(const uint8_t* firmwareData, uint32_t size, uint32_t expectedChecksum);
  
};

#endif // OTWUPDATE_H
