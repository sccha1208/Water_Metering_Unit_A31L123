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

//******************************************************************************
// 범용 프로토콜 파서 구현 (V1.1~V1.4 지원)
//******************************************************************************

/**
 * @brief 프로토콜 버전 자동 감지 (하이브리드 방식)
 * @param frame 원본 프레임 (68 L L 68 C A CI [UserData] CS 16)
 * @param length 프레임 길이
 * @return 감지된 프로토콜 버전
 *
 * @details 감지 알고리즘:
 *   1단계: L Field 확인 (프레임 유효성 검증)
 *   2단계: UserData 길이 계산
 *   3단계: UDF 유무 확인
 *   4단계: UDF 프로토콜 버전 필드 확인
 *   5단계: Status2 존재 확인 (V1.4 특징)
 */
ProtocolVersion_t Meter_DetectVersion(uint8_t* frame, uint16_t length)
{
    // 최소 프레임 길이 검증
    // 68 L L 68 C A CI [최소 UserData] CS 16
    //  0 1 2  3 4 5  6       ...        -2 -1
    if (length < 12)  // 최소: 6(헤더) + 최소UserData(4) + 2(CS+END)
    {
        return PROTOCOL_UNKNOWN;
    }

    // 프레임 시작/종료 바이트 확인
    if (frame[0] != 0x68 || frame[3] != 0x68 || frame[length - 1] != 0x16)
    {
        return PROTOCOL_UNKNOWN;
    }

    // L Field 추출
    uint8_t L = frame[1];

    // L Field 일치성 확인
    if (L != frame[2])
    {
        return PROTOCOL_UNKNOWN;
    }

    // L Field 범위 확인
    if (L < 0x0B || L > 0xFC)  // L = C + A + CI + UserData
    {
        return PROTOCOL_UNKNOWN;
    }

    // UserData 길이 계산
    // L = C(1) + A(1) + CI(1) + UserData(n)
    // UserData = L - 3
    uint8_t userdata_len = L - 3;

    // UserData 포인터
    uint8_t* userdata = &frame[7];  // CI Field 다음부터 UserData

    // 기본 UserData 구조:
    // MDH(1) + ID(4) + Status(1) + DIF(1) + VIF(1) + Data(4) = 12 bytes (최소)

    if (userdata_len < 12)
    {
        return PROTOCOL_UNKNOWN;  // 최소 길이 미달
    }

    // ========================================
    // 1단계: UDF 유무 확인
    // ========================================
    // V1.1: UserData = 12 bytes (UDF 없음)
    // V1.2+: UserData >= 12 bytes (UDF 포함 가능)

    if (userdata_len == 12)
    {
        // UDF 없음 → V1.1 가능성 높음
        // 추가 검증: Status Bit 1 확인 (V1.1은 동파경고 사용)
        uint8_t status = userdata[5];  // MDH(1) + ID(4) + Status(1)

        // V1.1 특징: Bit 1이 동파경고로 사용될 가능성
        // V1.2+: Bit 1은 reserved (0)
        // 하지만 확정 불가하므로 V1.1로 판단
        return PROTOCOL_V1_1;
    }

    // ========================================
    // 2단계: UDF 프로토콜 버전 확인
    // ========================================
    // UDF 시작 위치: MDH(1) + ID(4) + Status(1) + DIF(1) + VIF(1) + Data(4) = 12
    uint8_t udf_offset = 12;

    if (userdata_len >= udf_offset + 4)  // UDF 최소 4바이트 (Pro Ver + Ver Mo + Man Code)
    {
        // UDF[0]: Protocol Version
        uint8_t proto_ver = userdata[udf_offset];

        if (proto_ver == 0x12)
        {
            return PROTOCOL_V1_2;
        }
        else if (proto_ver == 0x13)
        {
            return PROTOCOL_V1_3;
        }
        else if (proto_ver == 0x14)
        {
            return PROTOCOL_V1_4;
        }
    }

    // ========================================
    // 3단계: Status2 존재 확인 (V1.4 특징)
    // ========================================
    // V1.4: VIF 위치에 S2&VIF 결합
    // S2&VIF의 Bit 7~6이 Status2 (자석감지, 동파경보)
    // VIF만 있는 경우: Bit 7~4는 단위, Bit 3~0은 소수점
    //   → 단위=1(m³)이므로 Bit 7~4 = 0001 → 상위 비트는 0

    uint8_t vif_byte = userdata[7];  // VIF 또는 S2&VIF

    // V1.4 판단: Bit 7 또는 Bit 6이 1인 경우 (자석감지/동파경보 가능)
    if ((vif_byte & 0xC0) != 0)
    {
        // Status2가 포함된 것으로 추정 → V1.4
        return PROTOCOL_V1_4;
    }

    // ========================================
    // 4단계: 기본값
    // ========================================
    // UDF가 있지만 버전 정보가 명확하지 않은 경우
    // V1.2로 간주 (가장 일반적)
    return PROTOCOL_V1_2;
}

