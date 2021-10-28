#include "morse.h"
#include "keyboard.h"
#include "usb/usb_hid.h"

#define SIGNAL_PATTERN_NUMBER       (68)
#define START_ASCII_CODE            (0x20) // space

const uint8_t codeTable[SIGNAL_PATTERN_NUMBER][2] = {    
    { 0x0E, 4 }, // Space   1110    (ASCII: 0x20) custom code
    { 0x03, 4 }, // CAPS    0011    (ASCII: 0x21) custom code
    { 0x12, 6 }, // "       010010  (ASCII: 0x22)        
    { 0x08, 5 }, // R-ALT   01000   (ASCII: 0x23) custom code for en/ko switching
    { 0x09, 7 }, // $       0001001 (ASCII: 0x24)        
    { 0x00, 0 }, //                 (ASCII: 0x25) for offset
    { 0x00, 0 }, //                 (ASCII: 0x26) for offset    
    { 0x1E, 6 }, // '       011110  (ASCII: 0x27)
    { 0x16, 5 }, // (       10110   (ASCII: 0x28)
    { 0x2D, 6 }, // )       101101  (ASCII: 0x29)    
    { 0x00, 0 }, //                 (ASCII: 0x2A) for offset
    { 0x00, 0 }, //                 (ASCII: 0x2B) for offset    
    { 0x33, 6 }, // ,       110011  (ASCII: 0x2C)
    { 0x21, 6 }, // -       100001  (ASCII: 0x2D)
    { 0x15, 6 }, // .       010101  (ASCII: 0x2E)
    { 0x12, 5 }, // /       10010   (ASCII: 0x2F)    
    { 0x1F, 5 }, // 0       11111   (ASCII, 0x30)
    { 0x0F, 5 }, // 1       01111
    { 0x07, 5 }, // 2       00111
    { 0x03, 5 }, // 3       00011
    { 0x01, 5 }, // 4       00001
    { 0x00, 5 }, // 5       00000
    { 0x10, 5 }, // 6       10000
    { 0x18, 5 }, // 7       11000
    { 0x1C, 5 }, // 8       11100
    { 0x1E, 5 }, // 9       11110   (ASCII, 0x39)        
    { 0x38, 6 }, // :       111000  (ASCII: 0x3A)
    { 0x15, 5 }, // ;       10101   (ASCII: 0x3B)    
    { 0x00, 0 }, //                 (ASCII: 0x3C) for offset
    { 0x00, 0 }, //                 (ASCII: 0x3D) for offset
    { 0x00, 0 }, //                 (ASCII: 0x3E) for offset
    { 0x0C, 6 }, // ?       001100  (ASCII: 0x3F)
    { 0x1A, 6 }, // @       011010  (ASCII: 0x40)
    { 0x01, 2 }, // A       01      (ASCII: 0x41)
    { 0x08, 4 }, // B       1000
    { 0x0A, 4 }, // C       1010
    { 0x04, 3 }, // D       100    
    { 0x00, 1 }, // E       0
    { 0x02, 4 }, // F       0010
    { 0x06, 3 }, // G       110    
    { 0x00, 4 }, // H       0000
    { 0x00, 2 }, // I       00
    { 0x07, 4 }, // J       0111
    { 0x05, 3 }, // K       101
    { 0x04, 4 }, // L       0100
    { 0x03, 2 }, // M       11
    { 0x02, 2 }, // N       10
    { 0x07, 3 }, // O       111
    { 0x06, 4 }, // P       0110
    { 0x0D, 4 }, // Q       1101
    { 0x02, 3 }, // R       010    
    { 0x00, 3 }, // S       000    
    { 0x01, 1 }, // T       1
    { 0x01, 3 }, // U       001
    { 0x01, 4 }, // V       0001
    { 0x03, 3 }, // W       011
    { 0x09, 4 }, // X       1001    
    { 0x0B, 4 }, // Y       1011
    { 0x0C, 4 }, // Z       1100    (ASCII: 0x5A)    
    { 0x00, 0 }, //                 (ASCII: 0x5B) for offset
    { 0x00, 0 }, //                 (ASCII: 0x5C) for offset
    { 0x00, 0 }, //                 (ASCII: 0x5D) for offset
    { 0x00, 0 }, //                 (ASCII: 0x5E) for offset
    { 0x0D, 6 }, // _       001101  (ASCII: 0x5F)        
    { 0x00, 0 }, //                 (ASCII: 0x60) for offset
    { 0x00, 8 }, // DEL     00000000(ASCII: 0x61) 
    { 0x02, 5 }, // DEL(KR) 00010   (ASCII: 0x62) for korean
    { 0x0F, 4 }, // ENTER   1111    (ASCII: 0x63) custom code
};

