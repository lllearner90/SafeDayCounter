/*******************************************************************************
 *  Author: shettyamar (shettyamar5@aol.com)
 *  Created: 03/Sep/2023 17:53:18
 *  FileName: config_manager.cpp
 *  Description:
 *    The implementation of interface to configure the application in real-time.
 ******************************************************************************/

/*
 *  Includes
 */
#include <cstring>
#include <queue>
#include <string>

#include "config_manager.h"
#include "display.h"
#include "elog.h"
#include "safe_days.h"

#include "stm32g0xx_hal.h"

extern IRDA_HandleTypeDef hirda1;
extern UART_HandleTypeDef huart2;

static const std::string secret_key       = "1234";
static const char       *logger_tag       = "CM";

static ConfigManager    *instance           = nullptr;
static Display          *display_instance   = nullptr;
static SafeDays         *safe_days_instance = nullptr;

std::queue<uint8_t> rx_queue;

ConfigManager *ConfigManager::getInstance(void) {
    // If no instance is created yet, create an instance of config manager
    if (nullptr == instance) {
        instance = new ConfigManager();
    }

    if (nullptr == display_instance) {
        display_instance = Display::getInstance();
    }

    if (nullptr == safe_days_instance) {
        safe_days_instance = SafeDays::getInstance();
    }

    return instance;
}

ConfigManager::ConfigManager(/* args */)
    : config_state(CONFIG_STATES::AUTHENTICATE),
      key(new uint8_t[secret_key.length()]), auth_input_cnt(0),
      config_type(CONFIG_TYPE::MODE_SEL), disp_pos(0) {}

void ConfigManager::run(void) {
    parseSerialData();
    parseIRData();
}

void ConfigManager::serialCallback(uint8_t data) { rx_queue.push(data); }

void ConfigManager::parseSerialData(void) {
    serial_data_t config_data{0};

    while (rx_queue.size() >= sizeof(config_data)) {
        for (auto i = 0; i < sizeof(config_data); i++) {
            reinterpret_cast<uint8_t *>(&config_data)[i] = rx_queue.front();
            rx_queue.pop();
        }

        if (SERIAL_HEADER == config_data.header) {
            elog_d(logger_tag, "S: Header valid");

            int day_cnt = 0;
            Calendar::time_t time    = {0};
            Calendar::date_t date    = {0};

            switch (config_data.cmd) {
            case SERIAL_CMD::S_CMD_TIME:
                elog_d(logger_tag, "S: Cmd byte TIME received!");
                time.hours   = config_data.data[0];
                time.minutes = config_data.data[1];
                setTime(time);
                break;

            case SERIAL_CMD::S_CMD_DATE:
                elog_d(logger_tag, "S: Cmd byte DATE received!");
                date.day   = config_data.data[0];
                date.month = (Calendar::Months) config_data.data[1];
                date.year  = config_data.data[2];
                setDate(date);
                break;

            case SERIAL_CMD::S_CMD_SDAYS:
                elog_d(logger_tag, "S: Cmd byte TIME received!");
                std::memcpy(&day_cnt, config_data.data, SERIAL_DATA_SIZE);
                safe_days_instance->setSafeDaysCount(day_cnt);
                elog_i(logger_tag, "Updated SafeDayCount = %d", day_cnt);
                break;

            default:
                elog_d(logger_tag, "S: Invalid Cmd byte received!");
                break;
            }
        }
    }
}

void ConfigManager::setTime(Calendar::time_t &time) {
    extern RTC_HandleTypeDef hrtc;
    RTC_TimeTypeDef          sTime = {0};

    sTime.Hours          = time.hours;
    sTime.Minutes        = time.minutes;
    sTime.Seconds        = 0x0;
    sTime.SubSeconds     = 0x0;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
        elog_a(logger_tag, "Set Time failed!");
    } else {
        elog_i(logger_tag, "Set Time success!");
    }
}

