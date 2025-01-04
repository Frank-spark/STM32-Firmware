

---

### **Project: STM32 Nucleo-H723ZG Firmware with Ethernet, Socket.IO, and OTW Updates**

#### **Overview**
This project implements a firmware for the STM32 Nucleo-H723ZG development board that integrates:
- **Ethernet communication** for network connectivity.
- **Socket.IO** for real-time server interaction.
- **Over-the-Wire (OTW) updates** for seamless firmware upgrades via an SD card.

The project ensures reliable communication and update mechanisms, providing an extendable framework for IoT or industrial applications.

---

#### **Features**
1. **Ethernet Communication**:
   - Configurable static IP or DHCP.
   - RMII interface setup for onboard Ethernet PHY.

2. **Socket.IO Client**:
   - Real-time communication with a server.
   - JSON-based data exchange for control and monitoring.

3. **Over-the-Wire Firmware Updates**:
   - Firmware update using an SD card.
   - Hash validation to avoid unnecessary updates.
   - Automatic reboot into the updated application firmware.

---

#### **Requirements**
- **Hardware**:
  - STM32 Nucleo-H723ZG development board.
  - Ethernet cable and a connected network.
  - SD card for storing the firmware binary.
- **Software**:
  - STM32CubeIDE (or Arduino IDE for simpler integration).
  - STM32 HAL and LL libraries (bundled with STM32CubeMX).

---

#### **Pin Configuration**
- **Ethernet RMII Interface**:
  - `PA1`: RMII Reference Clock
  - `PA2`: RMII MDIO
  - `PA7`: RMII TXD0
  - `PB11`: RMII TXD1
  - `PB12`: RMII TXEN
  - `PC1`: RMII MDC
  - `PC4`: RMII RXD0
  - `PC5`: RMII RXD1
  - `PG11`: RMII CRS_DV
- **SDMMC Interface**:
  - `PC8`: SDMMC1 D0
  - `PC9`: SDMMC1 D1
  - `PC10`: SDMMC1 D2
  - `PC11`: SDMMC1 D3
  - `PC12`: SDMMC1 CLK
  - `PD2`: SDMMC1 CMD

---

#### **Setup Instructions**
1. **Clone or Download the Repository**:
   - Ensure all required source files (`OTWUpdate`, `EthernetSetup`, and `SocketIOClient`) are available.

2. **Configure the Ethernet**:
   - Update the `mac[]` and `ip` in `main.cpp` to match your network configuration.

3. **Prepare the SD Card**:
   - Place the firmware binary (`firmware.bin`) in the root directory of the SD card.

4. **Compile and Flash**:
   - Use STM32CubeIDE or Arduino IDE to compile and upload the firmware to the STM32 board.

5. **Connect to Socket.IO Server**:
   - Ensure the server is running at the configured `serverIP` and `serverPort`.

6. **Monitor the Serial Output**:
   - Use a serial monitor (e.g., Arduino Serial Monitor) to debug and verify firmware behavior.

---

#### **Usage**
1. **Ethernet Communication**:
   - The board initializes Ethernet on startup.
   - Displays the assigned IP address via the serial console.

2. **Socket.IO Real-Time Updates**:
   - Communicates with the server for event-based data exchange.
   - Emits and receives JSON payloads.

3. **Over-the-Wire Firmware Updates**:
   - Checks the SD card for a new firmware file (`firmware.bin`).
   - Validates the firmware using a hash and applies updates if required.
   - Reboots automatically after a successful update.

---

#### **Project Structure**
```
STM32_Firmware_Project/
+-- OTWUpdate.h           # Header for OTW firmware updates
+-- OTWUpdate.cpp         # Implementation of OTW updates
+-- EthernetSetup.h       # Header for Ethernet initialization
+-- EthernetSetup.cpp     # Implementation of Ethernet setup
+-- SocketIOClient.h      # Header for Socket.IO client
+-- SocketIOClient.cpp    # Implementation of Socket.IO client
+-- main.cpp              # Main application logic
+-- README.md             # Project documentation
```

---

#### **Future Enhancements**
- Add support for multiple endpoints and events in the Socket.IO client.
- Implement dynamic network configuration (e.g., using a web server).
- Enhance OTW update mechanism with fallback capabilities.

---

#### **License**
This project is open-source and licensed under the [MIT License](https://opensource.org/licenses/MIT).

---

