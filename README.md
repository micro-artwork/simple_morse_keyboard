# Simple Morse Keyboard

The Simple Morse Keyboard made with Microchip PIC32MX(PIC32MX230F256B) device.

More details: https://opensourcehardware.io/posts/projects/morse-keyboard/2021-10-28-morse-keyboard-dev/ (Korean)

## Build Environment

* MPLAB X IDE v5.50
* XC32 Compiler v3.01
* MPLAB Harmony 3

## Features

### Signal Indicator

Dot(dit) and dash(dah) are displayed on monitor or indicated using bi-color LED depending on signal length. The indicators are able to be on and off via DIP switch independently.

### Input Sound

It makes a sound on keystroke and is able to be on/off via DIP switch.

### Custom Code

Custom codes are defined to input some keys that are not compatible with international morse code.

|Key|Custom Code|
|-|-|
|Space|ㅡㅡㅡㆍ|
|CapsLock|ㆍㆍㅡㅡ|
|Right Alt(for EN/KR switching)|ㆍㅡㆍㆍㆍ|
|Enter(Return)|ㅡㅡㅡㅡ|


### Demo

![typing](https://micro-artwork.github.io/images/projects/morse-keyboard/typing.gif)

https://youtu.be/risKpr9GJgU
