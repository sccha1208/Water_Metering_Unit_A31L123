/**
 *******************************************************************************
 * @file        meter_protocol_parser.c
 * @author      Seoul Digital Water Meter Protocol Implementation
 * @brief       서울시 디지털계량기 프로토콜 V1.1~V1.4 통합 파서
 * @details     모든 버전 자동 감지 및 파싱 구현
 *******************************************************************************
 */

#include "meter_protocol.h"
#include "A31L12x_hal_debug_frmwrk.h"
#include "string.h"

//******************************************************************************
// 내부 함수 선언
//******************************************************************************

static bool Meter_ParseCommon(uint8_t* userdata, MeterData_t* out);
static bool Meter_ParseV11_Status(uint8_t status_byte, MeterData_t* out);
static bool Meter_ParseV12_Status(uint8_t status_byte, MeterData_t* out);
static bool Meter_ParseV13_Status(uint8_t status_byte, MeterData_t* out);
static bool Meter_ParseV14_Status(uint8_t status_byte, uint8_t s2vif_byte, MeterData_t* out);
static bool Meter_ParseUDF(uint8_t* udf_data, uint8_t udf_len, MeterData_t* out);

//******************************************************************************
// 통합 파싱 함수 구현
//******************************************************************************

/**
 * @brief 모든 버전 프레임 통합 파싱
 * @param frame 원본 프레임
 * @param length 프레임 길이
 * @param parsed_data 파싱 결과 저장 구조체
 * @return true: 파싱 성공, false: 파싱 실패
 */
bool Meter_ParseFrame(uint8_t* frame, uint16_t length, MeterData_t* parsed_data)
{
    // 초기화
    memset(parsed_data, 0, sizeof(MeterData_t));
    parsed_data->raw_frame = frame;
    parsed_data->raw_length = length;
    parsed_data->parse_success = false;

    // 버전 감지
    ProtocolVersion_t version = Meter_DetectVersion(frame, length);
    parsed_data->version = version;

    if (version == PROTOCOL_UNKNOWN)
    {
        return false;
    }

    // 프레임 구조 검증
    // 68 L L 68 C A CI [UserData] CS 16
    //  0 1 2  3 4 5  6    ...     -2 -1

    uint8_t L = frame[1];

    // 체크섬 검증
    uint8_t checksum_calc = 0;
    uint16_t i;

    // 체크섬 계산: C + A + CI + UserData
    for (i = 4; i < 4 + L; i++)
    {
        checksum_calc += frame[i];
    }

    parsed_data->checksum_calculated = checksum_calc;
    parsed_data->checksum_received = frame[4 + L];
    parsed_data->checksum_valid = (checksum_calc == parsed_data->checksum_received);

    if (!parsed_data->checksum_valid)
    {
        // 체크섬 오류지만 파싱은 계속 (디버깅용)
    }

    // 기본 필드 추출
    parsed_data->c_field = frame[4];
    parsed_data->a_field = frame[5];
    parsed_data->ci_field = frame[6];

    // UserData 포인터
    uint8_t* userdata = &frame[7];
    uint8_t userdata_len = L - 3;

    // 공통 파싱
    if (!Meter_ParseCommon(userdata, parsed_data))
    {
        return false;
    }

    // 버전별 Status 파싱
    uint8_t status_byte = userdata[5];  // MDH(1) + ID(4) + Status(1)

    switch (version)
    {
        case PROTOCOL_V1_1:
            Meter_ParseV11_Status(status_byte, parsed_data);
            break;

        case PROTOCOL_V1_2:
            Meter_ParseV12_Status(status_byte, parsed_data);
            break;

        case PROTOCOL_V1_3:
            Meter_ParseV13_Status(status_byte, parsed_data);
            break;

        case PROTOCOL_V1_4:
        {
            uint8_t s2vif_byte = userdata[7];  // S2&VIF
            Meter_ParseV14_Status(status_byte, s2vif_byte, parsed_data);
            break;
        }

        default:
            return false;
    }

    // UDF 파싱 (V1.2+)
    if (userdata_len > 12)
    {
        uint8_t* udf_data = &userdata[12];
        uint8_t udf_len = userdata_len - 12;
        Meter_ParseUDF(udf_data, udf_len, parsed_data);
    }

    parsed_data->parse_success = true;
    return true;
}

//******************************************************************************
// 내부 파싱 함수 구현
//******************************************************************************

/**
 * @brief 공통 필드 파싱
 * @param userdata UserData 포인터
 * @param out 출력 구조체
 * @return true: 성공, false: 실패
 */
