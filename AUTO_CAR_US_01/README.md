# AUTO_CAR_US_01

STM32 + HC-SR04 + L298N 기반 장애물 회피 자율주행 예제입니다.

## 기능
- 전진 주행
- 초음파 거리 측정
- 임계거리(`25cm`) 이하에서 자동 회피(정지 -> 후진 -> 회전)
- USER 버튼(PC13)으로 안전정지 ON/OFF

## 핀 매핑
- Left Motor IN1: PB12
- Left Motor IN2: PB13
- Right Motor IN1: PB14
- Right Motor IN2: PB15
- Ultrasonic TRIG: PC0
- Ultrasonic ECHO: PC1
- Status LED: PA5
- Safety Button: PC13 (Nucleo USER button)

## 주의사항
- 모터 전원은 보드 5V 라인과 분리/공통 GND 권장
- HC-SR04 ECHO는 5V 출력일 수 있으므로 레벨시프터 사용 권장
- 현재 속도제어(PWM)는 미적용이며 방향 제어 중심 로직입니다
