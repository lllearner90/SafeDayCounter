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
#include <string>

#include "config_manager.h"
#include "display.h"
#include "elog.h"
#include "safe_days.h"

#include "stm32g0xx_hal.h"

extern IRDA_HandleTypeDef hirda1;
extern UART_HandleTypeDef huart2;

static const std::string secret_key       = "1234";
static ConfigManager    *instance         = nullptr;
static Display          *display_instance = nullptr;
static const char       *logger_tag       = "ConfigM";
static SafeDays         *safe_days_instance = nullptr;

ConfigManager *ConfigManager::getInstance(void) {
    // If no instance is created yet, create an instance of config manager
    if (nullptr == instance) {
        instance = new ConfigManager();
    }

    if (nullptr == display_instance) {
        display_instance = Display::getInstance();
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

void ConfigManager::parseSerialData(void) {
    serial_data_t     config_data{0};
    HAL_StatusTypeDef status = HAL_UART_Receive(
        &huart2, (uint8_t *) &config_data, sizeof(config_data), 100);

    if (status == HAL_OK) {
        if (SERIAL_HEADER == config_data.header) {
            elog_d(logger_tag, "S: Header valid");
            int day_cnt = 0;
            switch (config_data.cmd) {
            case SERIAL_CMD::S_CMD_TIME:
                elog_d(logger_tag, "S: Cmd byte TIME received!");
                break;

            case SERIAL_CMD::s_CMD_SDAYS:
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
