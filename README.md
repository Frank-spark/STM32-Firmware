

---

# **STM32 Nucleo-H723ZG Firmware**

## **Overview**

This project implements firmware for the STM32 Nucleo-H723ZG development board, integrating:

- **Ethernet Communication**: Provides network connectivity for IoT applications.
- **Socket.IO Client**: Enables real-time interaction with a server.
- **Over-the-Wire (OTW) Firmware Updates**: Supports seamless firmware updates directly written to Flash memory.

This firmware provides a reliable and extendable framework for industrial and IoT applications, focusing on network communication and maintainability.

---

## **Features**

### 1. **Ethernet Communication**
- Supports static IP configuration.
- Initializes the Ethernet interface using the onboard RMII PHY.
- Provides methods to retrieve the assigned IP address for debugging.

### 2. **Socket.IO Client**
- Implements basic Socket.IO functionality for real-time communication.
- Emits and receives JSON-based messages.
- Registers event-based callbacks for handling incoming server data.

### 3. **Over-the-Wire Firmware Updates**
- Firmware updates are directly written to the STM32's Flash memory.
- Hash-based validation ensures only new firmware is applied.
- Automatically reboots into the updated firmware after successful programming.
- Supports validation of firmware size against the allocated Flash memory slot.

---

## **Requirements**

### **Hardware**
- STM32 Nucleo-H723ZG development board.
- Ethernet cable and a connected network.

### **Software**
- Arduino IDE or STM32CubeIDE for development.
- STM32 HAL libraries (pre-configured via STM32CubeMX).

---

## **Pin Configuration**

### **Ethernet RMII Interface**
| **Pin** | **Function**    |
|---------|-----------------|
| PA1     | RMII Reference Clock |
| PA2     | RMII MDIO       |
| PA7     | RMII TXD0       |
| PB11    | RMII TXD1       |
| PB12    | RMII TXEN       |
| PC1     | RMII MDC        |
| PC4     | RMII RXD0       |
| PC5     | RMII RXD1       |
| PG11    | RMII CRS_DV     |

---

## **Setup Instructions**

1. **Clone the Repository**  
   Ensure all source files (`OTWUpdate`, `EthernetSetup`, and `SocketIOClient`) are present.

2. **Configure Ethernet**  
   Update the `mac[]` and `ip` variables in the main file to match your network setup.

3. **Compile and Flash**  
   Use STM32CubeIDE or Arduino IDE to build and upload the firmware to the board.

4. **Connect to a Socket.IO Server**  
   Ensure the server is running at the configured `serverIP` and `serverPort`.

5. **Monitor Serial Output**  
   Use a serial monitor to view debugging logs and firmware behavior.

---

## **Usage**

### **1. Ethernet Communication**
- Ethernet is initialized on startup, and the assigned IP is displayed via the serial console.
- Supports basic connectivity debugging.

### **2. Socket.IO Real-Time Updates**
- Enables communication with a server using JSON messages.
- Provides event-based callbacks for handling specific server events.

### **3. Over-the-Wire Firmware Updates**
- Updates are written directly to Flash memory.
- Validates firmware size and hash before programming.
- Automatically reboots after successful updates.

---

## **Project Structure**

```
STM32_Firmware_Project/
├── OTWUpdate.h           # Header for OTW firmware updates
├── OTWUpdate.cpp         # Implementation of OTW updates
├── EthernetSetup.h       # Header for Ethernet initialization
├── EthernetSetup.cpp     # Implementation of Ethernet setup
├── SocketIOClient.h      # Header for Socket.IO client
├── SocketIOClient.cpp    # Implementation of Socket.IO client
├── main.cpp              # Main application logic
├── README.md             # Project documentation
```

---

## **Known Limitations**

1. **Firmware Source Detection**  
   The function `checkForNewFirmware` currently always returns `false`. Implement logic to detect new firmware in memory or over the network.

2. **Socket.IO Event Handling**  
   While basic event handling works, it needs expansion to support multiple dynamic events with robust parsing.

3. **Ethernet Initialization Resilience**  
   Ethernet setup lacks retry mechanisms. Add retries or timeouts for enhanced reliability.

4. **Flash Write Validation**  
   The Flash programming function does not verify the written data matches the input. Add verification to improve reliability.

---

## **Future Enhancements**

- Add full detection logic for over-the-wire firmware updates.
- Implement dynamic network configuration (e.g., DHCP or web-based settings).
- Extend Socket.IO client functionality to support multiple endpoints and robust error handling.

---

## **License**

This project is open-source and licensed under the [MIT License](https://opensource.org/licenses/MIT).

---

