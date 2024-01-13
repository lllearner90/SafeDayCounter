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

void floatToString(float value, char *buffer, uint8_t precision,
                   size_t bufferSize) {
    // Check if the buffer size is sufficient
    if (bufferSize == 0) {
        // Handle error: Buffer size is zero
        // TODO: throw an exception / log error
        return;
    }

    // Handle negative values
    if (value < 0.0f) {
        if (bufferSize < 2) {
            // Handle error: Insufficient space for the negative sign
            // TODO: throw an exception
            return;
        }
        *buffer++ = '-';
        value     = -value;
        bufferSize--;
    }

    // Extract integer part
    uint32_t intValue  = static_cast<uint32_t>(value);
    volatile uint32_t fracValue = static_cast<uint32_t>(
        (value - static_cast<float>(intValue)) * pow(10, precision));

    // Convert the integer part to string manually
    do {
        if (bufferSize < 1) {
            // Handle error: Insufficient space for the integer part
            // TODO: throw an exception
            return;
        }

        uint32_t digit = intValue % 10;
        intValue /= 10;

        *buffer++ = '0' + digit;
        bufferSize--;

    } while (intValue > 0);

    // Reverse the integer part in the buffer
    // for (size_t i = 0, j = bufferSize - 1; i < j; ++i, --j) {
    //     char temp = buffer[i];
    //     buffer[i] = buffer[j];
    //     buffer[j] = temp;
    // }

    // buffer += bufferSize;
    // bufferSize = 1;   // Reset bufferSize to 1 for the decimal point

    // Check if there's enough space for the decimal point
    if (bufferSize < 1) {
        // Handle error: Insufficient space for the decimal point
        // TODO: throw an exception
        return;
    }

    // Add decimal point
    *buffer++ = '.';
    bufferSize--;

    // Convert the fractional part to string manually
    while (precision > 0) {
        if (bufferSize < 1) {
            // Handle error: Insufficient space for the fractional part
            // TODO: throw an exception
            return;
        }

        uint32_t digit = fracValue % 10;
        fracValue /= 10;

        *buffer++ = '0' + digit;
        bufferSize--;

        precision--;
    }

    // Null-terminate the string
    if (bufferSize >= 1) {
        *buffer = '\0';
    } else {
        // Handle error: Insufficient space for null-termination
        // TODO: throw an exception
    }
}
