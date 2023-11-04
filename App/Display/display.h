/*******************************************************************************
 *  Author: shettyamar (shettyamar5@aol.com)
 *  Created: 03/Sep/2023 17:53:18
 *  FileName: display.h
 *  Description:
 ******************************************************************************/

/*
 *  Includes
 */
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/*
 *  Includes
 */

#include "stdint.h"

#include "TLC591x.h"
#include "calendar.h"
/*
 *  Class Definition
 */

class Display {
  public:
    enum class DISPLAY_SM_STATES { SELF_TEST, APP_MODE, CONFIG_MODE };
    static Display *getInstance(void);

    void setTime(Calendar::time_t time);
    void setSafeDayCount(int safe_days);
    void setSafeYearCount(int safe_years);

    // No usecase to actually implement these routines
    // void getTime();
    // void getSafeDayCount();
    // void getSafeYearCount();

    void update(void);

    void selfTest(void);

    DISPLAY_SM_STATES getDisplayState(void);

    void setDisplayState(DISPLAY_SM_STATES state);

  protected:
    // Data is stored in BCD
    enum DISPLAY_BYTE_POS {
        TIME_HOUR  = 0,   // Max 24 hrs, so 1 byte
        TIME_MIN   = 1,   // Max 59 min, so 1 byte
        DAY_COUNT  = 2,   // Max 4 digits to display(9999), so 2 byte
        YEAR_COUNT = 4,   // Max 2 digit(99 year), so 1 byte
        MAX_SIZE   = 5
    };
    uint8_t display_buf[static_cast<uint16_t>(DISPLAY_BYTE_POS::MAX_SIZE)];

  private:
    Display(/* args */);
    DISPLAY_SM_STATES display_state;
    TLC591x          *disp_driver;
    // Display          *instance;
};

#endif   // End of _DISPLAY_H_
