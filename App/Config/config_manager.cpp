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

#include "stm32g0xx_hal.h"

extern IRDA_HandleTypeDef hirda1;

static const std::string secret_key = "1234";

ConfigManager::ConfigManager(/* args */)
    : config_state(CONFIG_STATES::AUTHENTICATE),
      key(new uint8_t[secret_key.length()]) {}

ConfigManager::~ConfigManager() { delete[] key; }

void ConfigManager::run(void) {
    uint8_t data = 0;
    // FIXME: Timeout should be the time needed to receive a single byte as per
    // bitrate of IR rx
    HAL_StatusTypeDef status =
        HAL_IRDA_Receive(&hirda1, &data, sizeof(data), 10);

    if (HAL_OK == status) {
        // TODO: Data received; Process data
    } else if ((HAL_ERROR == status) || (HAL_BUSY == status)) {
        // TODO: Log to debug console
    } else {
        // Timed out!
        // No data received
        // exit config manager thread!
    }
}

void ConfigManager::processData(const uint8_t data) {
    // TODO:
    // Check for a valid key(or key combination) to enter config mode
    // Once into config mode, switch Display mode to config
    // Process further keys.....
    // Config for Safe Day and year setting
    switch (config_state) {
    case CONFIG_STATES::AUTHENTICATE:
        break;

    case CONFIG_STATES::CONFIGURE:
        break;

    case CONFIG_STATES::SAVE:
        break;
    }
}

void ConfigManager::processAuthState(const uint8_t data) {
    static uint8_t auth_input_cnt = 0;

    key[auth_input_cnt] = data;
    auth_input_cnt++;
    if (auth_input_cnt >= secret_key.length()) {
        if (std::memcmp(key, secret_key.c_str(), secret_key.length())) {
            config_state = CONFIG_STATES::CONFIGURE;
        }
        auth_input_cnt = 0;
    }
}

void ConfigManager::processConfigState(uint8_t data) {}
