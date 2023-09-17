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

/*
 *  Class Definition
 */

class Calendar {
  public:
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

    struct date_t {
        uint8_t day;
        uint8_t month;
        uint8_t year;
    };

    struct time_t {
        uint8_t hours;
        uint8_t minutes;
        uint8_t seconds;
    };

    struct calendar {
        date_t date;
    };


    // virtual void init() = 0;
    virtual void setTime(time_t time) = 0;
    virtual void setDate(date_t date) = 0;
    // virtual void setYear(year_t year) = 0;

    static Calendar *getInstance(void);

  protected:
    static date_t date;
    static time_t time;
    static Calendar *instance;

};

#ifdef __cplusplus
}
#endif

#endif   // End of _CALENDAR_H_
