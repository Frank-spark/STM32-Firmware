#include "EthernetSetup.h"

EthernetSetup::EthernetSetup(byte* mac, IPAddress ip) : mac(mac), ip(ip) {}

bool EthernetSetup::initialize() {
    const uint8_t maxRetries = 5;       // Maximum number of retries
    const uint32_t retryDelayMs = 1000; // Delay between retries in milliseconds
    uint8_t attempt = 0;

    Serial.println("[Info] Initializing Ethernet with static IP...");
    while (attempt < maxRetries) {
        Ethernet.begin(mac, ip); // Set static IP

        if (Ethernet.localIP() == ip) { // Verify that the static IP was assigned
            Serial.print("[Info] Ethernet initialized with static IP: ");
            Serial.println(Ethernet.localIP());
            return true; // Initialization successful
        }

        Serial.print("[Warning] Ethernet initialization failed. Retrying (");
        Serial.print(attempt + 1);
        Serial.print("/");
        Serial.print(maxRetries);
        Serial.println(")...");

        delay(retryDelayMs); // Wait before the next attempt
        attempt++;
    }

    Serial.println("[Error] Ethernet initialization failed after maximum retries.");
    return false; // Initialization failed
}

IPAddress EthernetSetup::getLocalIP() {
    return Ethernet.localIP();
}
