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
#include "elog.h"
#include <cstring>
#include <exception>

#include "calendar_stm32.h"
#include "stm32g0xx_hal.h"

extern CRC_HandleTypeDef hcrc;

uint8_t safe_day_database[64] __attribute__((section(".NVSRAM")));

static const char *logger_tag = "SafeDays";

static SafeDays *instance = nullptr;

SafeDays *SafeDays::getInstance(void) {
    if (nullptr == instance) {
        instance = new SafeDays();
    }
    return instance;
}

SafeDays::SafeDays(/* args */)
    : safe_days_count{158}, safe_year_count{0}, stored_date{0} {
    // TODO :Read the values from persistent memory
    bool is_valid = validateSafeDayDatabase();
    if (is_valid) {
        elog_i(logger_tag, "Valid Data!");
        safe_days_count = getSafeDayCountFromMemory();
        safe_year_count = getSafeYearCountFromMemory();
        stored_date     = getStoredDateFromMemory();
    } else {
        elog_i(logger_tag, "Invalid Data!");
        Calendar        *calendar_instance = CalendarSTM32::getInstance();
        Calendar::date_t current_date      = calendar_instance->getDate();
        calSafeYearsCount(safe_days_count);
        storeSafeDayCountToMemory();
        storeSafeYearCountToMemory();

        // save/update stored date
        stored_date = current_date;
        storeStoredDateToMemory();
    }
    elog_d(logger_tag, "Init details:\nsafe_day = %d\n safe_year = %f",
           safe_days_count, safe_year_count);
}

int SafeDays::getSafeDaysCount(void) { return safe_days_count; }

float SafeDays::getSafeYearsCount(void) { return safe_year_count; }

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
    safe_days_count = days;
    elog_d(logger_tag, "New Safe Day count: %d", safe_days_count);
    // store to persistent memory
    storeSafeDayCountToMemory();
    calSafeYearsCount(safe_days_count);
}

void SafeDays::setSafeYearsCount(const int years) {
    safe_year_count = years;
    elog_d(logger_tag, "New Safe Year count: %f", safe_year_count);
    // store to persistent memory
    storeSafeYearCountToMemory();
}

void SafeDays::calSafeYearsCount(const int safe_days_cnt) {
    safe_year_count = (float) safe_days_cnt / kDAYS_IN_A_YEAR;
    elog_d(logger_tag, "Calc safe year count: %f", safe_year_count);
    storeSafeYearCountToMemory();
}

bool SafeDays::validateSafeDayDatabase(void) {
    bool     is_valid = false;
    uint32_t crc      = HAL_CRC_Calculate(
        &hcrc,
        (uint32_t *) &(safe_day_database[(
            uint8_t) SAFE_DAY_DATABASE_MAP::SAFE_DAY_OFFSET]),
        ((uint8_t) SAFE_DAY_DATABASE_MAP::END_OFFSET
         - (uint8_t) SAFE_DAY_DATABASE_MAP::CRC_FIELD_OFFSET));

    if (0
        == std::memcmp(&crc,
                       &safe_day_database[(
                           uint8_t) SAFE_DAY_DATABASE_MAP::CRC_FIELD_OFFSET],
                       kCRC_SIZE)) {
        is_valid = true;
    }
    return is_valid;
}

int SafeDays::getSafeDayCountFromMemory(void) {
    int count = 0;
    std::memcpy(
        &count,
        &safe_day_database[(uint8_t) SAFE_DAY_DATABASE_MAP::SAFE_DAY_OFFSET],
        sizeof(count));

    return count;
}

float SafeDays::getSafeYearCountFromMemory(void) {
    float years = 0;
    std::memcpy(
        &years,
        &safe_day_database[(uint8_t) SAFE_DAY_DATABASE_MAP::SAFE_YEAR_OFFSET],
        sizeof(years));

    return years;
}

Calendar::date_t SafeDays::getStoredDateFromMemory(void) {
    Calendar::date_t date = {0};

    std::memcpy(
        &date,
        &safe_day_database[(uint8_t) SAFE_DAY_DATABASE_MAP::STORED_DATE_OFFSET],
        sizeof(date));

    return date;
}

void SafeDays::storeSafeDayCountToMemory(void) {
    std::memcpy(
        &safe_day_database[(uint8_t) SAFE_DAY_DATABASE_MAP::SAFE_DAY_OFFSET],
        &safe_days_count, sizeof(safe_days_count));

    if (false == updateMemoryBlockCrc()) {
        // todo: log to console
    }
}

void SafeDays::storeSafeYearCountToMemory(void) {
    std::memcpy(
        &safe_day_database[(uint8_t) SAFE_DAY_DATABASE_MAP::SAFE_YEAR_OFFSET],
        &safe_year_count, sizeof(safe_year_count));

    if (false == updateMemoryBlockCrc()) {
        // todo: log to console
    }
}

void SafeDays::storeStoredDateToMemory(void) {
    std::memcpy(
        &safe_day_database[(uint8_t) SAFE_DAY_DATABASE_MAP::STORED_DATE_OFFSET],
        &stored_date, sizeof(stored_date));
    elog_d(logger_tag, "new date to memory: %d/%d/%d", stored_date.day,
           (uint8_t) stored_date.month, stored_date.year);
    if (false == updateMemoryBlockCrc()) {
        // todo: log to console
    }
}

bool SafeDays::updateMemoryBlockCrc(void) {
    // Compute CRC
    uint32_t crc = HAL_CRC_Calculate(
        &hcrc,
        (uint32_t *) &(safe_day_database[(
            uint8_t) SAFE_DAY_DATABASE_MAP::SAFE_DAY_OFFSET]),
        ((uint8_t) SAFE_DAY_DATABASE_MAP::END_OFFSET
         - (uint8_t) SAFE_DAY_DATABASE_MAP::CRC_FIELD_OFFSET));

    // Update CRC
    std::memcpy(
        &safe_day_database[(uint8_t) SAFE_DAY_DATABASE_MAP::CRC_FIELD_OFFSET],
        &crc, kCRC_SIZE);

    // Validate and return the status
    return validateSafeDayDatabase();
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
        // save/update stored date
        stored_date = current_date;
        storeStoredDateToMemory();
    }
    // 3. if the stored day is less than current day & year;
    else {
        // TODO:
        // prompt for reset?
        num_of_days = 0;
    }
    // Increment years
    safe_days_count += num_of_days;

    // while (safe_days_count >= kDAYS_IN_A_YEAR) {
    //     IncrementSafeYears();
    //     safe_days_count -= kDAYS_IN_A_YEAR;
    // }
    if (num_of_days) {
        // Set days
        setSafeDaysCount(safe_days_count);
    }
}
