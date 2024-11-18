# Age and Gender Detection Using IoT (ESP32-CAM)

This project integrates IoT with Machine Learning to build an age and gender detection system using the **ESP32-CAM module**, an **FTDI programmer**, and **pre-trained models** for age and gender detection. The ESP32-CAM captures images and sends them to a Python Flask server, which processes the images using OpenCV and ML models to predict age and gender.

---

## Features
- Detects **age brackets** (e.g., 0-2, 25-32, 60-100) using a pre-trained model.
- Predicts **gender** as Male or Female.
- Low-latency communication between ESP32-CAM and Flask server.
- Real-time predictions with easy scalability.

---

## Prerequisites

### Hardware Requirements
- **ESP32-CAM module**
- **FTDI Programmer** (for programming ESP32-CAM)
- USB cable and jumper wires for connections.

### Software Requirements
- Python 3.7 or above
- Libraries: 
  - `Flask`
  - `OpenCV` (`cv2`)
  - `NumPy`

---

## Setup

### 1. Hardware Configuration
1. Connect the ESP32-CAM module to the FTDI Programmer as follows:
   - **ESP32-CAM GND** → **FTDI GND**
   - **ESP32-CAM 5V** → **FTDI VCC**
   - **ESP32-CAM U0R** → **FTDI TX**
   - **ESP32-CAM U0T** → **FTDI RX**
   - Set the FTDI Programmer to 5V mode.

2. Install the **Arduino IDE** and add the ESP32 board support.  
   Follow [this guide](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) to install the ESP32 core.

3. Upload the ESP32-CAM sketch for image capturing and HTTP communication. The sample sketch is provided in the `ESP32-CAM` directory.

---

### 2. Python Flask Server
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/aAge-gender_detection_using_esp32CamModule
.git
   cd Age-gender_detection_using_esp32CamModule

