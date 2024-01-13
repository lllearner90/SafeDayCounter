/* Library to interface with the Texas Instruments TLC5916 and TLC5917
   8-Channel Constant-Current LED Sink Drivers.
   https://github.com/Andy4495/TLC591x

*/
/* Version History
   1.0.0    08/03/2018  A.T.   Original
   1.1.0    09/25/2020  A.T.   Support more daisy-chained digits.
   1.2.0    01/17/2021  A.T.   Add support for special mode.
   1.3.0    08/31/2022  Andy4495  Add hardware SPI support
   1.4.0    09/02/2022  Andy4495  Fix hardware SPI support for msp432r and tivac
                                  Fix potential race condition between pwm
   brightness and special mode 1.4.2    09/13/2022  Andy4495  Fix compilation
   issue with ESP32 (#6)
*/
#ifndef TLC591x_LIBRARY
#define TLC591x_LIBRARY

#include <stdint.h>

// MSP432 and Tiva platforms do not define SPI SCK signal in core library
// #if defined(ENERGIA_ARCH_TIVAC) || defined(ENERGIA_ARCH_MSP432R)
// #if !defined(SCK)
// #define SCK 7
// #endif
// #endif

class TLC591x {
  public:
    enum { NO_PIN = 255 };
    enum INTERFACE_TYPE {
        SW_SPI,
        HW_SPI
    };

    // Hardware SPI interface
    TLC591x(uint8_t n, INTERFACE_TYPE interface_type);

    void print(const char *s);
    void print(unsigned int n);
    void printDirect(const uint8_t *s);
    void displayEnable();
    void displayDisable();
    void normalMode();
    void specialMode();
    void displayBrightness(uint8_t b);

    // private:
    enum PIN_STATE { LOW = 0, HIGH = 1 };
    enum PIN_TYPE { INPUT, OUTPUT };
    enum class TLC591x_PINS {
        OE,
        LE,
        CLK,
        SDI
    };
    enum POWER_MODE { WAKEUP = 1, SHUTDOWN = 0 };
    enum {
        MINCHIPS = 1,
        MAXCHIPS = 254
    };
    enum EN_DIS { TLC_ENABLED = 0, TLC_DISABLED = 255 };
    EN_DIS enableState;
    EN_DIS pwmMode;   // Used to keep track of whether the displayBrightness()
                      // method was used
    uint8_t brightness;
    uint8_t numchips;

    INTERFACE_TYPE interface_type;

    void write(uint8_t n);
    void toggleLE();
    void toggleCLK();
    void init();
    void digitalWrite(TLC591x_PINS pin, uint8_t state);
    void pinMode(TLC591x_PINS pin, PIN_TYPE pin_type);
    void analogWrite(uint8_t duty_cycle);
    void sendValue(unsigned char pattern);

    unsigned char convertNumberToPattern(unsigned char number);
};

// Conversion of letters to LED segments.
// 0x00 is a blank character and is used for unsupported values.
// Save memory by not defining first 32 ascii characters, since they
// are all control characters
const unsigned char ICM7218_segment_map[96] = {
  //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 00: Control characters
  //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 08: Control characters
  //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 10: Control characters
  //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 18: Control characters
  0x00, 0x67, 0x22, 0x41, 0x18, 0x12, 0x45, 0x20,   // 20: spc ! " # $ % & '
  0x49, 0x51, 0x63, 0x28, 0x10, 0x04, 0x01, 0x2c,   // 28:   ( ) * + , - . /
  0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0,   // 30:   0 1 2 3 4 5 6 7
  0xfe, 0xf6, 0x44, 0x5d, 0x0d, 0x05, 0x15, 0x6c,   // 38:   8 9 : ; < = > ?
  0x00, 0x7e, 0x1f, 0x4b, 0x3d, 0x4f, 0x4e, 0x5b,   // 40:   @ A B C D E F G
  0x3e, 0x0a, 0x39, 0x0f, 0x0b, 0x5c, 0x1c, 0x7b,   // 48:   H I J K L M N O
  0x6e, 0x76, 0x0c, 0x57, 0x4a, 0x3b, 0x3b, 0x59,   // 50:   P Q R S T U V W
  0x3a, 0x37, 0x7d, 0x4b, 0x16, 0x71, 0x62, 0x01,   // 58:   X Y Z [ \ ] ^ _
  0x02, 0x7e, 0x1f, 0x0d, 0x3d, 0x4f, 0x4e, 0x5b,   // 60:   ` a b c d e f g
  0x1e, 0x08, 0x39, 0x0f, 0x0b, 0x5c, 0x1c, 0x1d,   // 68:   h i j k l m n o
  0x6e, 0x76, 0x0c, 0x57, 0x4a, 0x19, 0x19, 0x59,   // 70:   p q r s t u v w
  0x3a, 0x37, 0x7d, 0x4d, 0x08, 0x55, 0x66, 0x00    // 78:   x y z { | } ~ DEL
};
#endif
