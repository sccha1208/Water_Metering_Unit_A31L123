/**
 *******************************************************************************
 * @file        meter_protocol.h
 * @author      Seoul Digital Water Meter Protocol Implementation
 * @brief       서울시 디지털계량기 프로토콜 V1.1~V1.4 범용 헤더 파일
 * @details     1200bps, 8-N-1 통신 프로토콜
 *              모든 버전(V1.1, V1.2, V1.3, V1.4) 자동 감지 및 파싱 지원
 *******************************************************************************
 */

#ifndef _METER_PROTOCOL_H_
#define _METER_PROTOCOL_H_

#include "main_conf.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// 프로토콜 상수 정의
//******************************************************************************

// 통신 파라미터
#define METER_BAUDRATE              1200        // 1200 bps
#define METER_PREAMBLE_TIME_MS      20          // 20ms High Level 유지

// 프레임 구조 (서울시 디지털계량기 실제 프로토콜)
// 실제 수신 데이터 기반
//
// TX (MCU -> 계량기) 예시: 10-5B-01-5C-16
//   [HEADER] [CMD] [LEN] [DATA] [CHECKSUM]
//    0x10    0x5B   0x01  0x5C    0x16
//
// RX (계량기 -> MCU) 실제 데이터: 68-0F-0F-68-08-01-78-0F-17-50-16-23-00-1C-13-51-51-22-00-23-16-00
//   [START] [L] [L] [START] [DATA...........................] [CHECKSUM] [END]
//    0x68   0x0F 0x0F 0x68   (L=15 bytes)                      CS        0x16
//
// M-Bus 유사 프로토콜:
//   68 L L 68 [DATA] CS 16
//   - 68: Start byte (고정)
//   - L: Data length (2번 반복)
//   - 68: Start byte 반복 (확인용)
//   - DATA: 실제 데이터 (L 바이트)
//   - CS: Checksum
//   - 16: End byte (고정)
//
#define METER_FRAME_HEADER_TX       0x10        // TX 프레임 헤더
#define METER_FRAME_START_RX        0x68        // RX 프레임 시작 바이트 (트리거)
#define METER_FRAME_END_RX          0x16        // RX 프레임 종료 바이트
#define METER_ACK                   0x06        // Acknowledge
#define METER_NAK                   0x15        // Negative Acknowledge

// 프레임 길이
#define METER_MAX_FRAME_SIZE        256
#define METER_MIN_FRAME_SIZE        5           // HEADER + CMD + LEN + DATA(최소1) + CHECKSUM

// 커맨드 코드 (예시 - 실제 프로토콜 문서 참조)
#define CMD_READ_CURRENT_DATA       0x30        // 현재 데이터 읽기
#define CMD_READ_HISTORY_DATA       0x31        // 이력 데이터 읽기
#define CMD_SET_TIME                0x32        // 시간 설정
#define CMD_READ_STATUS             0x33        // 상태 읽기

// 타임아웃
#define METER_RESPONSE_TIMEOUT_MS   1000        // 응답 대기 시간
#define METER_BYTE_TIMEOUT_MS       100         // 바이트간 타임아웃

// 재전송
#define METER_MAX_RETRY             3           // 최대 재전송 횟수

// 딜레이 상수 (meter_protocol.c 내부 사용)
#define METER_PREAMBLE_DELAY_CYCLES 160000      // Preamble 20ms (32MHz 기준)
#define METER_TX_STABILIZE_DELAY    5000        // LPUART TX 안정화 딜레이
#define METER_FIFO_CLEAR_DELAY      1000        // FIFO 클리어 딜레이
#define METER_TX_COMPLETE_DELAY     400000      // TX 완료 대기 (~50ms)

//******************************************************************************
// 타입 정의
//******************************************************************************

// 프로토콜 상태
typedef enum
{
    METER_STATE_IDLE = 0,           // 대기 상태
    METER_STATE_PREAMBLE,           // Preamble 전송 중
    METER_STATE_TX,                 // 데이터 전송 중
    METER_STATE_WAIT_RESPONSE,      // 응답 대기 중
    METER_STATE_RX,                 // 데이터 수신 중
    METER_STATE_COMPLETE,           // 완료
    METER_STATE_ERROR               // 오류
} METER_STATE_Type;

// 프레임 타입
typedef enum
{
    FRAME_TYPE_COMMAND = 0,         // 명령 프레임
    FRAME_TYPE_RESPONSE,            // 응답 프레임
    FRAME_TYPE_ACK,                 // ACK 프레임
    FRAME_TYPE_NAK                  // NAK 프레임
} FRAME_TYPE_Type;

