# 서울시 디지털 계량기 프로토콜 V1.3 구현

## 개요
이 프로젝트는 서울특별시 디지털 계량기 프로토콜 V1.3을 ABOV A31L12x 마이크로컨트롤러의 LPUART 인터페이스를 통해 구현한 것입니다.

## 프로토콜 사양

### 물리 계층 (Physical Layer)
- **통신 방식**: Master/Slave (검침단말기/전자식 수도계량기)
- **신호 레벨**: TTL Level (Tx, Rx, Gnd)
  - High Level: 3.6V ~ 1.5V
  - Low Level: 0.7V ~ 0V
- **Baud Rate**: 1,200 bps
- **Data Format**: 8 data bits, None parity, 1 stop bit
- **통신 시작**: High Level을 20ms 유지 후 start bit 시작

### 프레임 구조 (로직 분석기 캡처 결과 기반)

**TX 프레임 (MCU → 계량기):**
```
+--------+-----+--------+------+----------+
| HEADER | CMD | LENGTH | DATA | CHECKSUM |
+--------+-----+--------+------+----------+
   1B     1B      1B      nB       1B

예시: 10-5B-01-5C-16
      |  |  |  |   |
      |  |  |  |   +-- CHECKSUM (0x10^0x5B^0x01^0x5C = 0x16)
      |  |  |  +------ DATA (0x5C)
      |  |  +--------- LENGTH (0x01)
      |  +------------ CMD (0x5B)
      +--------------- HEADER (0x10)
```

**RX 프레임 (계량기 → MCU):**
```
+--------+-----+--------+--------------+----------+
| HEADER | CMD | LENGTH | DATA         | CHECKSUM |
+--------+-----+--------+--------------+----------+
   1B     1B      1B      n bytes          1B

예시: 63-1F-1F-89-08-01-78-1F-17-50-16-23-03-1C-13-51-53-22-00-23-16-00
      |  |  |  |                                              |
      |  |  |  +-- DATA (가변 길이)                            +-- CHECKSUM
      |  |  +----- LENGTH
      |  +-------- CMD
      +----------- HEADER (0x63)
```

- **HEADER**: 프레임 시작 (TX: 0x10, RX: 0x63)
- **CMD**: 커맨드 코드 (1 byte)
- **LENGTH**: 데이터 길이 (1 byte)
- **DATA**: 실제 데이터 (가변 길이)
- **CHECKSUM**: XOR 체크섬 (HEADER + CMD + LENGTH + DATA)

### 제어 문자
- **ACK**: 0x06 (Acknowledge) - 정상 수신
- **NAK**: 0x15 (Negative Acknowledge) - 오류 발생

### 커맨드 코드 (예시)
- `0x30`: 현재 데이터 읽기
- `0x31`: 이력 데이터 읽기
- `0x32`: 시간 설정
- `0x33`: 상태 읽기

## 파일 구조

```
LPUART_Interrupt/
├── main.c                    # 메인 프로그램
├── meter_protocol.h          # 프로토콜 헤더 파일
├── meter_protocol.c          # 프로토콜 구현 파일
├── main_conf.h               # 설정 헤더
└── README_METER_PROTOCOL.md  # 본 문서
```

## 주요 기능

### 1. 초기화
```c
void Meter_Init(void);
```
계량기 프로토콜을 초기화합니다.

### 2. 콜백 함수 등록
```c
void Meter_SetResponseCallback(void (*callback)(uint8_t* data, uint16_t length));
void Meter_SetErrorCallback(void (*callback)(METER_ERROR_Type error));
```
응답 수신 및 에러 발생 시 호출될 콜백 함수를 등록합니다.

### 3. 커맨드 전송
```c
METER_ERROR_Type Meter_SendCommand(uint8_t addr, uint8_t cmd, uint8_t* data, uint8_t length);
```
계량기에 커맨드를 전송합니다. 자동으로 20ms Preamble을 전송하고 프레임을 구성합니다.

### 4. 태스크 실행
```c
void Meter_Task(void);
```
주기적으로 호출하여 수신 데이터 처리 및 타임아웃을 체크합니다.

## 사용 예제

### 기본 사용법

