/*******************************************************************************
 *  Author: shettyamar (shettyamar5@aol.com)
 *  Created: 03/Sep/2023 17:55:14
 *  FileName: display.cpp
 *  Description:
 ******************************************************************************/

/*
 *  Includes
 */
#include "display.h"
#include "bcd.h"
#include "cmsis_os.h"
#include "conv.h"
#include "elog.h"
#include "stm32g0xx_hal.h"

#include <cstring>
#include <string>

static const char *logger_tag = "Display";

// Ideally, the information should be hidden in the display driver
// Implementation should be of form
// Display_App -> Display_driver -> HW(TLC591x)
const uint8_t NUMBER_OF_7_SEG_DISPLAYS = 4;

static Display *instance = nullptr;

Display::Display(/* args */) : disp_driver(nullptr) {}

Display *Display::getInstance(void) {
    // If not instance is created yet, create an instance of Display
    if (nullptr == instance) {
        instance = new Display();
    }

    if (nullptr == instance->disp_driver) {
        instance->disp_driver = new TLC591x(NUMBER_OF_7_SEG_DISPLAYS,
                                            TLC591x::INTERFACE_TYPE::SW_SPI);
    }

    instance->display_state = Display::DISPLAY_SM_STATES::SELF_TEST;
    return instance;
}

void Display::setTime(Calendar::time_t time) {
    // Since Hours and minutes are a byte each, direct assignment works!
    // display_buf[Display::DISPLAY_BYTE_POS::TIME_HOUR] =
    // binaryToBCD(time.hours); display_buf[Display::DISPLAY_BYTE_POS::TIME_MIN]
    // =
    //     binaryToBCD(time.minutes);
    display_data.time_hr  = time.hours;
    display_data.time_min = time.minutes;
}

void Display::setSafeDayCount(int safe_days) {
    if (safe_days >= 0) {
        // uint16_t safe_days_bcd = safe_days;
        // //binaryToBCD(static_cast<uint16_t>(safe_days));
        // std::memcpy(&display_buf[Display::DISPLAY_BYTE_POS::DAY_COUNT],
        //             &safe_days_bcd, sizeof(safe_days_bcd));
        display_data.day_cnt = safe_days;
    } else {
        // throw an exception
        elog_a(logger_tag, "Expected safe_day >= 0, received %d", safe_days);
    }
}

void Display::setSafeYearCount(float safe_years) {
    if (safe_years >= 0) {
        // uint8_t safe_year_bcd = safe_years;
        // //binaryToBCD(static_cast<uint8_t>(safe_years));
        // std::memcpy(&display_buf[Display::DISPLAY_BYTE_POS::YEAR_COUNT],
        //             &safe_year_bcd, sizeof(safe_year_bcd));
        display_data.year_cnt = safe_years;
    } else {
        // throw an exception
        elog_a(logger_tag, "Expected safe_years >= 0, received %d", safe_years);
    }
}

void Display::update(void) {
    // TODO: Force update the driver
    // periodically called to push the data from SW buffer to HW implementation
    // of Display

    // Since the Time display is absent,
    // sent the data for Safe days and Safe Year
    static enum UPDATE_STATE {
        DAYS_CNT,
        YEAR_CNT,
        TIME
    } update_state   = DAYS_CNT;
    uint16_t data    = 0;
    char     year[6] = "";

    switch (update_state) {
    case DAYS_CNT:

        data = display_data.day_cnt;
        osDelay(5000 - 100);
        update_state = YEAR_CNT;

        disp_driver->print(data);
        break;

    case YEAR_CNT:
        osDelay(5000 - 100);
        update_state = DAYS_CNT;

        // std::sprintf(year, "%.2f", display_data.year_cnt);
        floatToChar(display_data.year_cnt, year, 2, sizeof(year));
        disp_driver->print(year);
        break;

    case TIME:

        data = display_data.time_hr * 100;
        data += display_data.time_min;
        osDelay(5000 - 100);
        update_state = DAYS_CNT;

        disp_driver->print(data);
        break;

    default:
        break;
    }
}

void Display::selfTest(void) {
    // FIXME: Representing " " is problematic for BCD!
    for (auto num = 0; num < 10; num++) {
        for (auto i = 0; i < NUMBER_OF_7_SEG_DISPLAYS; i++) {
            // disp_driver->sendValue(disp_driver->convertNumberToPattern(num));
            // disp_driver->print(num);
        }
        // disp_driver->toggleLE();
    }
    // char s[] = "12ab";
    // disp_driver->print(s);
}

Display::DISPLAY_SM_STATES Display::getDisplayState(void) {
    return this->display_state;
}

void Display::setDisplayState(DISPLAY_SM_STATES state) {
    display_state = state;
}
