# PHYSICAL_AI_NAV_01

STM32 기반 Physical AI 내비게이션 프로젝트입니다.

이 예제는 단순 if-else 회피를 넘어서,
- 센서 융합(World Model)
- 위험도 추정(Risk Score)
- 적응형 회피 편향(Adaptive Turn Bias)
을 적용한 임베디드 자율주행 제어를 구현합니다.

## 핵심 아이디어
- 초음파 거리(`raw_cm`)를 EMA로 평활(`filtered_cm`)
- IR 좌/우 에지 센서 + 거리 정보를 합쳐 `obstacle_score` 계산
- 상태머신(NAV_DRIVE/BRAKE/REVERSE/TURN/EDGE_RECOVER)으로 행동 결정
- 회피 성공/실패에 따라 `turn_bias`를 온라인 업데이트
  - 성공한 방향은 강화
  - 실패한 방향은 약화

## 하드웨어
- MCU: STM32F411 (Nucleo 계열)
- Motor Driver: L298N (또는 동등한 H-Bridge)
- 초음파: HC-SR04
- IR 센서 2개(좌/우)

## 핀 매핑
- Left Motor IN1: PB12
- Left Motor IN2: PB13
- Right Motor IN1: PB14
- Right Motor IN2: PB15
- Ultrasonic TRIG: PC0
- Ultrasonic ECHO: PC1
- IR Left: PC2
- IR Right: PC3
- Status LED: PA5
- Safety Button: PC13

## 상태머신
- `NAV_DRIVE`: 기본 주행, 위험 증가 시 감속/회피 진입
- `NAV_BRAKE`: 짧은 정지로 관성 완화
- `NAV_REVERSE`: 후진으로 공간 확보
- `NAV_TURN`: 편향/센서 기반 회전
- `NAV_EDGE_RECOVER`: 라인/에지 감지 시 즉시 탈출

## 튜닝 포인트 (`Core/Src/main.c`)
- 거리 임계값: `STOP_DISTANCE_CM`, `SLOW_DISTANCE_CM`
- 회피 시간: `BRAKE_MS`, `REVERSE_MS`, `TURN_MS`
- PWM/속도: `SPD_FAST`, `SPD_SLOW`, `SPD_REVERSE`, `SPD_TURN_OUT`, `SPD_TURN_IN`

## 참고
- 모터 전원은 별도 공급 + 공통 GND 권장
- HC-SR04 ECHO는 5V 레벨일 수 있으니 레벨 시프터/분압 권장
- 현재 구현은 소프트웨어 PWM입니다 (TIM 하드웨어 PWM으로 확장 가능)