```c
#include "meter_protocol.h"

// 콜백 함수 정의
void OnMeterResponseReceived(uint8_t* data, uint16_t length)
{
    // 수신 데이터 처리
    printf("Received %d bytes\n", length);
}

void OnMeterError(METER_ERROR_Type error)
{
    // 에러 처리
    printf("Error occurred: %d\n", error);
}

int main(void)
{
    // 시스템 초기화
    SystemInit();

    // LPUART 초기화 (1200 bps)
    LPUART_Configure();

    // 계량기 프로토콜 초기화
    Meter_Init();

    // 콜백 등록
    Meter_SetResponseCallback(OnMeterResponseReceived);
    Meter_SetErrorCallback(OnMeterError);

    while(1)
    {
        // 프로토콜 태스크 실행
        Meter_Task();

        // 커맨드 전송 (예: 현재 데이터 읽기)
        uint8_t data[] = {0x12, 0x34};
        Meter_SendCommand(0x01, CMD_READ_CURRENT_DATA, data, 2);

        delay_ms(5000);  // 5초 대기
    }
}
```

## 하드웨어 연결

### LPUART 핀 설정
- **TX**: PB3 (LPTXD) - 계량기 RX에 연결
- **RX**: PB4 (LPRXD) - 계량기 TX에 연결
- **GND**: 공통 그라운드

### TTL 레벨 변환
계량기가 다른 전압 레벨을 사용하는 경우 레벨 시프터를 사용하여 연결하십시오.

## 타이밍 다이어그램

```
Master (MCU)         Slave (Meter)
    |                     |
    |--- Preamble (20ms)--|
    |                     |
    |--- STX ------------>|
    |--- ADDR ----------->|
    |--- CMD ------------>|
    |--- LENGTH --------->|
    |--- DATA ----------->|
    |--- CHECKSUM ------->|
    |--- ETX ------------>|
    |                     |
    |<--- ACK/NAK --------|
    |                     |
    | (응답 프레임)        |
    |<--- STX ------------|
    |<--- ADDR -----------|
    |<--- CMD ------------|
    |<--- LENGTH ---------|
    |<--- DATA -----------|
    |<--- CHECKSUM -------|
    |<--- ETX ------------|
    |                     |
    |--- ACK ------------>|
    |                     |
```

## 에러 처리

### 에러 코드
- `METER_ERR_NONE`: 에러 없음
- `METER_ERR_TIMEOUT`: 응답 타임아웃 (1000ms)
- `METER_ERR_CHECKSUM`: 체크섬 오류
- `METER_ERR_INVALID_FRAME`: 잘못된 프레임
- `METER_ERR_NAK_RECEIVED`: NAK 수신
- `METER_ERR_BUFFER_OVERFLOW`: 버퍼 오버플로우

### 재전송
- 최대 재전송 횟수: 3회
- 타임아웃 발생 시 자동 재전송

## 디버그

### 디버그 포트
- **UART1**: 38400 bps, 8-N-1
- 디버그 메시지 출력용

### 로그 메시지
```
Seoul Digital Water Meter Protocol Initialized
Baudrate: 1200 bps, Format: 8-N-1

Sending READ_CURRENT_DATA command to meter...
Meter Response Received: 12 34 56 78
```

## 주의사항

1. **Preamble**: 모든 통신 시작 전 20ms High Level 유지 필수
2. **Baud Rate**: 정확한 1200 bps 설정 필요
3. **타이밍**: 저속 통신이므로 타임아웃 값 적절히 설정
4. **체크섬**: XOR 체크섬으로 데이터 무결성 확인

## 빌드 및 실행

### 빌드 환경
- **IDE**: Keil MDK 또는 IAR EWARM
- **컴파일러**: ARM C/C++ Compiler
- **MCU**: ABOV A31L12x

### 빌드 방법
1. 프로젝트 파일 열기
2. `meter_protocol.c`와 `meter_protocol.h`를 프로젝트에 추가
3. 빌드 실행
4. 타겟에 다운로드

## 문제 해결

### 통신이 안 되는 경우
1. Baud rate 확인 (1200 bps)
2. 핀 연결 확인 (TX-RX 교차 연결)
3. GND 공통 연결 확인
4. TTL 레벨 확인

### 체크섬 오류
1. 데이터 전송 중 노이즈 확인
2. 케이블 길이 확인 (짧을수록 좋음)
3. GND 연결 확인

### 타임아웃 발생
1. 계량기 전원 확인
2. 계량기 주소 확인
3. 커맨드 코드 확인

## 참고 문서
- 서울특별시 디지털계량기 프로토콜 V1.3 (2023.01.31)
- ABOV A31L12x HAL Driver Manual
- LPUART Application Note

## 라이선스
Copyright 2020 ABOV Semiconductor Co., Ltd. All rights reserved.
BSD-3-Clause License

## 작성자
Seoul Digital Water Meter Protocol Implementation Team

## 버전 이력
- **V1.0** (2024): 초기 구현
  - 기본 프로토콜 구현
  - 1200 bps LPUART 설정
  - 프레임 송수신 기능
  - 에러 처리 및 재전송
