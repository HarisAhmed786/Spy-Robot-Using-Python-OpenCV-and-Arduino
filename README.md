# 🤖 Spy Robot

Spy Robot is an intelligent surveillance and security robot built using **Arduino**, **ESP32-CAM**, and **OpenCV**. It is designed for real-time monitoring, human detection, obstacle avoidance, and autonomous response in potential danger situations.

This project combines **robotics**, **embedded systems**, **IoT**, and **computer vision** to create a low-cost smart security solution.

---

## 🚀 Features

- 🎥 Real-time video streaming using ESP32-CAM  
- 👤 Human / Face detection using OpenCV Haar Cascade  
- 🎙️ Audio monitoring support  
- 🚧 Obstacle avoidance using ultrasonic & IR sensors  
- 🔄 Autonomous escape mechanism in danger zone  
- 📡 Wireless monitoring and control  
- 🤖 Mobile robot movement using DC motors  

---

## 🛠️ Hardware Components

- Arduino UNO / Nano  
- ESP32-CAM Module  
- OV2640 Camera  
- L298N Motor Driver  
- DC Motors + Wheels + Chassis  
- Ultrasonic Sensor (HC-SR04)  
- IR Sensors  
- IR LEDs for night vision  
- Jumper Wires  
- Power Supply / Battery Pack  

---

## 💻 Software & Tools Used

- Arduino IDE  
- Python  
- OpenCV  
- Haar Cascade Classifier  
- Serial Communication  
- Embedded C  

---

## 📌 How It Works

### 1️⃣ Surveillance Mode
ESP32-CAM streams live video to a connected device.

### 2️⃣ Detection Mode
Laptop processes video using OpenCV Haar Cascade to detect faces or humans.

### 3️⃣ Obstacle Avoidance
Ultrasonic and IR sensors continuously scan surroundings and avoid collisions.

### 4️⃣ Danger Zone Response
If a human approaches too close, the robot automatically retreats.

### 5️⃣ Night Vision
IR LEDs allow visibility in low-light environments.

---

## 📷 Technologies Implemented

- Computer Vision  
- Embedded Systems  
- Robotics Automation  
- Sensor Integration  
- Real-Time Decision Making  
- Wireless Streaming  

---

## 📂 Project Structure

```bash
Spy-Robot/
│── Arduino_Code/
│── ESP32_CAM_Code/
│── Python_Detection/
│── Images/
│── README.md
