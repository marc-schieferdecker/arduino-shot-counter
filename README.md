# Arduino Shot Counter

## Overview

**An Arduino gun mountable shot counter**

Hardware components used:
* Arduino Board Nano ATMega328
* Joy-it SBC-OLED01
* Joy-it MPU6050
* Joy-it KY004TM

## Wireing

Arduino
* Attach a power source to VIN (+) and GND (-)
* (or use usb power for testing)

SBC-OLED01
* GND -> Arduino GND
* VCC -> Arduino 5V
* SCL -> Arduino A4
* SDA -> Arduino A5

MPU6050
* GND and AD0 -> Arduino GND
* VCC -> Arduino 5V
* SDA -> Arduino A5
* SCL -> Arduino A4

KY004TM
* Pin "-" -> Arduino GND
* Middle pin -> Arduino 5V
* Pin "S" -> Arduino D10

## Visual Studio Code settings

VSCODE C++ Clang_format_fallback Style

`{ BasedOnStyle: Google, IndentWidth: 4, ColumnLimit: 0}`
