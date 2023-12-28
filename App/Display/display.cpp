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
#include "elog.h"
#include "stm32g0xx_hal.h"

#include <cstring>

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
        instance->disp_driver = new TLC591x(NUMBER_OF_7_SEG_DISPLAYS);
    }

    instance->display_state = Display::DISPLAY_SM_STATES::SELF_TEST;
    return instance;
}

void Display::setTime(Calendar::time_t time) {
    // Since Hours and minutes are a byte each, direct assignment works!
    display_buf[Display::DISPLAY_BYTE_POS::TIME_HOUR] = binaryToBCD(time.hours);
    display_buf[Display::DISPLAY_BYTE_POS::TIME_MIN] =
        binaryToBCD(time.minutes);
}

void Display::setSafeDayCount(int safe_days) {
    if (safe_days >= 0) {
        uint16_t safe_days_bcd = binaryToBCD(static_cast<uint16_t>(safe_days));
        std::memcpy(&display_buf[Display::DISPLAY_BYTE_POS::DAY_COUNT],
                    &safe_days_bcd, sizeof(safe_days_bcd));
    } else {
        // throw an exception
        elog_a(logger_tag, "Expected safe_day >= 0, received %d", safe_days);
    }
}

void Display::setSafeYearCount(int safe_years) {
    if (safe_years >= 0) {
        uint8_t safe_year_bcd = binaryToBCD(static_cast<uint8_t>(safe_years));
        std::memcpy(&display_buf[Display::DISPLAY_BYTE_POS::YEAR_COUNT],
                    &safe_year_bcd, sizeof(safe_year_bcd));
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
    disp_driver->printDirect(&display_buf[DAY_COUNT]);
}

void Display::selfTest(void) {
    // FIXME: Representing " " is problematic for BCD!
    uint8_t s[5] = {"    "};
    disp_driver->printDirect(s);
    // TODO: Update ticks count to represent 1s
    osDelay(portTICK_PERIOD_MS * 1000);

    uint8_t st[5] = "8888";
    disp_driver->printDirect(st);
    // TODO: Update ticks count to represent 1s
    osDelay(portTICK_PERIOD_MS * 1000);
}

Display::DISPLAY_SM_STATES Display::getDisplayState(void) {
    return this->display_state;
}

void Display::setDisplayState(DISPLAY_SM_STATES state) {
    display_state = state;
}