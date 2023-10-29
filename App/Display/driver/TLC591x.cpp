/* Library to interface with the Texas Instruments TLC5916 and TLC5917
   8-Channel Constant-Current LED Sink Drivers.
   https://github.com/Andy4495/TLC591x

*/
/* Version History
   1.0.0    08/03/2018  A.T.   Original
   1.1.0    09/25/2020  A.T.   Support more daisy-chained displays.
                               Fixed array bound issue in previous
                               un-numbered file version.
   1.2.0    01/17/2021  A.T.   Add support for special mode.
   1.3.0    08/31/2022  Andy4495  Add hardware SPI support
   1.4.0    09/02/2022  Andy4495  Fix hardware SPI support for msp432r and tivac
                                  Fix potential race condition between pwm
   brightness and special mode 1.4.2    09/13/2022  Andy4495  Fix compilation
   issue with ESP32 (#6)
*/

#include "TLC591x.h"
#include "stm32g0xx_hal.h"

extern SPI_HandleTypeDef hspi1;

#define OE_PORT  GPIOA
#define OE_PIN   GPIO_PIN_15

// FIXME: Map Pin assignment
#define LE_PORT  GPIOA
#define LE_PIN   GPIO_PIN_1

#define CLK_PORT GPIOB
#define CLK_PIN  GPIO_PIN_3

void TLC591x::digitalWrite(TLC591x::TLC591x_PINS pin, uint8_t state) {
    switch (pin) {
    case TLC591x::TLC591x_PINS::OE:
        HAL_GPIO_WritePin(OE_PORT, OE_PIN, (GPIO_PinState) state);
        break;
    case TLC591x::TLC591x_PINS::LE:
        HAL_GPIO_WritePin(LE_PORT, LE_PIN, (GPIO_PinState) state);
        break;

    case TLC591x_PINS::CLK:
        HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, (GPIO_PinState) state);
        break;
    }
}

void TLC591x::pinMode(TLC591x_PINS pin, PIN_TYPE pin_type) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    uint32_t         sel_pin         = 0;
    GPIO_TypeDef    *sel_gpio        = 0;

    switch (pin) {
    case TLC591x::TLC591x_PINS::OE:
        sel_pin  = OE_PIN;
        sel_gpio = OE_PORT;
        break;
    case TLC591x::TLC591x_PINS::LE:
        sel_pin  = LE_PIN;
        sel_gpio = LE_PORT;
        break;

    case TLC591x_PINS::CLK:
        sel_pin  = CLK_PIN;
        sel_gpio = CLK_PORT;
        break;
    }
    if ((sel_pin) && (sel_gpio)) {
        GPIO_InitStruct.Pin  = sel_pin;
        GPIO_InitStruct.Mode = (pin_type == PIN_TYPE::INPUT)
                                   ? GPIO_MODE_INPUT
                                   : GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(sel_gpio, &GPIO_InitStruct);
    }
}

// Hardware SPI constructors
TLC591x::TLC591x(uint8_t n) {
    numchips = n;
    digitalWrite(TLC591x::TLC591x_PINS::OE,
                 TLC591x::PIN_STATE::HIGH);   // Default is disabled
    enableState = TLC_DISABLED;
    pwmMode = TLC_DISABLED;   // Used to indicate if the the displayBrightness()
                              // method was used
    // pinMode(OE_pin, OUTPUT);
    spiType = HW_SPI;
    init();
}

void TLC591x::init() {
    if (numchips < MINCHIPS)
        numchips = MINCHIPS;
    if (numchips > MAXCHIPS)
        numchips = MAXCHIPS;

    // TODO: COnfigure OE & LE as output

    digitalWrite(TLC591x::TLC591x_PINS::LE, TLC591x::PIN_STATE::LOW);
}

void TLC591x::print(const char *s) {
    int     i;
    char    c;
    uint8_t pos;

    for (i = numchips - 1; i >= 0; i--) {
        // Need a range check and adjustment, since segment map starts
        // at ASCII 32 (element 0) and ends at ASCII 0x7f (element 96)
        // Out of range default to a blank character
        // if (s[i] > 127) pos = 0; --> This would be a negative value, included
        // in next check
        if (s[i] < 32)
            pos = 0;
        else
            pos = s[i] - 32;
        c = ICM7218_segment_map[pos];
        write(c);
    }
    toggleLE();
}

void TLC591x::print(unsigned int n) {
    uint8_t size;
    if (numchips > 2)
        size = 2;
    else
        size = numchips;
    for (uint8_t i = 0; i < size; i++) {
        write(n);
        n = n / 256;   // Get the next most significant uint8_t
    }
    toggleLE();
}

void TLC591x::printDirect(const uint8_t *s) {
    for (int i = numchips - 1; i >= 0; i--) {
        write(s[i]);
    }
    toggleLE();
}

void TLC591x::displayEnable() {
    // if (OE_pin != NO_PIN) {
    digitalWrite(TLC591x::TLC591x_PINS::OE, PIN_STATE::LOW);
    enableState = TLC_ENABLED;
    pwmMode     = TLC_DISABLED;
    // }
}

void TLC591x::displayDisable() {
    // if (OE_pin != NO_PIN) {
    digitalWrite(TLC591x::TLC591x_PINS::OE, PIN_STATE::HIGH);
    enableState = TLC_DISABLED;
    pwmMode     = TLC_DISABLED;
    // }
}

