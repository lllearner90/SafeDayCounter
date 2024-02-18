/*******************************************************************************
 *  Author: shettyamar (shettyamar5@aol.com)
 *  Created: 03/Sep/2023 16:20:38
 *  FileName: safe_days.h
 *  Description:
 *      The interface to store/restore/update the safe days/year mechanism.
 ******************************************************************************/

#ifndef _SAFE_DAYS_H_
#define _SAFE_DAYS_H_

/*
 *  Includes
 */

#include "calendar.h"

/*
 *  Class Definition
 */

class SafeDays {
  public:
    /// @brief Fetches the SafeDays Instance
    /// @param  None
    /// @return Instance of class SafeDays
    /// @note Singleton class
    static SafeDays *getInstance(void);
    /// @brief Getter to retrieve the safe days count
    /// @param None
    /// @return Safe day count
    int getSafeDaysCount(void);
    /// @brief Getter to retrieve the safe years count
    /// @param None
    /// @return Safe years count
    float getSafeYearsCount(void);
    /// @brief Increment Safe Days
    /// @param None
    void IncrementSafeDays(void);
    /// @brief Increment Safe year count
    /// @param None
    void IncrementSafeYears(void);

    /// @brief Set/override Safe day count
    /// @param days New safe day count
    void setSafeDaysCount(const int days);

    /// @brief Handle the safe day/year update logic
    /// @param None
    void update(void);

  private:
    SafeDays(/* args */);
    /// @brief Constant representing days in a year
    const int kDAYS_IN_A_YEAR = 365;

    int              safe_days_count;
    float            safe_year_count;
    Calendar::date_t stored_date;

    /// @brief Size of the crc
    static const int kCRC_SIZE = sizeof(uint32_t);

    /// @brief Safe day data memory map in the database
    enum class SAFE_DAY_DATABASE_MAP : uint8_t {
        CRC_FIELD_OFFSET   = 0,
        SAFE_DAY_OFFSET    = CRC_FIELD_OFFSET + kCRC_SIZE,
        SAFE_YEAR_OFFSET   = SAFE_DAY_OFFSET + sizeof(safe_days_count),
        STORED_DATE_OFFSET = SAFE_YEAR_OFFSET + sizeof(safe_year_count),
        END_OFFSET         = STORED_DATE_OFFSET + sizeof(stored_date)
    };

    /// @brief Set/override Safe year count
    /// @param years New safe year count
    void setSafeYearsCount(const int years);

    /// @brief Calculate Safe year count for the given safe days count
    /// @param safe_days_cnt safe days count
    void calSafeYearsCount(const int safe_days_cnt);

    /// @brief Verify the validity of stored data in memory
    /// @param  None
    /// @return true if data valid else false
    bool validateSafeDayDatabase(void);

    /// @brief Fetches the safe_day_count stored in persistent memory
    /// @param  None
    /// @return safe_day_count
    /// @note The function should only be called if valid data exists
    ///       in memory i.e. after validateSafeDayDatabase() returns true.
    int getSafeDayCountFromMemory(void);

    /// @brief Fetches the safe_year_count stored in persistent memory
    /// @param  None
    /// @return safe_year_count
    /// @note The function should only be called if valid data exists
    ///       in memory i.e. after validateSafeDayDatabase() returns true.
    float getSafeYearCountFromMemory(void);

    /// @brief Fetches the stored_date stored in persistent memory
    /// @param  None
    /// @return stored_date
    /// @note The function should only be called if valid data exists
    ///       in memory i.e. after validateSafeDayDatabase() returns true.
    Calendar::date_t getStoredDateFromMemory(void);

    /// @brief Store the safe_day_count in persistent memory
    /// @param  None
    /// @return None
    /// @note The function also updates the CRC of the memory block
    void storeSafeDayCountToMemory(void);

    /// @brief Store the safe_year_count stored in persistent memory
    /// @param  None
    /// @return None
    /// @note The function also updates the CRC of the memory block
    void storeSafeYearCountToMemory(void);

    /// @brief Store the date in persistent memory
    /// @param  None
    /// @return None
    /// @note The function also updates the CRC of the memory block
    void storeStoredDateToMemory(void);

    /// @brief Update the crc of the memory block
    /// @param  None
    /// @return true if the crc update successful else false
    bool updateMemoryBlockCrc(void);
};

#endif   // End of _SAFE_DAYS_H_
