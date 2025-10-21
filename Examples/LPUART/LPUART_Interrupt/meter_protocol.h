/**
 *******************************************************************************
 * @file        meter_protocol.h
 * @author      Seoul Digital Water Meter Protocol Implementation
 * @brief       서울시 디지털계량기 프로토콜 V1.3 헤더 파일
 * @details     1200bps, 8-N-1 통신 프로토콜
 *******************************************************************************
 */

#ifndef _METER_PROTOCOL_H_
#define _METER_PROTOCOL_H_

#include "main_conf.h"

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

#ifdef __cplusplus
}
#endif

#endif /* _METER_PROTOCOL_H_ */
