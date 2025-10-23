/**
 *******************************************************************************
 * @file        meter_protocol.c
 * @author      Seoul Digital Water Meter Protocol Implementation
 * @brief       서울시 디지털계량기 프로토콜 V1.3 구현
 * @details     1200bps, 8-N-1 통신 프로토콜
 *******************************************************************************
 */

#include "meter_protocol.h"
#include "string.h"

//******************************************************************************
// 전역 변수
//******************************************************************************

static METER_CONTEXT_Type g_meter_ctx;

// 수신 상태 관리 변수 (정적 변수 문제 해결)
// 0: 대기, 1: 0x68 감지 후 데이터 수신 중
static uint8_t g_rx_state = 0;

// 외부 함수 참조 (main.c에서 정의)
extern uint32_t rbSend(uint8_t* txbuf, uint8_t buflen);
extern uint32_t rbReceive(uint8_t* rxbuf, uint8_t buflen);

//******************************************************************************
// 내부 함수 선언
//******************************************************************************

//static void Meter_StateMachine(void);
static uint32_t Meter_GetTick(void);  // 시스템 틱 가져오기 (구현 필요)

//******************************************************************************
// 공용 함수 구현
//******************************************************************************

/**
 * @brief 계량기 프로토콜 초기화
 */
void Meter_Init(void)
{
    memset(&g_meter_ctx, 0, sizeof(METER_CONTEXT_Type));
    g_meter_ctx.state = METER_STATE_IDLE;
    g_meter_ctx.last_error = METER_ERR_NONE;
}

/**
 * @brief 응답 수신 콜백 함수 설정
 */
void Meter_SetResponseCallback(void (*callback)(uint8_t* data, uint16_t length))
{
    g_meter_ctx.on_response_received = callback;
}

/**
 * @brief 에러 콜백 함수 설정
 */
void Meter_SetErrorCallback(void (*callback)(METER_ERROR_Type error))
{
    g_meter_ctx.on_error = callback;
}

/**
 * @brief 체크섬 계산 (XOR)
 * @param data 데이터 포인터
 * @param length 데이터 길이
 * @return 체크섬 값
 */
uint8_t Meter_CalculateChecksum(uint8_t* data, uint16_t length)
{
    uint8_t checksum = 0;
    uint16_t i;

    for (i = 0; i < length; i++)
    {
        checksum ^= data[i];
    }

    return checksum;
}

/**
 * @brief Preamble 전송 (20ms High Level)
 * @details TTL High Level을 20ms 동안 유지하여 통신 시작 신호 전송
 * @note 서울시 디지털계량기 프로토콜 요구사항:
 *       통신 시작 전 TX 라인을 High로 20ms 유지
 */
void Meter_SendPreamble(void)
{
    volatile uint32_t delay;

    // UART TX는 기본적으로 IDLE 상태에서 High이므로
    // 20ms 대기만 하면 됨 (실제 전송 전 안정화 시간)

    // 20ms 지연
    // SystemCoreClock = 32MHz 기준
    // 32,000,000 Hz / 1000 * 20 = 640,000 사이클
    // 실제로는 루프 오버헤드 고려하여 조정
    for (delay = 0; delay < METER_PREAMBLE_DELAY_CYCLES; delay++)
    {
        __NOP();
    }
}

/**
 * @brief 프레임 유효성 검사
 * @param frame 프레임 포인터
 * @return 1: 유효, 0: 무효
 */
uint8_t Meter_ValidateFrame(METER_FRAME_Type* frame)
{
    uint8_t calculated_checksum;

    // HEADER 확인
    // TX 프레임: 0x10
    // RX 프레임: 0x68 (시작 바이트)
    if (frame->header != METER_FRAME_HEADER_TX &&
        frame->header != METER_FRAME_START_RX)
    {
        // 알려지지 않은 헤더도 일단 허용 (디버깅 용도)
        // return 0;
    }

    // 체크섬 확인 (HEADER + CMD + LEN + DATA의 XOR)
    calculated_checksum = Meter_CalculateChecksum((uint8_t*)frame,
                                                   3 + frame->length); // HEADER + CMD + LEN + DATA

    if (calculated_checksum != frame->checksum)
    {
        return 0;
    }

    return 1;
}

