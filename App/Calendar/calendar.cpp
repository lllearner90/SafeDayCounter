#include "calendar.h"
#include <cstdlib>
#include <ctime>

static const int DaysInMonth[12] = {31,  59,  90,  120, 151, 181,
                                    212, 243, 273, 304, 334, 365};
static const int kCENTURY        = 100;

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
    std::tm start{.tm_mday = lhs_date.day,
                  .tm_mon  = (uint8_t) lhs_date.month - 1,
                  .tm_year = lhs_date.year + kCENTURY};
    std::tm end{.tm_mday = rhs_date.day,
                .tm_mon  = (uint8_t) rhs_date.month - 1,
                .tm_year = rhs_date.year + kCENTURY};

    std::time_t start_time = std::mktime(&start);
    std::time_t end_time   = std::mktime(&end);

    double difference = std::difftime(end_time, start_time);

    int num_of_days = difference / (60 * 60 * 24);

    return abs(num_of_days);
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
