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
// extern "C" {
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

    enum Months {
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
        Months  month;
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

    virtual time_t getTime(void) = 0;
    virtual date_t getDate(void) = 0;

    static Calendar *getInstance(void);
    friend bool      operator==(const date_t &lhs_date, const date_t &rhs_date);
    friend bool      operator!=(const date_t &lhs_date, const date_t &rhs_date);
    // FIXME: Create a function to compute the difference instead
    friend int  operator-(const date_t &lhs_date, const date_t &rhs_date);
    friend bool operator>(const date_t &lhs_date, const date_t &rhs_date);
    friend bool operator<(const date_t &lhs_date, const date_t &rhs_date);

  protected:
    date_t     date;
    time_t     time;
    static int getNumOfDays(const date_t &lhs_date, const date_t &rhs_date);
    static int getLeapYearCount(const date_t &date);
};

#ifdef __cplusplus
// }
#endif

#endif   // End of _CALENDAR_H_
