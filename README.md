# 🔥 Flame-Aware Rescue Robot

An autonomous robot that **detects flames** using three analog flame sensors, **avoids obstacles** using an ultrasonic sensor, and **points a servo** toward the detected flame. The robot uses an **Arduino** and an **L298N motor driver** for locomotion.

---

## 🧩 Features
- 🔥 Detects flames on left, front, and right using analog flame sensors.
- 🚧 Avoids obstacles in front using HC-SR04 ultrasonic sensor.
- 🎯 Servo points to the detected flame to indicate direction.
- 🚗 Differential drive using L298N motor driver.
- 🛠️ Simple logic suitable for rescue demo or fire-detection prototypes.

---

## ⚙️ Components
- Arduino UNO / Nano
- L298N motor driver
- 3 × Analog flame sensors
- HC-SR04 Ultrasonic sensor (TRIG/ECHO)
- Servo motor (for pointing)
- 2 × DC motors + chassis + wheels
- Battery pack (9V/12V) — common ground with Arduino
- Jumper wires, breadboard

---

## 🔌 Wiring (summary)
- Flame sensors → A1 (left), A0 (front), A2 (right)
- Ultrasonic TRIG → D6
- Ultrasonic ECHO → D7
- Servo → D5 (PWM)
- L298N:
  - ENA → D3 (PWM)
  - IN1 → D8
  - IN2 → D9
  - ENB → D11 (PWM)
  - IN3 → D10
  - IN4 → D12
- Ensure common ground between Arduino, L298N, and battery.

---

## 📂 Files
- `flame_rescue_bot.ino` — main Arduino sketch
- `README.md` — this file
- 
---

## 🔧 Calibration & Tips
- Calibrate `flameThreshold` according to your sensors and environment.
- Test flame sensors at safe, controlled sources (candle flame at a distance).
- Use separate power supply for motors; keep Arduino ground common.
- Adjust PWM values to balance motor speeds.

---

