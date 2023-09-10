/*******************************************************************************
 *  Author: shettyamar (shettyamar5@aol.com)
 *  Created: 03/Sep/2023 16:20:38
 *  FileName: safe_days.h
 *  Description:
 *      The interface to store/restore/update the safe days/year mechanism.
 ******************************************************************************/

/*
 *  Includes
 */
#ifndef _SAFE_DAYS_H_
#define _SAFE_DAYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Includes
 */

// #include " .h"

/*
 *  Class Definition
 */

class SafeDays {
  public:
    SafeDays(/* args */);
    ~SafeDays();
    /// @brief Getter to retrieve the safe days count
    /// @param None
    /// @return Safe day count
    int getSafeDaysCount(void);
    /// @brief Getter to retrieve the safe years count
    /// @param None
    /// @return Safe years count
    int getSafeYearsCount(void);
    /// @brief Increment Safe Days
    /// @param None
    void IncrementSafeDays(void);
    /// @brief Increment Safe year count
    /// @param None
    void IncrementSafeYears(void);

  private:
    /// @brief Constant representing days in a year
    const int kDAYS_IN_A_YEAR = 365;

    int safe_days_count;
    int safe_year_count;

    /// @brief Set/override Safe day count
    /// @param days New safe day count
    /// @note days should not exceed kDAYS_IN_A_YEAR
    void setSafeDaysCount(const int days);

    /// @brief Set/override Safe year count
    /// @param years New safe year count
    void setSafeYearsCount(const int years);
};

#ifdef __cplusplus
}
#endif

#endif   // End of _SAFE_DAYS_H_
