/*******************************************************************************
 *  Author: shettyamar (shettyamar5@aol.com)
 *  Created: 03/Sep/2023 18:12:52
 *  FileName: calendar.h
 *  Description:
 ******************************************************************************/

/*
 *  Includes
 */
#ifndef _CALENDAR_H_
#define _CALENDAR_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Includes
 */
#include <stdint.h>

#include "stm32g0xx_hal_rtc.h"

/*
 *  Class Definition
 */

class Calendar {
  public:
    Calendar(RTC_HandleTypeDef *const sys_rtc_instance);
    ~Calendar();

    enum class DaysOfWeek { MONDAY = 1, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY };

    enum class Months {
        JANUARY = 1,
        FEBRUARY,
        MARCH,
        APRIL,
        MAY,
        JUNE,
        JULY,
        AUGUST,
        SEPTEMBER,
        OCTOBER,
        NOVEMBER,
        DECEMBER
    };

    typedef struct date_t {
        uint8_t day;
        uint8_t month;
        uint8_t year;
    };

    typedef struct time_t {
        uint8_t hours;
        uint8_t minutes;
        uint8_t seconds;
    };

    typedef struct calendar {
        date_t date;
    };

    void Init(RTC_HandleTypeDef *const sys_rtc_instance);

    void SetTime(time_t time);
    void SetDate(date_t date);

  private:
};   // namespace Calendar

#ifdef __cplusplus
}
#endif

#endif   // End of _CALENDAR_H_
