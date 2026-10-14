#pragma once
#include "types.h"

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} RTCDateTime;

void rtc_read(RTCDateTime *time);