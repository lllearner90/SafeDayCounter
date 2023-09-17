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

Calendar *Calendar::instance = nullptr;

Calendar *CalendarSTM32::getInstance(void) {
    if (!instance) {
        if (!rtc_instance) {
            return new CalendarSTM32();
        }
    }
    return nullptr;
}

CalendarSTM32::CalendarSTM32() {}

void CalendarSTM32::init(RTC_HandleTypeDef *instance) {
    rtc_instance = instance;
}

void CalendarSTM32::setTime(time_t time) {
    RTC_TimeTypeDef sTime = {0};

    sTime.Hours = binaryToBCD(time.hours);
    sTime.Minutes = binaryToBCD(time.minutes);
    sTime.Seconds = binaryToBCD(time.seconds);

    if (HAL_RTC_SetTime(rtc_instance, &sTime, RTC_FORMAT_BCD)) {
        // TODO: Throw an error
    }
}

void CalendarSTM32::setDate(date_t date) {
    RTC_DateTypeDef sDate = {0};

    sDate.Date = binaryToBCD(date.day);
    sDate.Month = binaryToBCD(date.month);
    sDate.Year = binaryToBCD(date.year);

    if (HAL_RTC_SetDate(rtc_instance, &sDate, RTC_FORMAT_BCD) != HAL_OK) {
        // TODO: Throw an error
    }
}