// 에러 코드
typedef enum
{
    METER_ERR_NONE = 0,             // 에러 없음
    METER_ERR_TIMEOUT,              // 타임아웃
    METER_ERR_CHECKSUM,             // 체크섬 오류
    METER_ERR_INVALID_FRAME,        // 잘못된 프레임
    METER_ERR_NAK_RECEIVED,         // NAK 수신
    METER_ERR_BUFFER_OVERFLOW,      // 버퍼 오버플로우
    METER_ERR_INVALID_PARAM         // 잘못된 파라미터
} METER_ERROR_Type;

// 프레임 구조체 (서울시 실제 프로토콜)
// 프레임 구조: [HEADER(1)] [CMD(1)] [LEN(1)] [DATA(n)] [CHECKSUM(1)]
typedef struct
{
    uint8_t     header;             // 프레임 헤더 (0x10)
    uint8_t     cmd;                // 커맨드
    uint8_t     length;             // 데이터 길이
    uint8_t     data[METER_MAX_FRAME_SIZE - 4];  // 데이터
    uint8_t     checksum;           // 체크섬 (XOR)
} METER_FRAME_Type;

// 프로토콜 컨텍스트
typedef struct
{
    METER_STATE_Type    state;              // 현재 상태
    uint32_t            timeout_ms;         // 타임아웃 카운터
    uint8_t             retry_count;        // 재전송 카운터
    METER_ERROR_Type    last_error;         // 마지막 에러

    // TX 버퍼
    uint8_t             tx_buffer[METER_MAX_FRAME_SIZE];
    uint16_t            tx_length;
    uint16_t            tx_index;

    // RX 버퍼
    uint8_t             rx_buffer[METER_MAX_FRAME_SIZE];
    uint16_t            rx_length;
    uint16_t            rx_index;

    // 콜백 함수
    void (*on_response_received)(uint8_t* data, uint16_t length);
    void (*on_error)(METER_ERROR_Type error);
} METER_CONTEXT_Type;

//******************************************************************************
// 함수 프로토타입
//******************************************************************************

// 초기화 및 설정
void Meter_Init(void);
void Meter_SetResponseCallback(void (*callback)(uint8_t* data, uint16_t length));
void Meter_SetErrorCallback(void (*callback)(METER_ERROR_Type error));

// 프레임 생성 및 전송
METER_ERROR_Type Meter_SendCommand(uint8_t addr, uint8_t cmd, uint8_t* data, uint8_t length);
METER_ERROR_Type Meter_SendACK(void);
METER_ERROR_Type Meter_SendNAK(void);

// 수신 처리
void Meter_ProcessReceive(uint8_t* data, uint16_t length);
void Meter_Task(void);  // 주기적으로 호출해야 하는 타스크 함수

// 유틸리티 함수
uint8_t Meter_CalculateChecksum(uint8_t* data, uint16_t length);
uint8_t Meter_ValidateFrame(METER_FRAME_Type* frame);
void Meter_SendPreamble(void);  // 20ms High Level 전송

// 상태 조회
METER_STATE_Type Meter_GetState(void);
METER_ERROR_Type Meter_GetLastError(void);
void Meter_Reset(void);

// SysTick 지원 함수 (A31L12x_it.c에서 호출)
void Meter_SysTick_Increment(void);

//******************************************************************************
// 범용 프로토콜 파서 (V1.1~V1.4 지원)
//******************************************************************************

// 프로토콜 버전 열거형
typedef enum
{
    PROTOCOL_UNKNOWN = 0,       // 알 수 없는 버전
    PROTOCOL_V1_1    = 11,      // V1.1 (2013.06)
    PROTOCOL_V1_2    = 12,      // V1.2 (2021.05)
    PROTOCOL_V1_3    = 13,      // V1.3 (2023.01)
    PROTOCOL_V1_4    = 14       // V1.4 (2024.07~2025.05)
} ProtocolVersion_t;

// Status 정보 (버전별 차이 반영)
typedef struct
{
    // 공통 필드 (모든 버전)
    bool q3_exceed;             // Bit 7: 최대유량 초과 (V1.1은 Q4, V1.2+는 Q3)
    bool reverse_flow;          // Bit 6: 역류 감지
    bool indoor_leak;           // Bit 5: 옥내누수

    // 버전별 전용 필드
    union {
        // V1.1 전용
        struct {
            bool batt_low;          // Bit 2: 배터리 부족
            bool freeze_warning;    // Bit 1: 동파경고 (-2℃ 3시간)
        } v11;

        // V1.2 전용
        struct {
            bool batt_low;          // Bit 2: 배터리 부족 (3개월 전)
        } v12;

        // V1.3 전용
        struct {
            uint8_t batt_voltage;   // Bit 4-0: 배터리 전압 (0~31, 0.1V 단위)
                                    // 00000 = 3.7V 이상
                                    // 00001 = 3.6~3.7V
                                    // ...
                                    // 11111 = 0.7V 미만
        } v13;

        // V1.4 전용
        struct {
            uint8_t batt_voltage;   // Bit 4-0: 배터리 전압 (V1.3과 동일)
            bool magnet_detected;   // Status2 Bit 7: 자석 감지 (5분)
            bool freeze_warning;    // Status2 Bit 6: 동파경보 (0℃ 1분)
        } v14;
    } ext;

} MeterStatus_t;