void ConfigManager::setDate(Calendar::date_t &date) {
    extern RTC_HandleTypeDef hrtc;
    RTC_DateTypeDef          sDate = {0};

    // sDate.WeekDay = RTC_WEEKDAY_SATURDAY;
    sDate.Month = (uint8_t) date.month;
    sDate.Date  = date.day;
    sDate.Year  = date.year;

    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
        elog_a(logger_tag, "Set Date failed!");
    } else {
        elog_i(logger_tag, "Set Date success!");
    }
}

void ConfigManager::parseIRData(void) {
    uint8_t data = 0;
    // FIXME: Timeout should be the time needed to receive a single byte as per
    // bitrate of IR rx
    HAL_StatusTypeDef status =
        HAL_IRDA_Receive(&hirda1, &data, sizeof(data), 10);

    if (HAL_OK == status) {
        // TODO: Data received; Process data
        processData(data);
    } else if ((HAL_ERROR == status) || (HAL_BUSY == status)) {
        // TODO: Log to debug console
        // TODO: Display Err code for User?
        elog_e(logger_tag, "Data Error code: %d when in CONFIG_STATE=%d",
               status, config_state);
        // reset parameters
        auth_input_cnt = 0;
        config_state   = CONFIG_STATES::AUTHENTICATE;

    } else {
        // Timed out!
        // No data received
        // exit config manager thread!
    }
}

void ConfigManager::processData(const uint8_t &data) {
    // TODO:
    // Check for a valid key(or key combination) to enter config mode
    // Once into config mode, switch Display mode to config
    // Process further keys.....
    // Config for Safe Day and year setting
    switch (config_state) {
    case CONFIG_STATES::AUTHENTICATE:
        processAuthState(data);
        break;

    case CONFIG_STATES::CONFIGURE:
        processConfigState(data);
        break;

    case CONFIG_STATES::SAVE:
        break;
    }
}

void ConfigManager::processAuthState(const uint8_t &data) {
    key[auth_input_cnt] = data;
    auth_input_cnt++;
    if (auth_input_cnt >= secret_key.length()) {
        if (std::memcmp(key, secret_key.c_str(), secret_key.length())) {
            config_state = CONFIG_STATES::CONFIGURE;
            elog_i(logger_tag, "AUTH success!");
        }
        auth_input_cnt = 0;
    }
}

void ConfigManager::processConfigState(const uint8_t &data) {

    switch (config_type) {
    case CONFIG_TYPE::MODE_SEL:
        configureModeSelect(data);
        break;

    case CONFIG_TYPE::MODE_TIME:
        configureTime(data);
        break;

    case CONFIG_TYPE::MODE_SAFE_DAY:
        configureSafeDay(data);
        break;
    }
}

void ConfigManager::processSaveState(void) {
    // TODO: Set blinker OFF

    // Save data

    // Reset parameters
    // The parameters to be reset are the same one's that are initialised by the
    // Constructor
}

void ConfigManager::configureModeSelect(const uint8_t &data) {
    // Convert the data which ideally should be a char (1 or 2) into numeric
    // form i.e. int value
    int mode_in_int = data - '0';

    switch (mode_in_int) {
    case static_cast<int>(CONFIG_TYPE::MODE_TIME):
        config_type = CONFIG_TYPE::MODE_TIME;
        // TODO: Set blinker ON
        break;

    case static_cast<int>(CONFIG_TYPE::MODE_SAFE_DAY):
        config_type = CONFIG_TYPE::MODE_SAFE_DAY;
        // TODO: Set blinker ON
        break;

    default:
        // log on console
        elog_a(logger_tag, "Invalid mode(%c) encountered ", data);
        break;
    }
}

void ConfigManager::configureTime(const uint8_t &data) {
    if (data == NEXT_CHAR) {
        // Move to next
        disp_pos++;
        // FIXME: Fetch information from driver/display
        if (disp_pos >= 4) {
            disp_pos = 0;
        }
    } else if (data == SAVE_CHAR) {
        // TODO: Move to Save and save the data
    }

    // TODO: Set blinker position = disp_pos
}

void ConfigManager::configureSafeDay(const uint8_t &data) {}
