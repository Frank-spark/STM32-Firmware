#include "EthernetSetup.h"

EthernetSetup::EthernetSetup(byte* mac, IPAddress ip) : mac(mac), ip(ip) {}

bool EthernetSetup::initialize() {
    Serial.println("[Info] Initializing Ethernet...");
    Ethernet.begin(mac, ip);

    if (Ethernet.localIP() == INADDR_NONE) {
        Serial.println("[Error] Failed to configure Ethernet using static IP.");
        return false;
    }

    Serial.print("[Info] Ethernet initialized with IP: ");
    Serial.println(Ethernet.localIP());
    return true;
}

IPAddress EthernetSetup::getLocalIP() {
    return Ethernet.localIP();
}