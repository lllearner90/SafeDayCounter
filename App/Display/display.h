/*******************************************************************************
 *  Author: shettyamar (shettyamar5@aol.com)
 *  Created: 03/Sep/2023 17:53:18
 *  FileName: display.h
 *  Description:
 ******************************************************************************/

/*
 *  Includes
 */
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Includes
 */

#include "stdint.h"

/*
 *  Class Definition
 */

class Display {
public:
    Display(/* args */);
    ~Display();
    uint8_t *Serialize();
    uint8_t *Deserialize();
    void Update(void);
private:
    //
};



#ifdef __cplusplus
}
#endif

#endif // End of _DISPLAY_H_