/**
 * @brief BCD 4바이트를 uint32로 변환
 * @param bcd BCD 배열 [4] (Little Endian)
 * @return 변환된 정수
 *
 * @example
 *   BCD: {0x78, 0x56, 0x34, 0x12}
 *   Result: 12345678
 */
uint32_t Meter_BCD_To_Uint32(uint8_t bcd[4])
{
    uint32_t result = 0;
    uint8_t i;

    // Little Endian: bcd[3] bcd[2] bcd[1] bcd[0]
    for (i = 0; i < 4; i++)
    {
        uint8_t byte_val = bcd[3 - i];  // 역순으로 읽기
        uint8_t high_nibble = (byte_val >> 4) & 0x0F;
        uint8_t low_nibble = byte_val & 0x0F;

        // BCD 유효성 검사
        if (high_nibble > 9 || low_nibble > 9)
        {
            return 0;  // 잘못된 BCD
        }

        result = result * 100 + (high_nibble * 10 + low_nibble);
    }

    return result;
}

/**
 * @brief 배터리 전압 코드를 실제 전압(V x 10)으로 변환
 * @param voltage_code 0~31 (5비트)
 * @return 전압 (V x 10), 예: 36 → 3.6V
 *
 * @details
 *   00000 (0)  = 3.7V 이상  → 37
 *   00001 (1)  = 3.6~3.7V   → 36
 *   00010 (2)  = 3.5~3.6V   → 35
 *   ...
 *   11111 (31) = 0.7V 미만  → 6
 */
uint8_t Meter_GetBatteryVoltage_x10(uint8_t voltage_code)
{
    // 코드 범위 검증
    if (voltage_code > 31)
    {
        return 0;  // 잘못된 코드
    }

    // 변환 공식: Voltage = 3.7V - (code * 0.1V)
    // 반환값은 10배: (37 - code)
    return (37 - voltage_code);
}

/**
 * @brief 구경 코드를 mm 단위로 변환
 * @param dif_code DIF의 상위 4비트 (Bit 7~4)
 * @return 구경 (mm)
 *
 * @details 변환 테이블:
 *   0x1 → 15mm
 *   0x2 → 20mm
 *   0x3 → 25mm
 *   0x4 → 32mm
 *   0x5 → 40mm
 *   0x6 → 50mm
 *   0x7 → 80mm
 *   0x8 → 100mm
 *   0x9 → 150mm
 *   0xA → 200mm
 *   0xB → 250mm
 *   0xC → 300mm
 */
uint16_t Meter_GetDiameter(uint8_t dif_code)
{
    // DIF의 상위 4비트 추출
    uint8_t code = (dif_code >> 4) & 0x0F;

    // 변환 테이블 (uint16_t로 변경 - 300mm 지원)
    const uint16_t diameter_table[] = {
        0,    // 0: 사용 안 함
        15,   // 1
        20,   // 2
        25,   // 3
        32,   // 4
        40,   // 5
        50,   // 6
        80,   // 7
        100,  // 8
        150,  // 9
        200,  // A
        250,  // B
        300   // C
    };

    if (code >= sizeof(diameter_table))
    {
        return 0;  // 잘못된 코드
    }

    return diameter_table[code];
}

