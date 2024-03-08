#include "../calendar.h"
#include <gtest/gtest.h>

class TestCalendar : public Calendar {
  public:
    void setTime(time_t time) override { this->time = time; };
    void setDate(date_t date) override { this->date = date; };

    Calendar::time_t getTime(void) override { return time; };
    Calendar::date_t getDate(void) override { return date; };

    static TestCalendar *getInstance(void);

  private:
    time_t time;
    date_t date;

    TestCalendar(){};
};

static TestCalendar *instance{nullptr};

TestCalendar *TestCalendar::getInstance(void) {
    if (instance == nullptr) {
        instance = new TestCalendar();
    }
    return instance;
};

TEST(Calendar, LeapYear) {
    Calendar::date_t date_prev{
      .day = 29, .month = Calendar::FEBRUARY, .year = 24};
    Calendar::date_t date_today{.day = 1, .month = Calendar::MARCH, .year = 24};

    int day_count = date_today - date_prev;

    EXPECT_EQ(day_count, 1);
}

TEST(Calendar, GetSetDate) {
    Calendar::date_t set_date{
      .day = 1, .month = Calendar::FEBRUARY, .year = 24};
    TestCalendar *instance = TestCalendar::getInstance();

    instance->setDate(set_date);

    Calendar::date_t get_date = instance->getDate();

    EXPECT_EQ(set_date, get_date);
}

TEST(Calendar, GetSetTime) {
    Calendar::time_t set_time{.hours = 12, .minutes = 35, .seconds = 10};
    TestCalendar    *instance = TestCalendar::getInstance();

    instance->setTime(set_time);

    Calendar::time_t get_time = instance->getTime();

    EXPECT_EQ(set_time.hours, get_time.hours);
    EXPECT_EQ(set_time.minutes, get_time.minutes);
    EXPECT_EQ(set_time.seconds, get_time.seconds);
}

TEST(Calendar, Operators) {
    Calendar::date_t date_prev{
      .day = 1, .month = Calendar::FEBRUARY, .year = 24};
    Calendar::date_t date_today{.day = 1, .month = Calendar::MARCH, .year = 24};

    EXPECT_TRUE(date_prev < date_today);
    EXPECT_FALSE(date_today < date_prev);

    EXPECT_FALSE(date_prev > date_today);
    EXPECT_TRUE(date_today > date_prev);

    EXPECT_TRUE(date_prev == date_prev);
    EXPECT_FALSE(date_today == date_prev);

    EXPECT_TRUE(date_today != date_prev);
    EXPECT_FALSE(date_prev != date_prev);

    int day_count = date_today - date_prev;

    EXPECT_EQ(day_count, 29);
}