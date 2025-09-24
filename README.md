# ESP32 Line Following Robot for Competition

A competitive line-following robot built on the ESP32 platform. This robot uses a 5-sensor array for line tracking, two side sensors for line recovery, and an IR sensor for obstacle detection.



## 🤖 Features

- **Autonomous Line Following:** Uses a 5-channel IR sensor array to navigate black lines on a white surface.
- **Obstacle Detection:** An IR proximity sensor stops the robot if an obstacle is detected in its path.
- **Rescue/Recovery Mode:** Utilizes two side-mounted sensors to find the line if the robot completely derails.
- **Variable Speed Control:** Implements different speeds for straightaways, moderate turns, and sharp turns for optimized performance.

## 🛠️ Hardware Components

- **Microcontroller:** ESP32 Development Board
- **Motor Driver:** L298N Dual H-Bridge Motor Driver
- **Sensors:**
    - 5x IR Sensor Array (Front)
    - 2x IR Sensors (Left & Right Sides)
    - 1x IR Proximity Sensor (Obstacle Detection)
- **Actuators:** 2x DC Geared Motors
- **Chassis:** Custom or pre-built robot chassis
- **Power:** Li-Po battery or other suitable power source

## 💻 Software & Logic

- **Language:** C++ (Arduino Framework)
- **IDE:** Arduino IDE or PlatformIO
- **Core Logic:** The robot's movement is based on a series of `if-else` conditions that read the state of the 5 front sensors. If the robot loses the line entirely, it enters a "Rescue Mode" guided by the side sensors until the line is found again.

## Challenges & Learnings

_ A key challenge was calibrating the sensor thresholds to work under different lighting conditions. I solved this by creating a simple calibration routine and finding an optimal `frontThreshold` value through testing._
