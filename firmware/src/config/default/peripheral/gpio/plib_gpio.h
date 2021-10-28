/*******************************************************************************
  GPIO PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_gpio.h

  Summary:
    GPIO PLIB Header File

  Description:
    This library provides an interface to control and interact with Parallel
    Input/Output controller (GPIO) module.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#ifndef PLIB_GPIO_H
#define PLIB_GPIO_H

#include <device.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************


/*** Macros for INPUT_KEY pin ***/
#define INPUT_KEY_Set()               (LATASET = (1<<0))
#define INPUT_KEY_Clear()             (LATACLR = (1<<0))
#define INPUT_KEY_Toggle()            (LATAINV= (1<<0))
#define INPUT_KEY_OutputEnable()      (TRISACLR = (1<<0))
#define INPUT_KEY_InputEnable()       (TRISASET = (1<<0))
#define INPUT_KEY_Get()               ((PORTA >> 0) & 0x1)
#define INPUT_KEY_PIN                  GPIO_PIN_RA0
#define INPUT_KEY_InterruptEnable()   (CNENASET = (1<<0))
#define INPUT_KEY_InterruptDisable()  (CNENACLR = (1<<0))

/*** Macros for ADJ_UNIT_KEY pin ***/
#define ADJ_UNIT_KEY_Set()               (LATASET = (1<<1))
#define ADJ_UNIT_KEY_Clear()             (LATACLR = (1<<1))
#define ADJ_UNIT_KEY_Toggle()            (LATAINV= (1<<1))
#define ADJ_UNIT_KEY_OutputEnable()      (TRISACLR = (1<<1))
#define ADJ_UNIT_KEY_InputEnable()       (TRISASET = (1<<1))
#define ADJ_UNIT_KEY_Get()               ((PORTA >> 1) & 0x1)
#define ADJ_UNIT_KEY_PIN                  GPIO_PIN_RA1

/*** Macros for DIP_TYPING_IND pin ***/
#define DIP_TYPING_IND_Set()               (LATBSET = (1<<2))
#define DIP_TYPING_IND_Clear()             (LATBCLR = (1<<2))
#define DIP_TYPING_IND_Toggle()            (LATBINV= (1<<2))
#define DIP_TYPING_IND_OutputEnable()      (TRISBCLR = (1<<2))
#define DIP_TYPING_IND_InputEnable()       (TRISBSET = (1<<2))
#define DIP_TYPING_IND_Get()               ((PORTB >> 2) & 0x1)
#define DIP_TYPING_IND_PIN                  GPIO_PIN_RB2

/*** Macros for DIP_LED_IND pin ***/
#define DIP_LED_IND_Set()               (LATBSET = (1<<3))
#define DIP_LED_IND_Clear()             (LATBCLR = (1<<3))
#define DIP_LED_IND_Toggle()            (LATBINV= (1<<3))
#define DIP_LED_IND_OutputEnable()      (TRISBCLR = (1<<3))
#define DIP_LED_IND_InputEnable()       (TRISBSET = (1<<3))
#define DIP_LED_IND_Get()               ((PORTB >> 3) & 0x1)
#define DIP_LED_IND_PIN                  GPIO_PIN_RB3

/*** Macros for DIP_CUSTOM_UNIT pin ***/
#define DIP_CUSTOM_UNIT_Set()               (LATBSET = (1<<4))
#define DIP_CUSTOM_UNIT_Clear()             (LATBCLR = (1<<4))
#define DIP_CUSTOM_UNIT_Toggle()            (LATBINV= (1<<4))
#define DIP_CUSTOM_UNIT_OutputEnable()      (TRISBCLR = (1<<4))
#define DIP_CUSTOM_UNIT_InputEnable()       (TRISBSET = (1<<4))
#define DIP_CUSTOM_UNIT_Get()               ((PORTB >> 4) & 0x1)
#define DIP_CUSTOM_UNIT_PIN                  GPIO_PIN_RB4

/*** Macros for DIP_BUZZER pin ***/
#define DIP_BUZZER_Set()               (LATASET = (1<<4))
#define DIP_BUZZER_Clear()             (LATACLR = (1<<4))
#define DIP_BUZZER_Toggle()            (LATAINV= (1<<4))
#define DIP_BUZZER_OutputEnable()      (TRISACLR = (1<<4))
#define DIP_BUZZER_InputEnable()       (TRISASET = (1<<4))
#define DIP_BUZZER_Get()               ((PORTA >> 4) & 0x1)
#define DIP_BUZZER_PIN                  GPIO_PIN_RA4

/*** Macros for LED_IND_DIT pin ***/
#define LED_IND_DIT_Set()               (LATBSET = (1<<8))
#define LED_IND_DIT_Clear()             (LATBCLR = (1<<8))
#define LED_IND_DIT_Toggle()            (LATBINV= (1<<8))
#define LED_IND_DIT_OutputEnable()      (TRISBCLR = (1<<8))
#define LED_IND_DIT_InputEnable()       (TRISBSET = (1<<8))
#define LED_IND_DIT_Get()               ((PORTB >> 8) & 0x1)
#define LED_IND_DIT_PIN                  GPIO_PIN_RB8

