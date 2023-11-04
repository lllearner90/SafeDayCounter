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
    enum class CONFIG_STATES { AUTHENTICATE, CONFIGURE, SAVE };
    CONFIG_STATES config_state;
    uint8_t      *key;

    void processData(const uint8_t data);
    void processAuthState(const uint8_t data);
    void processConfigState(const uint8_t data);
    void processSaveState(void);

  public:
    ConfigManager(/* args */);
    ~ConfigManager();
    void run(void);
};

#endif   // End of _CONFIG_MANAGER_H_