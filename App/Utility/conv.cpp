/*******************************************************************************
 *  Author: shettyamar (shettyamar5@aol.com)
 *  Created: 03/Sep/2023 18:47:28
 *  FileName: conv.cpp
 *  Description:
 ******************************************************************************/

/*
 *  Includes
 */
#include "conv.h"
#include <cmath>

/*
 *  Function Definitions
 */

void floatToChar(float x, char *p, uint8_t precision, size_t bufferSize) {
    char *s = p + bufferSize;   // go to end of buffer
    *--s    = '\0';
    uint16_t decimals;   // variable to store the decimals
    int units;   // variable to store the units (part to left of decimal place)
    int precision_multiplier = pow(10, precision);
    if (x < 0) {   // take care of negative numbers
        decimals = (int) (x * -precision_multiplier)
                   % precision_multiplier;   // make 1000 for 3 decimals etc.
        units = (int) (-1 * x);
    } else {   // positive numbers
        decimals = (int) (x * precision_multiplier) % precision_multiplier;
        units    = (int) x;
    }
    // Check if the unit + decimal fits
    // else truncate the decimal part
    int num_of_digits_units = (units < 10)     ? 1
                              : (units < 100)  ? 2
                              : (units < 1000) ? 3
                                               : 4;
    do {
        // -1 is the space for null termination
        (num_of_digits_units + precision) > (bufferSize - 1) ? decimals /= 10
                                                             : decimals;
        precision--;
    } while (precision);

    do {
        *--s = (decimals % 10) + '0';
        decimals /= 10;   // repeat for as many decimal places as you need
    } while (decimals);
    *--s = '.';

    while (units > 0) {
        *--s = (units % 10) + '0';
        units /= 10;
    }
    if (x < 0)
        *--s = '-';   // unary minus sign for negative numbers
}
