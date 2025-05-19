<h1 align="center">💡 STM32 프로젝트 학습 저장소</h1>
<p align="center">
  STM32F411RE | ARM Cortex-M4 | HAL 기반 임베디드 개발 실습
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Board-STM32F411RE-blue?logo=stmicroelectronics" />
  <img src="https://img.shields.io/badge/Language-C-blue.svg" />
  <img src="https://img.shields.io/badge/IDE-STM32CubeIDE-lightgrey?logo=c" />
</p>

---

## 🧭 소개

이 저장소는 **STM32F411RE (Nucleo-64)** 보드를 활용하여 C언어 기반의 임베디드 시스템을 학습하는 공간입니다.  
GPIO, UART, PWM, ADC 등 마이크로컨트롤러 주변장치들을 실습하며, STM32CubeIDE와 HAL 드라이버로 프로젝트를 구성합니다.

---

## 🔍 STM32F411RE 보드 개요

> STMicroelectronics의 32비트 ARM Cortex-M4 기반 MCU

| 항목 | 내용 |
|------|------|
| MCU | STM32F411RE (Cortex-M4, 최대 100MHz) |
| Flash | 512KB |
| SRAM | 128KB |
| 인터페이스 | GPIO, UART, SPI, I2C, ADC, PWM, EXTI 등 |
| 특징 | ST-LINK 디버거 내장, Arduino 호환 커넥터 |

---

## 📁 디렉토리 구성 예시

```bash
STM32/
├── GPIO_LedToggle/          # LED 깜빡이기 예제
├── UART_Communication/      # 시리얼 통신 송수신
├── ADC_TemperatureSensor/   # 온도 센서 읽기
├── PWM_MotorControl/        # PWM을 이용한 모터 제어
├── EXTI_ButtonInterrupt/    # 버튼 인터럽트 처리
└── README.md