// 파싱된 계량기 데이터 (디버그 출력용, 스택 할당)
typedef struct
{
    // 프레임 기본 정보
    ProtocolVersion_t version;      // 감지된 프로토콜 버전
    bool parse_success;             // 파싱 성공 여부

    // 프레임 필드
    uint8_t c_field;                // Control Field
    uint8_t a_field;                // Address Field
    uint8_t ci_field;               // CI Field

    // User Data
    uint32_t meter_id;              // 기물번호 (BCD → Decimal)
                                    // 예: BCD 0x09123456 → 9123456

    MeterStatus_t status;           // Status 정보 (버전별)

    // DIF/VIF 정보
    uint16_t diameter_mm;           // 계량기 구경 (15, 20, 25, ..., 300)
    uint8_t decimal_point;          // 소수점 위치 (2 or 3)

    // 검침값 (원본 BCD)
    uint8_t reading_bcd[4];         // BCD 형식 그대로 (Little Endian)
                                    // 예: 12345.678 → {0x78, 0x56, 0x34, 0x12}

    // 검침값 (변환 결과 - 디스플레이용)
    uint32_t reading_value;         // BCD를 정수로 변환
                                    // 예: 12345678 (소수점 없음)
                                    // 실제 값 = reading_value / 10^decimal_point

    // UDF (선택적, V1.2+)
    bool has_udf;                   // UDF 포함 여부
    uint8_t udf_protocol_ver;       // UDF 프로토콜 버전 (0x12, 0x13, 0x14)
    uint8_t udf_verification_month; // 검정 월 (1~12)
    uint16_t udf_manufacturer_code; // 제조사 코드 (예: 0x0041 = 'A')

    // 체크섬
    uint8_t checksum_received;      // 수신한 체크섬
    uint8_t checksum_calculated;    // 계산한 체크섬
    bool checksum_valid;            // 체크섬 유효성

    // 원본 프레임 참조 (포인터만 저장, 복사 안 함)
    uint8_t* raw_frame;             // 원본 프레임 포인터
    uint16_t raw_length;            // 원본 프레임 길이

} MeterData_t;

//******************************************************************************
// 범용 파서 함수 프로토타입
//******************************************************************************

/**
 * @brief 프로토콜 버전 자동 감지
 * @param frame 원본 프레임 (68 L L 68 ...)
 * @param length 프레임 길이
 * @return 감지된 프로토콜 버전
 */
ProtocolVersion_t Meter_DetectVersion(uint8_t* frame, uint16_t length);

/**
 * @brief 모든 버전 프레임 통합 파싱
 * @param frame 원본 프레임
 * @param length 프레임 길이
 * @param parsed_data 파싱 결과 저장 구조체 (출력)
 * @return true: 파싱 성공, false: 파싱 실패
 */
bool Meter_ParseFrame(uint8_t* frame, uint16_t length, MeterData_t* parsed_data);

/**
 * @brief 파싱된 데이터를 디버그 포트로 출력
 * @param data 파싱된 데이터
 * @note UART1 디버그 포트로 출력 (PB0:TX, PB1:RX)
 */
void Meter_PrintParsedData(MeterData_t* data);

/**
 * @brief BCD 4바이트를 uint32로 변환
 * @param bcd BCD 배열 [4] (Little Endian)
 * @return 변환된 정수 (예: {0x78, 0x56, 0x34, 0x12} → 12345678)
 */
uint32_t Meter_BCD_To_Uint32(uint8_t bcd[4]);

/**
 * @brief 배터리 전압 코드를 실제 전압(V)으로 변환
 * @param voltage_code 0~31 (5비트)
 * @return 전압 (V), 예: 36 (3.6V를 10배한 값)
 */
uint8_t Meter_GetBatteryVoltage_x10(uint8_t voltage_code);

/**
 * @brief 구경 코드를 mm 단위로 변환
 * @param dif_code DIF의 상위 4비트 (구경 코드)
 * @return 구경 (mm), 예: 15, 20, 25, 32, ..., 300
 */
uint16_t Meter_GetDiameter(uint8_t dif_code);

#ifdef __cplusplus
}
#endif

#endif /* _METER_PROTOCOL_H_ */
