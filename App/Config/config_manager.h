/*******************************************************************************
 *  Author: shettyamar (shettyamar5@aol.com)
 *  Created: 03/Sep/2023 16:20:38
 *  FileName: config_manager.h
 *  Description:
 *      The implementation of interface to configure the application in
 *  real-time.
 ******************************************************************************/

#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

/*
 *  Includes
 */
#include <stdint.h>

/*
 *  Class Definition
 */

class ConfigManager {
  private:
    enum class CONFIG_STATES {
        AUTHENTICATE,
        CONFIGURE,
        SAVE
    };

    enum class CONFIG_TYPE {
        MODE_SEL      = 0,
        MODE_TIME     = 1,
        MODE_SAFE_DAY = 2
    };

    CONFIG_STATES config_state;
    CONFIG_TYPE   config_type;
    uint8_t      *key;
    uint8_t       auth_input_cnt;
    uint8_t       disp_pos;

    // FIXME: assign proper char to be used to next shift
    const uint8_t NEXT_CHAR = static_cast<uint8_t>("a"[0]);
    const uint8_t SAVE_CHAR = static_cast<uint8_t>("a"[0]);

    ConfigManager(/* args */);

    void processData(const uint8_t &data);
    void processAuthState(const uint8_t &data);
    void processConfigState(const uint8_t &data);
    void processSaveState(void);

    void configureModeSelect(const uint8_t &data);
    void configureTime(const uint8_t &data);
    void configureSafeDay(const uint8_t &data);

  public:
    static ConfigManager *getInstance(void);
    void run(void);
};

#endif   // End of _CONFIG_MANAGER_H_