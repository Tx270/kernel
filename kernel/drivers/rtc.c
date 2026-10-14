#include "../include/rtc.h"
#include "../include/io.h"

#define RTC_ADDRESS 0x70
#define RTC_DATA    0x71

#define RTC_SECONDS 0x00
#define RTC_MINUTES 0x02
#define RTC_HOURS   0x04
#define RTC_DAY     0x07
#define RTC_MONTH   0x08
#define RTC_YEAR    0x09
#define RTC_STATUS_B 0x0B

static uint8_t rtc_read_register(uint8_t reg)
{
    outb(RTC_ADDRESS, reg);
    return inb(RTC_DATA);
}

static uint8_t bcd_to_binary(uint8_t value)
{
    return (value & 0x0F) + ((value >> 4) * 10);
}

void rtc_read(RTCDateTime *time)
{
    uint8_t status_b = rtc_read_register(RTC_STATUS_B);

    time->second = rtc_read_register(RTC_SECONDS);
    time->minute = rtc_read_register(RTC_MINUTES);
    time->hour   = rtc_read_register(RTC_HOURS);
    time->day    = rtc_read_register(RTC_DAY);
    time->month  = rtc_read_register(RTC_MONTH);
    time->year   = rtc_read_register(RTC_YEAR);

    if (!(status_b & 0x04)) {
        time->second = bcd_to_binary(time->second);
        time->minute = bcd_to_binary(time->minute);
        time->hour   = (time->hour & 0x80) |
                       bcd_to_binary(time->hour & 0x7F);
        time->day    = bcd_to_binary(time->day);
        time->month  = bcd_to_binary(time->month);
        time->year   = bcd_to_binary(time->year);
    }

    if (!(status_b & 0x02)) {
        uint8_t pm = time->hour & 0x80;
        time->hour &= 0x7F;
        time->hour = time->hour % 12;
        if (pm)
            time->hour += 12;
    }
}