/**
 * @brief 커맨드 전송
 * @param addr 계량기 주소 (사용 안 함)
 * @param cmd 커맨드 코드
 * @param data 전송할 데이터
 * @param length 데이터 길이
 * @return 에러 코드
 */
METER_ERROR_Type Meter_SendCommand(uint8_t addr, uint8_t cmd, uint8_t* data, uint8_t length)
{
    uint16_t frame_length = 0;
    uint16_t i;

    // 파라미터 검사
    if (length > (METER_MAX_FRAME_SIZE - 4))
    {
        return METER_ERR_INVALID_PARAM;
    }

    // 프로토콜이 IDLE 상태인지 확인
    if (g_meter_ctx.state != METER_STATE_IDLE)
    {
        return METER_ERR_INVALID_PARAM;
    }

    // 프레임 생성: [HEADER] [CMD] [LEN] [DATA...] [CHECKSUM]
    // 예시: 10-5B-01-5C-16
    g_meter_ctx.tx_buffer[frame_length++] = METER_FRAME_HEADER_TX;  // 0x10
    g_meter_ctx.tx_buffer[frame_length++] = cmd;                    // 0x5B
    g_meter_ctx.tx_buffer[frame_length++] = length;                 // 0x01

    // 데이터 복사
    if (data != NULL && length > 0)
    {
        for (i = 0; i < length; i++)
        {
            g_meter_ctx.tx_buffer[frame_length++] = data[i];    // 0x5C
        }
    }

    // 체크섬 계산 (HEADER + CMD + LEN + DATA의 XOR)
    // 예: 0x10 ^ 0x5B ^ 0x01 ^ 0x5C = 0x16
    g_meter_ctx.tx_buffer[frame_length] = Meter_CalculateChecksum(g_meter_ctx.tx_buffer, frame_length);
    frame_length++;

    g_meter_ctx.tx_length = frame_length;
    g_meter_ctx.tx_index = 0;

    // Preamble 전송 전 LPUART TX 인터럽트 비활성화
    HAL_LPUART_ConfigInterrupt(LPUART_INTCFG_TXCIEN, DISABLE);

    // Preamble 전송
    g_meter_ctx.state = METER_STATE_PREAMBLE;
    Meter_SendPreamble();

    // LPUART TX FIFO 클리어 및 안정화
    {
        volatile uint32_t delay;

        // TX 인터럽트 완전 비활성화
        HAL_LPUART_ConfigInterrupt(LPUART_INTCFG_TXCIEN, DISABLE);

        // LPUART를 일시적으로 비활성화 후 재활성화 (FIFO 클리어)
        HAL_LPUART_Enable(DISABLE);
        for (delay = 0; delay < METER_FIFO_CLEAR_DELAY; delay++) { __NOP(); }
        HAL_LPUART_Enable(ENABLE);

        // 추가 안정화 딜레이 (첫 바이트 손실 방지)
        for (delay = 0; delay < METER_TX_STABILIZE_DELAY; delay++) { __NOP(); }
    }

    // 디버그: 전송 프레임 출력 (주석 처리 - 필요시 활성화)
    /*
    {
        uint16_t i;
        _DBG("TX Frame: ");
        for (i = 0; i < g_meter_ctx.tx_length; i++)
        {
            _DBH(g_meter_ctx.tx_buffer[i]);
            _DBG(" ");
        }
        _DBG("\n\r");
    }
    */

    // 데이터 전송 (BLOCKING 모드로 한 번에 전송)
    // 계량기가 프레임을 인식하려면 바이트 간 간격 없이 연속으로 전송해야 함
    g_meter_ctx.state = METER_STATE_TX;
    HAL_LPUART_Transmit(g_meter_ctx.tx_buffer, g_meter_ctx.tx_length, BLOCKING);

    // 전송 완료 대기 (추가 딜레이로 안정성 확보)
    {
        volatile uint32_t delay;
        // 5바이트 전송 시간: 5 * 10 bits / 1200 bps = ~41.7ms
        // 여유를 두고 50ms 대기
        for (delay = 0; delay < METER_TX_COMPLETE_DELAY; delay++)  // 약 50ms
        {
            __NOP();
        }
    }

    // RX 인터럽트 재활성화 (응답 수신 준비)
    HAL_LPUART_ConfigInterrupt(LPUART_INTCFG_RXCIEN, ENABLE);

    // 응답 대기 상태로 전환
    g_meter_ctx.state = METER_STATE_WAIT_RESPONSE;
    g_meter_ctx.timeout_ms = Meter_GetTick() + METER_RESPONSE_TIMEOUT_MS;
    g_meter_ctx.rx_index = 0;
    g_meter_ctx.rx_length = 0;

    return METER_ERR_NONE;
}

