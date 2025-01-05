#include "OTWupdater.h"

OTWupdater::OTWupdater(uint32_t firmwareSlotAddress, uint32_t firmwareSize, uint32_t hashStorageAddress)
    : firmwareSlotAddress(firmwareSlotAddress), firmwareSize(firmwareSize), hashStorageAddress(hashStorageAddress) {}

bool OTWupdater::checkForNewFirmware() {
    // Check for update logic
    return true;
}

bool OTWupdater::validatePayloadForUpdate(const char* payload) {
    // Validate payload logic
    return true;
}

bool OTWupdater::performUpdate() {
    // Perform firmware update logic
    return true;
}


uint32_t OTWupdater::calculateChecksum(const uint8_t* data, uint32_t length) {
    uint32_t checksum = 0;
    for (uint32_t i = 0; i < length; i++) {
        checksum += data[i];
    }
    return checksum;
}
void OTWupdater::rebootToNewFirmware() {
    Serial.println("Rebooting to new firmware...");
    NVIC_SystemReset(); // STM32-specific system reset
}
void OTWupdater::writeFirmwareToFlash(const uint8_t* data, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        EEPROM.write(firmwareSlotAddress + i, data[i]);
    }
}
