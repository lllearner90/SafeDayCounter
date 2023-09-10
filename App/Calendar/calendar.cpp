/*******************************************************************************
 *  Author: shettyamar (shettyamar5@aol.com)
 *  Created: 03/Sep/2023 18:47:28
 *  FileName: calendar.cpp
 *  Description:
 ******************************************************************************/

/*
 *  Includes
 */
#include "calendar.h"

/*
 *  Private variables prototypes
 */
// TODO: Clean up global variables for RTC
//  Decouple HW RTC with the software implementation
static RTC_HandleTypeDef *rtc_instance = nullptr;

/*
 *  Private function prototypes
 */

/*
 *  Function Definitions
 */

Calendar::Calendar(RTC_HandleTypeDef *const sys_rtc_instance) {}

Calendar::~Calendar() {}

void Calendar::Init(RTC_HandleTypeDef *const sys_rtc_instance) {
    rtc_instance = sys_rtc_instance;
}