uint8_t buffer = 0;
uint8_t signalCount = 0;

static void _SetModifiers(uint8_t ch, MORSE_RESULT_T *code) {
    switch (ch) { 
        case '$':
        case '@':
        case '\"':
        case '_':
        case '?':
        case ':':
        case '(':
        case ')':
            // shift
            code->shiftPressed = true;
            break;
        case '#':   // right alt
            code->altPressed = true;
            break;
        default:
            break;
    }
}


static uint8_t _AsciiToKeycode(uint8_t ch) {
    switch (ch) {                
        case ' ':
            return USB_HID_KEYBOARD_KEYPAD_KEYBOARD_SPACEBAR;
        case '!':
            return USB_HID_KEYBOARD_KEYPAD_KEYBOARD_CAPS_LOCK;
        case '#':
            return USB_HID_KEYBOARD_KEYPAD_RESERVED_NO_EVENT_INDICATED;
        case '1':
        case '2':
        case '3':        
        case '5':
        case '6':
        case '7':        
            return (ch - 0x31) + USB_HID_KEYBOARD_KEYPAD_KEYBOARD_1_AND_EXCLAMATION_POINT;
        case '0':
        case ')':
            return USB_HID_KEYBOARD_KEYPAD_KEYBOARD_0_AND_CLOSE_PARENTHESIS;
        case '4':
        case '$':
            return USB_HID_KEYBOARD_KEYPAD_KEYBOARD_4_AND_DOLLAR;
        case '8':
        case '@':
            return USB_HID_KEYBOARD_KEYPAD_KEYBOARD_8_AND_ASTERISK;    
        case '9':
        case '(':
            return USB_HID_KEYBOARD_KEYPAD_KEYBOARD_9_AND_OPEN_PARENTHESIS;    
        case '\'':
        case '\"':
            return USB_HID_KEYBOARD_KEYPAD_KEYBOARD_APOSTROPHE_AND_QUOTE;
        case ',':
            return USB_HID_KEYBOARD_KEYPAD_KEYBOARD_COMMA_AND_LESS_THAN;
        case '.':
            return USB_HID_KEYBOARD_KEYPAD_KEYBOARD_PERIOD_AND_GREATER_THAN;
        case '-':
        case '_':
            return USB_HID_KEYBOARD_KEYPAD_KEYBOARD_MINUS_AND_UNDERSCORE;
        case '/':
        case '?':
            return USB_HID_KEYBOARD_KEYPAD_KEYBOARD_FORWARD_SLASH_AND_QUESTION_MARK;                    
        case ';':
        case ':':
            return USB_HID_KEYBOARD_KEYPAD_KEYBOARD_SEMICOLON_AND_COLON;        
        case 'a':
        case 'b':
            return USB_HID_KEYBOARD_KEYPAD_KEYBOARD_DELETE;
        case 'c':
            return USB_HID_KEYBOARD_KEYPAD_KEYBOARD_RETURN_ENTER;
        default: {            
            if (ch >= 'A' && ch <= 'Z') {
                return (ch - 0x41) + USB_HID_KEYBOARD_KEYPAD_KEYBOARD_A;
            }            
        } return USB_HID_KEYBOARD_KEYPAD_RESERVED_NO_EVENT_INDICATED;
    }    
}

void MORSE_Reset() {
    buffer = 0;
    signalCount = 0;
}

MORSE_RESULT_T MORSE_ParseCode() {
    
    MORSE_RESULT_T code = { false, false, false, 0, 0, 0 };
    
    for (uint8_t index = 0; index < SIGNAL_PATTERN_NUMBER; index++) {
        if (codeTable[index][0] == buffer && codeTable[index][1] == signalCount) {
            code.valid = true;
            code.signal = buffer;
            code.ascii = START_ASCII_CODE + index;            
            code.keycode = _AsciiToKeycode(START_ASCII_CODE + index);
            _SetModifiers(START_ASCII_CODE + index, &code);
            break;
        };
    }
    
    MORSE_Reset();
    return code;    
}

MORSE_SIGNAL_T MORSE_InputSignal(uint16_t pressedTime) {    

    if (pressedTime >  MORSE_TIMEOUT) {
        return END;
    } 
    
    if (pressedTime >  MORSE_DAH_MIN) {
        buffer = (buffer << 1) | 0x01;
        signalCount += 1;
        return DAH;
    } 
    
    if (pressedTime >  MORSE_DIT_MIN) {
        buffer = (buffer << 1) & 0xFE;
        signalCount += 1;
        return DIT;
    }
    
    return END;
}