void TLC591x::normalMode() {
    // if (OE_pin != NO_PIN) {
    if (spiType == HW_SPI) {
        // SPI.end();
        // End spi transmission
        // Change CLK pin mode

        digitalWrite(TLC591x_PINS::CLK, PIN_STATE::LOW);
        pinMode(TLC591x_PINS::CLK, PIN_TYPE::OUTPUT);
    }
    pwmMode = TLC_DISABLED;
    digitalWrite(TLC591x_PINS::OE, PIN_STATE::HIGH);
    toggleCLK();
    digitalWrite(TLC591x_PINS::OE, PIN_STATE::LOW);
    toggleCLK();
    digitalWrite(TLC591x_PINS::OE, PIN_STATE::HIGH);   // Disables the display
    toggleCLK();
    toggleCLK();   // Mode switching
    toggleCLK();   // Now in normal mode
    if (enableState == TLC_ENABLED)
        displayEnable();   // Re-enable display if it was enabled previously
    if (spiType == HW_SPI) {
        // TODO: Check is a spi re-init is needed?
        // SPI.begin();
    }
    // }
}

void TLC591x::specialMode() {
    // if (OE_pin != NO_PIN) {
    if (spiType == HW_SPI) {
        // TODO: Check is SPI deinit needed?
        // SPI.end();
        digitalWrite(TLC591x_PINS::CLK, PIN_STATE::LOW);
        pinMode(TLC591x_PINS::CLK, PIN_TYPE::OUTPUT);
    }
    pwmMode = TLC_DISABLED;
    digitalWrite(TLC591x_PINS::OE, PIN_STATE::HIGH);
    toggleCLK();
    digitalWrite(TLC591x_PINS::OE, PIN_STATE::LOW);
    toggleCLK();
    digitalWrite(TLC591x_PINS::OE, PIN_STATE::HIGH);   // Disables the display
    toggleCLK();
    digitalWrite(TLC591x_PINS::LE, PIN_STATE::HIGH);
    toggleCLK();   // Mode switching
    digitalWrite(TLC591x_PINS::LE, PIN_STATE::LOW);
    toggleCLK();   // Now in special mode
    // Switching to special mode disables the display
    // normalMode() automatically re-enables display if it was previously
    // enabled before specialMode()
    if (spiType == HW_SPI) {
        // TODO: Check if re-init needed
        // SPI.begin();
    }
    // }
}

void TLC591x::displayBrightness(uint8_t b) {
    // if (OE_pin != NO_PIN) {
    // FIXME: Refer datasheet!!!
    // analogWrite(OE_pin, b);
    pwmMode     = TLC_ENABLED;
    enableState = TLC_ENABLED;
    brightness  = b;
    // }
}

void TLC591x::write(uint8_t n) {

    // Since the Time display is absent,
    // sent the data for Safe days and Safe Year
    HAL_SPI_Transmit(&hspi1, &n, sizeof(n), (-1));   // Max timeout set
    toggleLE();

    // if (/*OE_pin != NO_PIN && */ pwmMode == TLC_ENABLED)
    //     digitalWrite(TLC591x::TLC591x_PINS::OE, TLC_ENABLED);
    // Need a continuous level on OE when writing
    // if (spiType == SW_SPI) {
    //     digitalWrite(SDI_pin, n & 0x01);
    //     toggleCLK();
    //     digitalWrite(SDI_pin, n & 0x02);
    //     toggleCLK();
    //     digitalWrite(SDI_pin, n & 0x04);
    //     toggleCLK();
    //     digitalWrite(SDI_pin, n & 0x08);
    //     toggleCLK();
    //     digitalWrite(SDI_pin, n & 0x10);
    //     toggleCLK();
    //     digitalWrite(SDI_pin, n & 0x20);
    //     toggleCLK();
    //     digitalWrite(SDI_pin, n & 0x40);
    //     toggleCLK();
    //     digitalWrite(SDI_pin, n & 0x80);
    //     toggleCLK();
    // }
    // #if !defined(ENERGIA_ARCH_MSP432R) && !defined(ENERGIA_ARCH_MSP432E)
    // else {
    //     SPI.beginTransaction(SPISettings(10000000, LSBFIRST, SPI_MODE0));
    //     SPI.transfer(n);
    //     toggleLE();
    //     SPI.endTransaction();
    // }
    // #endif
    // if (/*OE_pin != NO_PIN && */ pwmMode == TLC_ENABLED)
    //     displayBrightness(brightness);   // Switch back to previous setting
}

void TLC591x::toggleLE() {
    // TLC5916 minimum LE pulse time is 20 ns, so don't need to worry about
    // putting in a hard-coded delay.
    digitalWrite(TLC591x::TLC591x_PINS::LE, PIN_STATE::HIGH);
    digitalWrite(TLC591x::TLC591x_PINS::LE, PIN_STATE::LOW);
}

void TLC591x::toggleCLK() {
    // TLC5916 minimum CLK pulse time is 20 ns, so don't need to worry about
    // putting in a hard-coded delay.
    digitalWrite(TLC591x_PINS::CLK, PIN_STATE::HIGH);
    digitalWrite(TLC591x_PINS::CLK, PIN_STATE::LOW);
}
