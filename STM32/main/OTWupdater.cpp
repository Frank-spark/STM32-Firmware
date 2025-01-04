#include "OTWUpdate.h"
#include "stm32h7xx_hal.h"
#include <Arduino.h>

// Constructor
OTWUpdate::OTWUpdate(uint32_t firmwareSlotAddress, uint32_t firmwareSize, uint32_t hashStorageAddress)
    : firmwareSlotAddress(firmwareSlotAddress), firmwareSize(firmwareSize), hashStorageAddress(hashStorageAddress) {}

// Perform Firmware Update
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

// Write Data to Flash Memory
void OTWUpdate::writeToFlash(uint32_t startAddress, const uint8_t* data, uint32_t size) {
    HAL_FLASH_Unlock(); // Unlock Flash for writing

    for (uint32_t i = 0; i < size; i += 4) {
        uint32_t word = *(uint32_t*)(data + i);
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, startAddress + i, word) != HAL_OK) {
            Serial.println("Error writing to Flash.");
            break;
        }
    }

    HAL_FLASH_Lock(); // Lock Flash after writing
}

// Validate Firmware via Checksum
bool OTWUpdate::validateFirmware(uint32_t startAddress, uint32_t size, uint32_t expectedChecksum) {
    uint32_t checksum = 0;
    for (uint32_t i = 0; i < size; i += 4) {
        checksum += *(uint32_t*)(startAddress + i);
    }
    Serial.printf("Computed Checksum: 0x%08X, Expected Checksum: 0x%08X\n", checksum, expectedChecksum);
    return (checksum == expectedChecksum);
}

// Store Firmware Hash in Flash
void OTWUpdate::storeFirmwareHash(uint32_t hash) {
    HAL_FLASH_Unlock(); // Unlock Flash for writing
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, hashStorageAddress, hash);
    HAL_FLASH_Lock(); // Lock Flash after writing
}

// Retrieve Stored Firmware Hash
uint32_t OTWUpdate::getStoredFirmwareHash() {
    return *(uint32_t*)hashStorageAddress;
}

// Jump to New Firmware
void OTWUpdate::jumpToFirmware(uint32_t address) {
    // Disable interrupts to safely switch execution
    __disable_irq();

    // Set the Main Stack Pointer
    __set_MSP(*(uint32_t*)address);

    // Set the Program Counter to the new firmware's Reset Handler
    void (*appEntry)(void) = (void (*)(void))(*(uint32_t*)(address + 4));
    appEntry();
}
