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

#ifdef __cplusplus
//extern "C" {
#endif

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

    void init(RTC_HandleTypeDef *instance);
    void setTime(time_t time) override;
    void setDate(date_t date) override;
    // void setYear(year_t year) override;
    static Calendar* getInstance(void);
  private:
    CalendarSTM32();
    static RTC_HandleTypeDef *rtc_instance;
};

#ifdef __cplusplus
//}
#endif

#endif   // End of _CALENDAR_STM32_H_
