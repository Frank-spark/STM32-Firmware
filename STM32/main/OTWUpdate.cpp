#include "OTWupdate.h"
#include "stm32h7xx_hal.h"
#include <Arduino.h>
#include <EEPROM.h>
OTWUpdate::OTWUpdate(uint32_t firmwareSlotAddress, uint32_t firmwareSize, uint32_t hashStorageAddress)
    : firmwareSlotAddress(firmwareSlotAddress), firmwareSize(firmwareSize), hashStorageAddress(hashStorageAddress) {}

bool OTWUpdate::checkAndUpdateFirmware(const uint8_t* firmwareData, uint32_t size, uint32_t expectedChecksum) {
    // Ensure size is within firmware slot limits
    if (size > firmwareSize) {
        Serial.println("Firmware size exceeds allocated Flash slot.");
        return false;
    }
    // Check if the firmware hash has changed
    uint32_t storedHash = getStoredFirmwareHash();
    if (storedHash == expectedChecksum) {
        Serial.println("Firmware is up-to-date. No update required.");
        return false;
    }
    Serial.println("Writing firmware to Flash...");
    writeToFlash(firmwareSlotAddress, firmwareData, size);
    Serial.println("Validating firmware...");
    if (!validateFirmware(firmwareSlotAddress, size, expectedChecksum)) {
        Serial.println("Firmware validation failed.");
        return false;
    }
    // Store the new firmware hash
    storeFirmwareHash(expectedChecksum);
    Serial.println("Firmware validation successful. Jumping to new firmware...");
    jumpToFirmware(firmwareSlotAddress);
    return true;
}

// Check for new firmware
bool OTWUpdate::checkForNewFirmware() {
    // Implementation for checking new firmware
    return false;
}

bool OTWUpdate::validatePayloadForUpdate(const char* payload) {
    // Validate payload logic
    return true;
}

bool OTWUpdate::performUpdate() {
    // Perform firmware update logic
    return true;
}

uint32_t OTWUpdate::calculateChecksum(const uint8_t* data, uint32_t length) {
    uint32_t checksum = 0;
    for (uint32_t i = 0; i < length; i++) {
        checksum += data[i];
    }
    return checksum;
}

void OTWUpdate::rebootToNewFirmware() {
    Serial.println("Rebooting to new firmware...");
    NVIC_SystemReset(); // STM32-specific system reset
}

void OTWUpdate::writeFirmwareToFlash(const uint8_t* data, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        EEPROM.write(firmwareSlotAddress + i, data[i]);
    }
}
