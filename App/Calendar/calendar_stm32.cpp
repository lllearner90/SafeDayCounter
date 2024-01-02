/*******************************************************************************
 *  Author: shettyamar (shettyamar5@aol.com)
 *  Created: 03/Sep/2023 18:47:28
 *  FileName: calendar.cpp
 *  Description:
 ******************************************************************************/

/*
 *  Includes
 */
#include "calendar_stm32.h"
#include "bcd.h"

/*
 *  Private variables prototypes
 */

/*
 *  Private function prototypes
 */

/*
 *  Function Definitions
 */
extern RTC_HandleTypeDef  hrtc;
static RTC_HandleTypeDef *rtc_instance = &hrtc;
CalendarSTM32            *instance     = nullptr;

CalendarSTM32 *CalendarSTM32::getInstance(void) {
    if (nullptr == instance) {
        instance = new CalendarSTM32();
    }

    return instance;
}

CalendarSTM32::CalendarSTM32() {}

void CalendarSTM32::init(RTC_HandleTypeDef *instance) {
    rtc_instance = instance;
}

void CalendarSTM32::setTime(time_t time) {
    RTC_TimeTypeDef sTime = {0};

    sTime.Hours   = binaryToBCD(time.hours);
    sTime.Minutes = binaryToBCD(time.minutes);
    sTime.Seconds = binaryToBCD(time.seconds);

    if (HAL_RTC_SetTime(rtc_instance, &sTime, RTC_FORMAT_BIN)) {
        // TODO: Throw an error
    }
}

void CalendarSTM32::setDate(date_t date) {
    RTC_DateTypeDef sDate = {0};

    sDate.Date  = binaryToBCD(date.day);
    sDate.Month = binaryToBCD(static_cast<uint8_t>(date.month));
    sDate.Year  = binaryToBCD(date.year);

    if (HAL_RTC_SetDate(rtc_instance, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
        // TODO: Throw an error
    }
}

Calendar::time_t CalendarSTM32::getTime(void) {
    RTC_TimeTypeDef  sTime = {0};
    Calendar::time_t time  = {0};
    if (HAL_RTC_GetTime(rtc_instance, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
        // TODO: throw an error
    }
    time.hours   = sTime.Hours;
    time.minutes = sTime.Minutes;
    time.seconds = sTime.Seconds;
    return time;
}

Calendar::date_t CalendarSTM32::getDate(void) {
    RTC_DateTypeDef  sDate = {0};
    Calendar::date_t date  = {0};
    if (HAL_RTC_GetDate(rtc_instance, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
        // TODO: Throw an error
    }
    date.day   = sDate.Date;
    date.month = static_cast<Calendar::Months>(sDate.Month);
    date.year  = sDate.Year;
    return date;
}
