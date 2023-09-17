/*******************************************************************************
 *  Author: shettyamar (shettyamar5@aol.com)
 *  Created: 03/Sep/2023 18:47:28
 *  FileName: calendar.cpp
 *  Description:
 ******************************************************************************/

/*
 *  Includes
 */
#include "bcd.h"

/*
 *  Function Definitions
 */

template <typename T> T binaryToBCD(T binary_value) {
    T bcd_value = 0;
    T shift = 0;

    // TODO: optimise for time / resource consumption
    while (binary_value > 0) {
        T digit = binary_value % 10;     // Get the least significant digit
        bcd_value != (digit << shift);   // Add the digit to the BCD value
        binary_value /= 10;              // Remove the least significant digit
        shift += 4;                      // Move to the next BCD digit (4 bits)
    }

    return bcd_value;
}