/**
 * @brief ACK 전송
 */
METER_ERROR_Type Meter_SendACK(void)
{
    uint8_t ack = METER_ACK;
    rbSend(&ack, 1);
    return METER_ERR_NONE;
}

/**
 * @brief NAK 전송
 */
METER_ERROR_Type Meter_SendNAK(void)
{
    uint8_t nak = METER_NAK;
    rbSend(&nak, 1);
    return METER_ERR_NONE;
}

/**
 * @brief 수신 데이터 처리 (0x68 트리거 기반)
 * @param data 수신 데이터
 * @param length 데이터 길이
 *
 * @details 프레임 구조: 68 L L 68 [DATA] CS 16
 *          - 0x68을 트리거로 프레임 시작 감지
 *          - L 바이트 만큼 데이터 수신
 *          - 0x16으로 프레임 종료
 */
void Meter_ProcessReceive(uint8_t* data, uint16_t length)
{
    uint16_t i;

    for (i = 0; i < length; i++)
    {
        uint8_t byte = data[i];

        // 0x68 트리거 감지 (프레임 시작)
        if (byte == METER_FRAME_START_RX && g_rx_state == 0)
        {
            // 프레임 시작
            g_meter_ctx.rx_index = 0;
            g_meter_ctx.rx_buffer[g_meter_ctx.rx_index++] = byte;
            g_rx_state = 1;
            g_meter_ctx.state = METER_STATE_RX;
            continue;
        }

        // 수신 중 상태
        if (g_rx_state > 0)
        {
            // 버퍼 오버플로우 체크
            if (g_meter_ctx.rx_index >= METER_MAX_FRAME_SIZE)
            {
                g_meter_ctx.last_error = METER_ERR_BUFFER_OVERFLOW;
                g_meter_ctx.state = METER_STATE_ERROR;
                if (g_meter_ctx.on_error != NULL)
                {
                    g_meter_ctx.on_error(METER_ERR_BUFFER_OVERFLOW);
                }
                g_rx_state = 0;  // 상태 초기화
                g_meter_ctx.rx_index = 0;
                return;
            }

            g_meter_ctx.rx_buffer[g_meter_ctx.rx_index++] = byte;

            // 0x16 종료 바이트 감지
            if (byte == METER_FRAME_END_RX && g_meter_ctx.rx_index > 6)
            {
                // 프레임 완성
                g_meter_ctx.rx_length = g_meter_ctx.rx_index;
                g_meter_ctx.state = METER_STATE_COMPLETE;

                // 콜백 호출 (전체 프레임 전달)
                if (g_meter_ctx.on_response_received != NULL)
                {
                    g_meter_ctx.on_response_received(g_meter_ctx.rx_buffer, g_meter_ctx.rx_length);
                }

                // 상태 초기화
                g_rx_state = 0;
                g_meter_ctx.state = METER_STATE_IDLE;
                g_meter_ctx.rx_index = 0;
                return;
            }
        }
    }
}

/**
 * @brief 수신 상태 리셋
 */
static void Meter_ResetRxState(void)
{
    g_rx_state = 0;
}

/**
 * @brief 주기적으로 호출해야 하는 타스크 함수
 */
