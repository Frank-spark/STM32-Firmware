

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
├── OTWUpdate.h           # Header for OTW firmware updates
├── OTWUpdate.cpp         # Implementation of OTW updates
├── EthernetSetup.h       # Header for Ethernet initialization
├── EthernetSetup.cpp     # Implementation of Ethernet setup
├── SocketIOClient.h      # Header for Socket.IO client
├── SocketIOClient.cpp    # Implementation of Socket.IO client
├── main.cpp              # Main application logic
└── README.md             # Project documentation
```
Using the static IP as the identifier simplifies the system and ensures each STM32 device is uniquely identified based on its assigned network address. Here’s an updated explanation for the Socket.IO server maintainers to implement firmware hosting using static IPs as the identifier.

---

### **Firmware Hosting for STM32 Devices Using Static IPs**

#### **Overview**
The Socket.IO server will identify STM32 devices based on their **static IP addresses**. Each device will:
1. Request its firmware update by sending its static IP.
2. Download the appropriate firmware file based on its IP.

This approach ensures that each STM32 device receives the correct firmware version for updates.

---

#### **Implementation on the Socket.IO Server**

1. **Directory Structure for Firmware Files**:
   - Organize firmware files using static IPs as directory names.
   - Example directory structure:
     ```
     firmware/
     ├── 192.168.1.100/
     │   ├── firmware_v1.0.bin
     │   └── firmware_v1.1.bin
     ├── 192.168.1.101/
     │   ├── firmware_v2.0.bin
     │   └── firmware_v2.1.bin
     └── 192.168.1.102/
         ├── firmware_v3.0.bin
         └── firmware_v3.1.bin
     ```

2. **Endpoint for Firmware Requests**:
   - Define a REST API endpoint to serve firmware files based on the device's static IP.
   - Example API route:
     ```
     GET /firmware/:ip/latest
     ```
   - The `ip` parameter corresponds to the device’s static IP.

3. **Serving Firmware Based on IP**:
   - Use the IP from the request to locate the firmware directory.
   - Example in Node.js (Express):
     ```javascript
     const express = require('express');
     const path = require('path');
     const app = express();

     // Define firmware directory
     const firmwareDir = path.join(__dirname, 'firmware');

     // Endpoint to get the latest firmware based on IP
     app.get('/firmware/:ip/latest', (req, res) => {
         const ip = req.params.ip.replace(/\./g, '_'); // Sanitize IP for folder name
         const devicePath = path.join(firmwareDir, ip);

         // Find the latest firmware file
         const fs = require('fs');
         fs.readdir(devicePath, (err, files) => {
             if (err || !files || files.length === 0) {
                 res.status(404).send('Firmware not found');
                 return;
             }

             // Sort files to find the latest version (assumes versioned filenames)
             const latestFirmware = files.sort().reverse()[0];
             const firmwarePath = path.join(devicePath, latestFirmware);

             // Send the firmware file
             res.download(firmwarePath, latestFirmware, (err) => {
                 if (err) {
                     res.status(500).send('Error serving firmware');
                 }
             });
         });
     });

     // Start the server
     const PORT = 5000;
     app.listen(PORT, () => {
         console.log(`Firmware server running on port ${PORT}`);
     });
     ```

4. **Socket.IO Communication**:
   - Notify devices about available updates.
   - Use the device’s IP to determine its firmware version.
   - Example Socket.IO event:
     ```javascript
     io.on('connection', (socket) => {
         socket.on('requestFirmwareUpdate', (data) => {
             const { ip } = data; // Static IP of the STM32 device

             // Check for firmware update
             const firmwareAvailable = checkForFirmwareUpdate(ip); // Custom function
             if (firmwareAvailable) {
                 socket.emit('firmwareUpdate', {
                     url: `http://your-server-ip:5000/firmware/${ip.replace(/\./g, '_')}/latest`,
                 });
             }
         });
     });
     ```

5. **STM32 Device Interaction**:
   - Each STM32 device should:
     1. Send its static IP to the Socket.IO server during connection.
     2. Listen for the `firmwareUpdate` event containing the download URL.
     3. Use the provided URL to download the firmware via HTTP.

---

#### **Example Workflow**
1. **Device Initialization**:
   - STM32 device connects to the server using its static IP.

2. **Update Request**:
   - The device sends its static IP and checks for updates.

3. **Firmware Distribution**:
   - The server responds with the firmware URL if an update is available.

4. **Firmware Download**:
   - The STM32 device downloads and applies the firmware.

---

#### **Benefits of Using Static IPs**
- **Simplicity**: No need to maintain additional identifiers.
- **Reliability**: Static IPs are easy to configure and unique within the network.
- **Scalability**: The directory structure naturally scales with the number of devices.

---


---

### **Programming the STM32 with Arduino IDE and STCube Programmer**

#### **Step 1: Install the STM32 Arduino Core**
1. Open the Arduino IDE.
2. Go to **File > Preferences**.
3. In the **Additional Boards Manager URLs** field, add the STM32 URL:  
   ```
   https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json
   ```
4. Go to **Tools > Board > Boards Manager**.
5. Search for `STM32` and install the **STM32 MCU based boards** package.

---

#### **Step 2: Configure the STM32 Board**
1. Go to **Tools > Board > STM32 Boards (selected core)** and select your board:
   - Example: `Nucleo-144 > Nucleo H723ZG`.
2. Configure the following in the **Tools** menu:
   - **Board Part Number**: `Nucleo-H723ZG`.
   - **Upload Method**: `STM32CubeProgrammer (SWD)`.
   - **Port**: Choose the correct COM port for your board.

---

#### **Step 3: Install STCube Programmer**
1. Download the STCube Programmer from the STMicroelectronics website:  
   [STCube Programmer Download](https://www.st.com/en/development-tools/stm32cubeprog.html).
2. Install and open the programmer.
3. Ensure the STM32 board is connected to your PC via USB.

---

#### **Step 4: Write and Upload Code**
1. Write your program in the Arduino IDE or open an example sketch.
2. Click the **Upload** button in the Arduino IDE.
   - The IDE will compile the sketch and upload it using the STM32CubeProgrammer.
3. Monitor the Serial Output:
   - Open the Serial Monitor (`Tools > Serial Monitor`) to view output from the STM32 board.

---

#### **Step 5: Debugging**
- If the upload fails:
  - Ensure the **BOOT0** pin on the STM32 is set correctly .
  - Check connections and the selected port in the Arduino IDE.
  - Use the STM32CubeProgrammer to manually erase the board or debug connection issues.



---

#### **Future Enhancements**
- Add support for multiple endpoints and events in the Socket.IO client.
- Implement dynamic network configuration (e.g., using a web server).
- Enhance OTW update mechanism with fallback capabilities.

---

#### **License**
This project is open-source and licensed under the [MIT License](https://opensource.org/licenses/MIT).

---