/*** Macros for LED_IND_DAH pin ***/
#define LED_IND_DAH_Set()               (LATBSET = (1<<9))
#define LED_IND_DAH_Clear()             (LATBCLR = (1<<9))
#define LED_IND_DAH_Toggle()            (LATBINV= (1<<9))
#define LED_IND_DAH_OutputEnable()      (TRISBCLR = (1<<9))
#define LED_IND_DAH_InputEnable()       (TRISBSET = (1<<9))
#define LED_IND_DAH_Get()               ((PORTB >> 9) & 0x1)
#define LED_IND_DAH_PIN                  GPIO_PIN_RB9


// *****************************************************************************
/* GPIO Port

  Summary:
    Identifies the available GPIO Ports.

  Description:
    This enumeration identifies the available GPIO Ports.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all ports are available on all devices.  Refer to the specific
    device data sheet to determine which ports are supported.
*/

typedef enum
{
    GPIO_PORT_A = 0,
    GPIO_PORT_B = 1,
} GPIO_PORT;

// *****************************************************************************
/* GPIO Port Pins

  Summary:
    Identifies the available GPIO port pins.

  Description:
    This enumeration identifies the available GPIO port pins.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all pins are available on all devices.  Refer to the specific
    device data sheet to determine which pins are supported.
*/

typedef enum
{
    GPIO_PIN_RA0 = 0,
    GPIO_PIN_RA1 = 1,
    GPIO_PIN_RA2 = 2,
    GPIO_PIN_RA3 = 3,
    GPIO_PIN_RA4 = 4,
    GPIO_PIN_RB0 = 16,
    GPIO_PIN_RB1 = 17,
    GPIO_PIN_RB2 = 18,
    GPIO_PIN_RB3 = 19,
    GPIO_PIN_RB4 = 20,
    GPIO_PIN_RB5 = 21,
    GPIO_PIN_RB7 = 23,
    GPIO_PIN_RB8 = 24,
    GPIO_PIN_RB9 = 25,
    GPIO_PIN_RB10 = 26,
    GPIO_PIN_RB11 = 27,
    GPIO_PIN_RB13 = 29,
    GPIO_PIN_RB14 = 30,
    GPIO_PIN_RB15 = 31,

    /* This element should not be used in any of the GPIO APIs.
       It will be used by other modules or application to denote that none of the GPIO Pin is used */
    GPIO_PIN_NONE = -1

} GPIO_PIN;

typedef  void (*GPIO_PIN_CALLBACK) ( GPIO_PIN pin, uintptr_t context);

void GPIO_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

uint32_t GPIO_PortRead(GPIO_PORT port);

void GPIO_PortWrite(GPIO_PORT port, uint32_t mask, uint32_t value);

uint32_t GPIO_PortLatchRead ( GPIO_PORT port );

void GPIO_PortSet(GPIO_PORT port, uint32_t mask);

void GPIO_PortClear(GPIO_PORT port, uint32_t mask);

void GPIO_PortToggle(GPIO_PORT port, uint32_t mask);

void GPIO_PortInputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortOutputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortInterruptEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortInterruptDisable(GPIO_PORT port, uint32_t mask);

// *****************************************************************************
// *****************************************************************************
// Section: Local Data types and Prototypes
// *****************************************************************************
// *****************************************************************************

typedef struct {

    /* target pin */
    GPIO_PIN                 pin;

    /* Callback for event on target pin*/
    GPIO_PIN_CALLBACK        callback;

    /* Callback Context */
    uintptr_t               context;

} GPIO_PIN_CALLBACK_OBJ;

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

static inline void GPIO_PinWrite(GPIO_PIN pin, bool value)
{
    GPIO_PortWrite((GPIO_PORT)(pin>>4), (uint32_t)(0x1) << (pin & 0xF), (uint32_t)(value) << (pin & 0xF));
}

static inline bool GPIO_PinRead(GPIO_PIN pin)
{
    return (bool)(((GPIO_PortRead((GPIO_PORT)(pin>>4))) >> (pin & 0xF)) & 0x1);
}

static inline bool GPIO_PinLatchRead(GPIO_PIN pin)
{
    return (bool)((GPIO_PortLatchRead((GPIO_PORT)(pin>>4)) >> (pin & 0xF)) & 0x1);
}

static inline void GPIO_PinToggle(GPIO_PIN pin)
{
    GPIO_PortToggle((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinSet(GPIO_PIN pin)
{
    GPIO_PortSet((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinClear(GPIO_PIN pin)
{
    GPIO_PortClear((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinInputEnable(GPIO_PIN pin)
{
    GPIO_PortInputEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinOutputEnable(GPIO_PIN pin)
{
    GPIO_PortOutputEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinInterruptEnable(GPIO_PIN pin)
{
    GPIO_PortInterruptEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinInterruptDisable(GPIO_PIN pin)
{
    GPIO_PortInterruptDisable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

bool GPIO_PinInterruptCallbackRegister(
    GPIO_PIN pin,
    const   GPIO_PIN_CALLBACK callBack,
    uintptr_t context
);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END
#endif // PLIB_GPIO_H