void Meter_Task(void)
{
    uint8_t rx_buf[32];
    uint32_t len;

    // 수신 데이터 확인
    len = rbReceive(rx_buf, sizeof(rx_buf));
    if (len > 0)
    {
        Meter_ProcessReceive(rx_buf, len);
    }

    // 타임아웃 체크
    if (g_meter_ctx.state == METER_STATE_WAIT_RESPONSE)
    {
        if (Meter_GetTick() >= g_meter_ctx.timeout_ms)
        {
            g_meter_ctx.last_error = METER_ERR_TIMEOUT;
            g_meter_ctx.state = METER_STATE_ERROR;

            if (g_meter_ctx.on_error != NULL)
            {
                g_meter_ctx.on_error(METER_ERR_TIMEOUT);
            }

            // 재전송 시도
            if (g_meter_ctx.retry_count < METER_MAX_RETRY)
            {
                g_meter_ctx.retry_count++;
                g_meter_ctx.state = METER_STATE_IDLE;

                // 재전송 로직: 마지막 전송 프레임을 다시 전송
                if (g_meter_ctx.tx_length > 0)
                {
                    // Preamble 전송
                    Meter_SendPreamble();

                    // FIFO 클리어
                    HAL_LPUART_ConfigInterrupt(LPUART_INTCFG_TXCIEN, DISABLE);
                    HAL_LPUART_Enable(DISABLE);
                    {
                        volatile uint32_t delay;
                        for (delay = 0; delay < METER_FIFO_CLEAR_DELAY; delay++) { __NOP(); }
                    }
                    HAL_LPUART_Enable(ENABLE);

                    // 재전송
                    g_meter_ctx.state = METER_STATE_TX;
                    HAL_LPUART_Transmit(g_meter_ctx.tx_buffer, g_meter_ctx.tx_length, BLOCKING);

                    // 전송 완료 대기
                    {
                        volatile uint32_t delay;
                        for (delay = 0; delay < METER_TX_COMPLETE_DELAY; delay++) { __NOP(); }
                    }

                    // 응답 대기 상태로 전환
                    g_meter_ctx.state = METER_STATE_WAIT_RESPONSE;
                    g_meter_ctx.timeout_ms = Meter_GetTick() + METER_RESPONSE_TIMEOUT_MS;
                    HAL_LPUART_ConfigInterrupt(LPUART_INTCFG_RXCIEN, ENABLE);
                }
            }
            else
            {
                // 최대 재전송 횟수 초과
                g_meter_ctx.state = METER_STATE_IDLE;
                g_meter_ctx.retry_count = 0;
            }
        }
    }
}

/**
 * @brief 현재 상태 조회
 */
METER_STATE_Type Meter_GetState(void)
{
    return g_meter_ctx.state;
}

/**
 * @brief 마지막 에러 조회
 */
METER_ERROR_Type Meter_GetLastError(void)
{
    return g_meter_ctx.last_error;
}

/**
 * @brief 프로토콜 리셋
 */
void Meter_Reset(void)
{
    g_meter_ctx.state = METER_STATE_IDLE;
    g_meter_ctx.rx_index = 0;
    g_meter_ctx.rx_length = 0;
    g_meter_ctx.tx_index = 0;
    g_meter_ctx.tx_length = 0;
    g_meter_ctx.retry_count = 0;
    g_meter_ctx.last_error = METER_ERR_NONE;

    // 수신 상태도 리셋
    Meter_ResetRxState();
}

/**
 * @brief 수신 상태 리셋 (내부 함수)
 */
static void Meter_ResetRxState(void);  // Forward declaration

//******************************************************************************
// 내부 함수 구현
//******************************************************************************

// SysTick 기반 밀리초 카운터 (전역 변수)
static volatile uint32_t g_systick_ms = 0;

/**
 * @brief SysTick 인터럽트 핸들러에서 호출 (1ms마다)
 * @note A31L12x_it.c의 SysTick_Handler()에서 이 함수를 호출해야 함
 */
void Meter_SysTick_Increment(void)
{
    g_systick_ms++;
}

/**
 * @brief 시스템 틱 가져오기 (밀리초 단위)
 * @return 시스템 시작 후 경과 시간 (ms)
 */
static uint32_t Meter_GetTick(void)
{
    return g_systick_ms;
}
