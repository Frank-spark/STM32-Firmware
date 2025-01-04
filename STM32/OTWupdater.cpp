#include "OTWUpdate.h"
#include <Crypto.h>
#include <SHA256.h>

OTWUpdate::OTWUpdate(uint32_t firmwareStartAddress, uint32_t hashStorageAddress)
    : firmwareStartAddress(firmwareStartAddress), hashStorageAddress(hashStorageAddress) {}

bool OTWUpdate::checkAndUpdateFirmware(const char* firmwarePath) {
    if (!validateFirmware(firmwarePath)) {
        Serial.println("Firmware validation failed.");
        return false;
    }

    File firmwareFile = SD.open(firmwarePath, FILE_READ);
    if (!firmwareFile) {
        Serial.println("Failed to open firmware file.");
        return false;
    }

    FlashIAP flash;
    flash.init();
    flash.erase(firmwareStartAddress, firmwareFile.size());

    uint8_t buffer[256];
    size_t bytesRead;
    uint32_t address = firmwareStartAddress;

    while ((bytesRead = firmwareFile.read(buffer, sizeof(buffer))) > 0) {
        if (flash.program(buffer, address, bytesRead) != 0) {
            Serial.println("Flash programming error.");
            firmwareFile.close();
            flash.deinit();
            return false;
        }
        address += bytesRead;
    }

    firmwareFile.close();
    flash.deinit();
    return true;
}

bool OTWUpdate::validateFirmware(const char* firmwarePath) {
    File firmwareFile = SD.open(firmwarePath, FILE_READ);
    if (!firmwareFile) {
        Serial.println("Failed to open firmware file for validation.");
        return false;
    }

    SHA256 sha256;
    uint8_t computedHash[32];
    while (firmwareFile.available()) {
        uint8_t buffer[256];
        size_t bytesRead = firmwareFile.read(buffer, sizeof(buffer));
        sha256.update(buffer, bytesRead);
    }
    sha256.finalize(computedHash, sizeof(computedHash));
    firmwareFile.close();

    uint8_t storedHash[32];
    FlashIAP flash;
    flash.read(hashStorageAddress, storedHash, sizeof(storedHash));

    return memcmp(computedHash, storedHash, sizeof(computedHash)) != 0;
}

void OTWUpdate::storeHash(const uint8_t* hash) {
    FlashIAP flash;
    flash.erase(hashStorageAddress, 32);
    flash.program(hash, hashStorageAddress, 32);
}

void OTWUpdate::jumpToApplication() {
    void (*appReset)(void) = (void (*)(void))(*((uint32_t*)(firmwareStartAddress + 4)));
    appReset();
}
