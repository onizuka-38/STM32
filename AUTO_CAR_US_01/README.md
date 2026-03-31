# AUTO_CAR_US_01

STM32 + HC-SR04 + L298N obstacle-avoidance autonomous car example

## Features
- Forward driving in normal mode
- Ultrasonic distance measurement
- Obstacle avoidance when distance <= 25 cm: brake -> reverse -> turn
- Safety stop toggle by USER button (PC13)
- Software PWM speed control (independent left/right speed)

## Pin Mapping
- Left Motor IN1: PB12
- Left Motor IN2: PB13
- Right Motor IN1: PB14
- Right Motor IN2: PB15
- Ultrasonic TRIG: PC0
- Ultrasonic ECHO: PC1
- Status LED: PA5
- Safety Button: PC13 (Nucleo USER button)

## Tuning Parameters
Tune these in `Core/Src/main.c`:
- Speed: `SPD_FORWARD`, `SPD_REVERSE`, `SPD_TURN_OUT`, `SPD_TURN_IN`
- Distance: `STOP_DISTANCE_CM`, `REVERSE_DISTANCE_CM`
- Timing: `BRAKE_MS`, `REVERSE_MS`, `TURN_MS`

## Notes
- This project uses software PWM (not timer hardware PWM).
- Use common GND between STM32 board, motor driver, and sensor.
- HC-SR04 ECHO can be 5V level; use a level shifter or divider.