static bool Meter_ParseCommon(uint8_t* userdata, MeterData_t* out)
{
    // UserData 구조:
    // MDH(1) + ID(4) + Status(1) + DIF(1) + VIF(1) + Data(4) [+ UDF(n)]
    //  0       1-4      5           6        7        8-11

    // MDH 확인 (0x0F 고정)
    if (userdata[0] != 0x0F)
    {
        return false;
    }

    // 기물번호 파싱 (BCD 4바이트)
    uint8_t id_bcd[4];
    id_bcd[0] = userdata[4];  // Little Endian
    id_bcd[1] = userdata[3];
    id_bcd[2] = userdata[2];
    id_bcd[3] = userdata[1];

    out->meter_id = Meter_BCD_To_Uint32(id_bcd);

    // DIF 파싱
    uint8_t dif = userdata[6];
    out->diameter_mm = Meter_GetDiameter(dif);

    // VIF 파싱 (또는 S2&VIF)
    uint8_t vif = userdata[7];
    out->decimal_point = vif & 0x0F;  // Bit 3-0: 소수점 위치

    // 검침값 파싱 (BCD 4바이트)
    out->reading_bcd[0] = userdata[8];   // Little Endian
    out->reading_bcd[1] = userdata[9];
    out->reading_bcd[2] = userdata[10];
    out->reading_bcd[3] = userdata[11];

    out->reading_value = Meter_BCD_To_Uint32(out->reading_bcd);

    return true;
}

/**
 * @brief V1.1 Status 파싱
 */
static bool Meter_ParseV11_Status(uint8_t status_byte, MeterData_t* out)
{
    // V1.1 Status Bit:
    // Bit 7: Q4 초과
    // Bit 6: 역류
    // Bit 5: 옥내누수
    // Bit 2: Batt.Low
    // Bit 1: 동파경고

    out->status.q3_exceed = (status_byte & 0x80) ? true : false;  // Bit 7 (V1.1은 Q4)
    out->status.reverse_flow = (status_byte & 0x40) ? true : false;
    out->status.indoor_leak = (status_byte & 0x20) ? true : false;

    out->status.ext.v11.batt_low = (status_byte & 0x04) ? true : false;  // Bit 2
    out->status.ext.v11.freeze_warning = (status_byte & 0x02) ? true : false;  // Bit 1

    return true;
}

/**
 * @brief V1.2 Status 파싱
 */
static bool Meter_ParseV12_Status(uint8_t status_byte, MeterData_t* out)
{
    // V1.2 Status Bit:
    // Bit 7: Q3 초과
    // Bit 6: 역류
    // Bit 5: 옥내누수
    // Bit 2: Batt.Low (3개월 전)

    out->status.q3_exceed = (status_byte & 0x80) ? true : false;
    out->status.reverse_flow = (status_byte & 0x40) ? true : false;
    out->status.indoor_leak = (status_byte & 0x20) ? true : false;

    out->status.ext.v12.batt_low = (status_byte & 0x04) ? true : false;  // Bit 2

    return true;
}

/**
 * @brief V1.3 Status 파싱
 */
static bool Meter_ParseV13_Status(uint8_t status_byte, MeterData_t* out)
{
    // V1.3 Status Bit:
    // Bit 7: Q3 초과
    // Bit 6: 역류
    // Bit 5: 옥내누수
    // Bit 4-0: 배터리 전압 정보 (5비트)

    out->status.q3_exceed = (status_byte & 0x80) ? true : false;
    out->status.reverse_flow = (status_byte & 0x40) ? true : false;
    out->status.indoor_leak = (status_byte & 0x20) ? true : false;

    out->status.ext.v13.batt_voltage = status_byte & 0x1F;  // Bit 4-0

    return true;
}

/**
 * @brief V1.4 Status 파싱
 */
static bool Meter_ParseV14_Status(uint8_t status_byte, uint8_t s2vif_byte, MeterData_t* out)
{
    // V1.4 Status1 Bit:
    // Bit 7: Q3 초과
    // Bit 6: 역류
    // Bit 5: 옥내누수
    // Bit 4-0: 배터리 전압 정보

    out->status.q3_exceed = (status_byte & 0x80) ? true : false;
    out->status.reverse_flow = (status_byte & 0x40) ? true : false;
    out->status.indoor_leak = (status_byte & 0x20) ? true : false;

    out->status.ext.v14.batt_voltage = status_byte & 0x1F;  // Bit 4-0

    // V1.4 Status2 (S2&VIF의 상위 비트):
    // Bit 7: 자석 감지
    // Bit 6: 동파경보

    out->status.ext.v14.magnet_detected = (s2vif_byte & 0x80) ? true : false;  // Bit 7
    out->status.ext.v14.freeze_warning = (s2vif_byte & 0x40) ? true : false;   // Bit 6

    return true;
}

/**
 * @brief UDF 파싱
 */
static bool Meter_ParseUDF(uint8_t* udf_data, uint8_t udf_len, MeterData_t* out)
{
    if (udf_len < 4)
    {
        out->has_udf = false;
        return false;
    }

    out->has_udf = true;

    // UDF 구조:
    // [Pro Ver] [Ver Mo] [Man Code High] [Man Code Low]
    //    0         1           2                3

    out->udf_protocol_ver = udf_data[0];  // 0x12, 0x13, 0x14
    out->udf_verification_month = udf_data[1];  // 1~12

    // 제조사 코드 (Big Endian)
    out->udf_manufacturer_code = (udf_data[2] << 8) | udf_data[3];

    return true;
}
