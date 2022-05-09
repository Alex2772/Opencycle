#include <cstring>
#include <Bicycle/Calendar.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal_rtc.h"

extern "C" RTC_HandleTypeDef hrtc;


void Calendar::set() {
    RTC_TimeTypeDef nativeTime;
    std::memset(&nativeTime, 0, sizeof(nativeTime));
    nativeTime.Hours = hours;
    nativeTime.Minutes = minutes;
    nativeTime.Seconds = seconds;
    HAL_RTC_SetTime(&hrtc, &nativeTime, RTC_FORMAT_BIN);
}
inline uint8_t RTC_Bcd2ToByte(uint8_t Value)
{
    uint32_t tmp = 0U;
    tmp = ((uint8_t)(Value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
    return (tmp + (Value & (uint8_t)0x0F));
}
Calendar Calendar::get() {
    Calendar out;
    auto tmpreg = (uint32_t)(RTC->TR & RTC_TR_RESERVED_MASK);
    out.hours = RTC_Bcd2ToByte((uint8_t)((tmpreg & (RTC_TR_HT | RTC_TR_HU)) >> 16U));
    out.minutes = RTC_Bcd2ToByte((uint8_t)((tmpreg & (RTC_TR_MNT | RTC_TR_MNU)) >> 8U));
    out.seconds = RTC_Bcd2ToByte((uint8_t)(tmpreg & (RTC_TR_ST | RTC_TR_SU)));

    return out;
}
