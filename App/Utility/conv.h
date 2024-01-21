/*******************************************************************************
 *  Author: shettyamar (shettyamar5@aol.com)
 *  Created: 03/Sep/2023 18:12:52
 *  FileName: conv.h
 *  Description:
 ******************************************************************************/

/*
 *  Includes
 */
#ifndef _CONV_H_
#define _CONV_H_

#include <cstdint>
#include <cstdlib>

void floatToChar(float x, char *p, uint8_t precision, size_t bufferSize);
#endif   // End of _CONV_H_