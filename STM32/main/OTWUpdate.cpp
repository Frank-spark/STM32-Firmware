#include "OTWUpdate.h"
#include "stm32h7xx_hal.h"

OTWUpdate::OTWUpdate(uint32_t firmwareSlotAddress, uint32_t firmwareSize, uint32_t hashStorageAddress)
    : firmwareSlotAddress(firmwareSlotAddress), firmwareSize(firmwareSize), hashStorageAddress(hashStorageAddress) {}

bool OTWUpdate::checkAndPerformUpdate() {
    if (checkForNewFirmware()) {
        Serial.println("[Info] New firmware detected.");
        if (performUpdate()) {
            Serial.println("[Info] Update successful. Rebooting...");
            rebootToNewFirmware();
            return true; // Successfully handled update
        } else {
            Serial.println("[Error] Update failed.");
            return false;
        }
    }
    return false; // No update found
}

void OTWUpdate::writeFirmwareToFlash(const uint8_t* data, uint32_t length) {
    HAL_FLASH_Unlock();

    for (uint32_t i = 0; i < length; i += 8) { // Program in 64-bit (8-byte) chunks
        uint64_t flashWord = 0;

        // Combine 8 bytes into a 64-bit value
        for (uint8_t j = 0; j < 8 && (i + j) < length; j++) {
            flashWord |= ((uint64_t)data[i + j] << (j * 8));
        }

        uint32_t address = firmwareSlotAddress + i;
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, address, flashWord) != HAL_OK) {
            Serial.println("[Error] Flash programming failed.");
            HAL_FLASH_Lock();
            return;
        }
    }

    HAL_FLASH_Lock();
    Serial.println("[Info] Firmware written to Flash.");
}


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

