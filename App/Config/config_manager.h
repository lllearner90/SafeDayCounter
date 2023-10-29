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
    void processData(uint8_t data);

  public:
    ConfigManager(/* args */);
    ~ConfigManager();
    void run(void);
};

#endif   // End of _CONFIG_MANAGER_H_