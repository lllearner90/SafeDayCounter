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
#include "TLC591x.h"
#include "bcd.h"
#include "cmsis_os.h"
#include "stm32g0xx_hal.h"

#include <cstring>

TLC591x *disp_driver;

Display::Display(/* args */) {}

Display *Display::getInstance(void) {
    // If not instance is created yet, create an instance of Display
    // if (instance == nullptr) {
    //     instance = new Display();
    // }
    static Display instance;
    disp_driver = new TLC591x(4);
    return &instance;
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
        // TODO: throw an exception
    }
}

void Display::setSafeYearCount(int safe_years) {
    if (safe_years >= 0) {
        uint8_t safe_year_bcd = binaryToBCD(static_cast<uint8_t>(safe_years));
        std::memcpy(&display_buf[Display::DISPLAY_BYTE_POS::YEAR_COUNT],
                    &safe_year_bcd, sizeof(safe_year_bcd));
    } else {
        // TODO: throw an exception
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
    osDelay(1);

    uint8_t st[5] = "8888";
    disp_driver->printDirect(st);
    // TODO: Update ticks count to represent 1s
    osDelay(1);
}
