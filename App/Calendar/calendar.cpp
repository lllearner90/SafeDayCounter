#include "calendar.h"
#include <cstdlib>

static const int DaysInMonth[12] = {31,  59,  90,  120, 151, 181,
                                    212, 243, 273, 304, 334, 365};

bool operator==(const Calendar::date_t &lhs_date,
                const Calendar::date_t &rhs_date) {
    if ((lhs_date.day == rhs_date.day) && (lhs_date.month == rhs_date.month)
        && (lhs_date.year == rhs_date.year))
        return true;
    else
        return false;
}

bool operator!=(const Calendar::date_t &lhs_date,
                const Calendar::date_t &rhs_date) {
    // Inverted logic to the "==" operator
    if (lhs_date == rhs_date)
        return false;
    else
        return true;
}

int operator-(const Calendar::date_t &lhs_date,
              const Calendar::date_t &rhs_date) {

    return Calendar::getNumOfDays(lhs_date, rhs_date);
}

bool operator>(const Calendar::date_t &lhs_date,
               const Calendar::date_t &rhs_date) {
    // TODO: revisit the logic for any missed case
    if (lhs_date.year > rhs_date.year) {
        return true;
    } else if (lhs_date.year == rhs_date.year) {
        if (lhs_date.month > rhs_date.month) {
            return true;
        } else if (lhs_date.year == rhs_date.year) {
            if (lhs_date.day > rhs_date.day) {
                return true;
            }
        }
    }
    return false;
}

bool operator<(const Calendar::date_t &lhs_date,
               const Calendar::date_t &rhs_date) {
    if (lhs_date > rhs_date)
        return false;
    else
        return true;
}

int Calendar::getNumOfDays(const Calendar::date_t &lhs_date,
                           const Calendar::date_t &rhs_date) {
    Calendar::date_t start_date = lhs_date;
    Calendar::date_t end_date   = rhs_date;

    long int num_of_days_1 = start_date.year * 365 + start_date.day;

    // Add days for months in given date
    num_of_days_1 += DaysInMonth[static_cast<int>(start_date.month) - 1];

    num_of_days_1 += getLeapYearCount(start_date);

    long int num_of_days_2 = end_date.year * 365 + end_date.day;

    // Add days for months in given date
    num_of_days_2 += DaysInMonth[static_cast<int>(end_date.month) - 1];

    num_of_days_2 += getLeapYearCount(end_date);

    return (num_of_days_1 - num_of_days_2);
}

int Calendar::getLeapYearCount(const Calendar::date_t &date) {
    int leap_year_count = date.year;

    if (date.month <= Calendar::Months::FEBRUARY) {
        leap_year_count--;
    }

    leap_year_count = ((leap_year_count / 4) - (leap_year_count / 100)
                       + (leap_year_count / 400));
    return leap_year_count;
}
