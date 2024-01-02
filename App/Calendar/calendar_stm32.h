/*******************************************************************************
 *  Author: shettyamar (shettyamar5@aol.com)
 *  Created: 03/Sep/2023 18:12:52
 *  FileName: calendar_stm32.h
 *  Description:
 ******************************************************************************/

/*
 *  Includes
 */
#ifndef _CALENDAR_STM32_H_
#define _CALENDAR_STM32_H_


/*
 *  Includes
 */
#include "calendar.h"
#include "stm32g0xx_hal.h"

/*
 *  Class Definition
 */

class CalendarSTM32 : public Calendar {
  public:
    static void init(RTC_HandleTypeDef *instance);
    void setTime(time_t time) override;
    void setDate(date_t date) override;

    Calendar::time_t getTime(void) override;
    Calendar::date_t getDate(void) override;

    static CalendarSTM32 *getInstance(void);

  private:
    CalendarSTM32();
};


#endif   // End of _CALENDAR_STM32_H_
