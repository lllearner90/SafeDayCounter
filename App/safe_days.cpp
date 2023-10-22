/*******************************************************************************
 *  Author: shettyamar (shettyamar5@aol.com)
 *  Created: 03/Sep/2023 17:53:18
 *  FileName: safe_days.cpp
 *  Description:
 *    The implementation of interface to store/restore/update the safe days/year
 *  mechanism.
 ******************************************************************************/

/*
 *  Includes
 */
#include "safe_days.h"
#include "calendar_stm32.h"
#include <exception>

SafeDays::SafeDays(/* args */) {
    // TODO :Read the values from persistent memory
    safe_days_count = 0;
    safe_year_count = 0;
    stored_date     = Calendar::date_t{0};
}

SafeDays::~SafeDays() {}

int SafeDays::getSafeDaysCount(void) { return safe_days_count; }

int SafeDays::getSafeYearsCount(void) { return safe_year_count; }

void SafeDays::IncrementSafeDays(void) {
    safe_days_count++;
    if (safe_days_count > kDAYS_IN_A_YEAR) {
        safe_year_count++;
        safe_days_count = 0;
    }
    // TODO: Store to persistent memory
}

void SafeDays::IncrementSafeYears(void) {
    safe_year_count++;
    // TODO: Store to persistent memory
}

void SafeDays::setSafeDaysCount(const int days) {
    if (days <= kDAYS_IN_A_YEAR) {
        safe_days_count = days;
    } else {
        // TODO: Throw exception or debug message?
    }
    // TODO: store to persistent memory
}

void SafeDays::setSafeYearsCount(const int years) {
    safe_year_count = years;
    // TODO: store to persistent memory
}

void SafeDays::update(void) {
    // TODO: implement
    int num_of_days = 0;

    // 1. Read the current day from RTC
    Calendar        *calendar_instance = CalendarSTM32::getInstance();
    Calendar::date_t current_date      = calendar_instance->getDate();

    // 2. If the current day is greater then stored day,
    // computed the days elapsed
    if ((current_date != stored_date) && (current_date > stored_date)) {
        num_of_days = current_date - stored_date;
    }
    // 3. if the stored day is less than current day & year;
    else {
        // TODO:
        // prompt for reset?
        num_of_days = 0;
    }
    // Increment years
    safe_days_count += num_of_days;
    while (safe_days_count >= kDAYS_IN_A_YEAR) {
        IncrementSafeYears();
        safe_days_count -= kDAYS_IN_A_YEAR;
    }
    // Set days
    setSafeDaysCount(safe_days_count);
}
