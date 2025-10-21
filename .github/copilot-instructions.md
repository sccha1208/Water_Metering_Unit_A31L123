# A31L12x Firmware Development Guide

이 문서는 ABOV A31L12x 마이크로컨트롤러 펌웨어 개발을 위한 AI 에이전트 가이드라인입니다.

## 프로젝트 구조

```
├── Core/               # CMSIS 코어 파일
├── Drivers/           # HAL 드라이버
│   ├── Include/      # 드라이버 헤더 파일
│   └── Source/       # 드라이버 소스 파일
├── Examples/          # 예제 코드
├── Doc/              # 문서
└── Projects/         # IDE 프로젝트 파일
    ├── IAR/         # IAR Workbench 프로젝트
    └── KEIL/        # Keil MDK 프로젝트
```

## 주요 컴포넌트

1. **HAL 드라이버**
   - `Drivers/Include/A31L12x_hal_*.h`: 하드웨어 추상화 계층 헤더
   - `Drivers/Source/A31L12x_hal_*.c`: HAL 구현 파일
   - 네이밍 규칙: `HAL_<peripheral>_<function>()`

2. **시스템 초기화**
   - `A31L12x_SystemClock.c`: 시스템 클럭 설정
   - `A31L12x_PortInit.c`: GPIO 초기 설정
   - `A31L12x_it.c`: 인터럽트 핸들러

## 개발 패턴

1. **주변장치 초기화**
   ```c
   HAL_<peripheral>_Init(&<peripheral>_InitStruct);
   ```

2. **인터럽트 처리**
   - 모든 인터럽트 핸들러는 `A31L12x_it.c`에 정의
   - 네이밍 규칙: `<peripheral>_IRQHandler`

3. **전력 관리**
   - `HAL_PWR_EnterSleepMode()`: 슬립 모드 진입
   - `HAL_PWR_EnterPowerDownMode()`: 파워다운 모드 진입

## 중요 작업 흐름

1. **새 주변장치 초기화**:
   - `A31L12x_PortInit.c`에 GPIO 설정 추가
   - `main_conf.h`에 관련 설정 정의
   - `main.c`에서 초기화 함수 호출

2. **예제 코드 구조**:
   ```c
   void main(void)
   {
       SystemClock_Config();    // 시스템 클럭 설정
       PortInit();             // GPIO 초기화
       <peripheral>_Init();    // 주변장치 초기화
       
       while(1)
       {
           // 메인 루프
       }
   }
   ```

## 주의사항

- v1.5.0 기준 최신 HAL 드라이버 사용
- GPIO 설정 시 `HAL_GPIO_ConfigOutput()` 함수 사용에 주의
- I2C 통신 시 `I2Cn_Mwait()` 상태 체크 방식 참고
- SRAM 리텐션 설정 시 `PMU_SRAMRetentionPower` 예제 참조

## 참조 파일

- 주변장치 설정: `Examples/*/main_conf.h`
- 시스템 초기화: `Examples/*/A31L12x_SystemClock.c`
- 인터럽트 처리: `Examples/*/A31L12x_it.c`