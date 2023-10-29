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
#include "config_manager.h"

#include "stm32g0xx_hal.h"

extern IRDA_HandleTypeDef hirda1;

ConfigManager::ConfigManager(/* args */) {}

ConfigManager::~ConfigManager() {}

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

void ConfigManager::processData(uint8_t data) {
    // TODO:
    // Check for a valid key(or key combination) to enter config mode
    // Once into config mode, switch Display mode to config
    // Process further keys.....
    // Config for Safe Day and year setting
    switch (data) {
    case '1':
        break;
    }
}