#ifndef _MORSE_H 
#define _MORSE_H

#include <stdint.h>
#include <stdbool.h>

#define MORSE_BUFFER_SIZE   (6)

// unit size
#define MORSE_DIT           (1)
#define MORSE_DAH           (3)
#define MORSE_DIT_DAH_SPACE (1)
#define MORSE_CHAR_SPACE    (3)
#define MORSE_WORD_SPACE    (7)

// msec
#define MORSE_DIT_LENGTH    (150)
#define MORSE_DAH_LENGTH    (MORSE_DIT_LENGTH * 3)
#define TOLERANCE_TIME      (50)

#define MORSE_DIT_MIN       (MORSE_DIT_LENGTH - TOLERANCE_TIME)
#define MORSE_DAH_MIN       (MORSE_DAH_LENGTH - MORSE_DIT_LENGTH)
#define MORSE_TIMEOUT       (MORSE_DAH_LENGTH * 2)

#define MORSE_IS_TIMEOUT(time)    (time > MORSE_TIMEOUT)

typedef enum {
    DIT = 0,
    DAH,
    END = -1,
} MORSE_SIGNAL_T;

typedef struct {
    bool valid;
    bool shiftPressed;
    bool altPressed;
    uint8_t ascii;
    uint8_t signal;    
    uint8_t keycode;    
} MORSE_RESULT_T;

void MORSE_Reset();
MORSE_SIGNAL_T MORSE_InputSignal(uint16_t);
MORSE_RESULT_T MORSE_ParseCode();

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* _MORSE_H */