/**
 * @brief 파싱된 계량기 데이터를 디버그 포트로 출력
 * @param data 파싱된 데이터 구조체
 * @note UART1 디버그 포트(PB0:TX, PB1:RX)로 출력
 */
void Meter_PrintParsedData(MeterData_t* data)
{
    if (data == NULL)
    {
        _DBG("[ERROR] NULL pointer\r\n");
        return;
    }

    _DBG("\r\n");
    _DBG("========================================\r\n");
    _DBG("       Seoul Water Meter Protocol      \r\n");
    _DBG("========================================\r\n");

    // 파싱 결과
    if (data->parse_success)
    {
        _DBG("[PARSE] Success\r\n");
    }
    else
    {
        _DBG("[PARSE] Failed\r\n");
        return;  // 실패 시 여기서 종료
    }

    // 프로토콜 버전
    _DBG("[VERSION] ");
    switch (data->version)
    {
        case PROTOCOL_V1_1:
            _DBG("V1.1 (2013.06)\r\n");
            break;
        case PROTOCOL_V1_2:
            _DBG("V1.2 (2021.05)\r\n");
            break;
        case PROTOCOL_V1_3:
            _DBG("V1.3 (2023.01)\r\n");
            break;
        case PROTOCOL_V1_4:
            _DBG("V1.4 (2024.07)\r\n");
            break;
        default:
            _DBG("Unknown\r\n");
            break;
    }

    // 체크섬 검증
    _DBG("[CHECKSUM] ");
    if (data->checksum_valid)
    {
        _DBG("OK (0x");
        _DBH(data->checksum_received);
        _DBG(")\r\n");
    }
    else
    {
        _DBG("FAIL (Recv:0x");
        _DBH(data->checksum_received);
        _DBG(" Calc:0x");
        _DBH(data->checksum_calculated);
        _DBG(")\r\n");
    }

    // 기본 필드
    _DBG("[C FIELD] 0x");
    _DBH(data->c_field);
    _DBG("\r\n");

    _DBG("[A FIELD] 0x");
    _DBH(data->a_field);
    _DBG("\r\n");

    _DBG("[CI FIELD] 0x");
    _DBH(data->ci_field);
    _DBG("\r\n");

    // 기물번호
    _DBG("[METER ID] ");
    cprintf("%08lu\r\n", data->meter_id);

    // 계량기 구경
    _DBG("[DIAMETER] ");
    _DBD32(data->diameter_mm);
    _DBG(" mm\r\n");

    // 검침값 (BCD 원본)
    _DBG("[READING BCD] ");
    _DBH(data->reading_bcd[3]);  // Big Endian 출력
    _DBG(" ");
    _DBH(data->reading_bcd[2]);
    _DBG(" ");
    _DBH(data->reading_bcd[1]);
    _DBG(" ");
    _DBH(data->reading_bcd[0]);
    _DBG("\r\n");

    // 검침값 (변환 결과)
    _DBG("[READING VALUE] ");
    cprintf("%lu", data->reading_value);
    _DBG(" (Decimal Point: ");
    _DBD32(data->decimal_point);
    _DBG(")\r\n");

    // 실제 검침값 계산 및 출력 (소수점 적용)
    _DBG("[ACTUAL VALUE] ");
    if (data->decimal_point == 2)
    {
        // 소수점 2자리 (예: 12345.67)
        uint32_t integer_part = data->reading_value / 100;
        uint32_t decimal_part = data->reading_value % 100;
        cprintf("%lu.%02lu m3\r\n", integer_part, decimal_part);
    }
    else if (data->decimal_point == 3)
    {
        // 소수점 3자리 (예: 12345.678)
        uint32_t integer_part = data->reading_value / 1000;
        uint32_t decimal_part = data->reading_value % 1000;
        cprintf("%lu.%03lu m3\r\n", integer_part, decimal_part);
    }
    else
    {
        cprintf("%lu m3\r\n", data->reading_value);
    }

    // 상태 정보
    _DBG("========================================\r\n");
    _DBG("[STATUS]\r\n");

    // 공통 상태
    _DBG("  Q3 Exceed:    ");
    _DBG(data->status.q3_exceed ? "YES\r\n" : "NO\r\n");

    _DBG("  Reverse Flow: ");
    _DBG(data->status.reverse_flow ? "YES\r\n" : "NO\r\n");

    _DBG("  Indoor Leak:  ");
    _DBG(data->status.indoor_leak ? "YES\r\n" : "NO\r\n");

    // 버전별 상태
    switch (data->version)
    {
        case PROTOCOL_V1_1:
            _DBG("  Batt Low:     ");
            _DBG(data->status.ext.v11.batt_low ? "YES\r\n" : "NO\r\n");

            _DBG("  Freeze Warn:  ");
            _DBG(data->status.ext.v11.freeze_warning ? "YES (-2C 3hr)\r\n" : "NO\r\n");
            break;

        case PROTOCOL_V1_2:
            _DBG("  Batt Low:     ");
            _DBG(data->status.ext.v12.batt_low ? "YES (3 months)\r\n" : "NO\r\n");
            break;

        case PROTOCOL_V1_3:
            _DBG("  Batt Voltage: ");
            {
                uint8_t voltage_x10 = Meter_GetBatteryVoltage_x10(data->status.ext.v13.batt_voltage);
                cprintf("%u.%u V (Code:%u)\r\n",
                        voltage_x10 / 10,
                        voltage_x10 % 10,
                        data->status.ext.v13.batt_voltage);
            }
            break;

        case PROTOCOL_V1_4:
            _DBG("  Batt Voltage: ");
            {
                uint8_t voltage_x10 = Meter_GetBatteryVoltage_x10(data->status.ext.v14.batt_voltage);
                cprintf("%u.%u V (Code:%u)\r\n",
                        voltage_x10 / 10,
                        voltage_x10 % 10,
                        data->status.ext.v14.batt_voltage);
            }

            _DBG("  Magnet Det:   ");
            _DBG(data->status.ext.v14.magnet_detected ? "YES (5 min)\r\n" : "NO\r\n");

            _DBG("  Freeze Warn:  ");
            _DBG(data->status.ext.v14.freeze_warning ? "YES (0C 1min)\r\n" : "NO\r\n");
            break;

        default:
            break;
    }

    // UDF 정보 (V1.2+)
    if (data->has_udf)
    {
        _DBG("========================================\r\n");
        _DBG("[UDF]\r\n");

        _DBG("  Protocol Ver: 0x");
        _DBH(data->udf_protocol_ver);
        _DBG("\r\n");

        _DBG("  Verify Month: ");
        _DBD32(data->udf_verification_month);
        _DBG("\r\n");

        _DBG("  Manufact Code: 0x");
        _DBH((uint8_t)(data->udf_manufacturer_code >> 8));
        _DBH((uint8_t)(data->udf_manufacturer_code & 0xFF));
        _DBG("\r\n");
    }

    // 원본 프레임 (HEX)
    _DBG("========================================\r\n");
    _DBG("[RAW FRAME] (");
    _DBD32(data->raw_length);
    _DBG(" bytes)\r\n");

    if (data->raw_frame != NULL)
    {
        uint16_t i;
        for (i = 0; i < data->raw_length; i++)
        {
            _DBH(data->raw_frame[i]);
            _DBG(" ");

            // 16바이트마다 줄바꿈
            if ((i + 1) % 16 == 0)
            {
                _DBG("\r\n");
            }
        }

        // 마지막 줄이 16바이트가 아닌 경우 줄바꿈
        if (data->raw_length % 16 != 0)
        {
            _DBG("\r\n");
        }
    }

    _DBG("========================================\r\n");
    _DBG("\r\n");
}
