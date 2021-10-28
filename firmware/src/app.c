/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

/*Keyboard Report to be transmitted*/
KEYBOARD_INPUT_REPORT __attribute__((aligned(16))) keyboardInputReport USB_ALIGN ;
/* Keyboard output report */
KEYBOARD_OUTPUT_REPORT __attribute__((aligned(16))) keyboardOutputReport USB_ALIGN ;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

void APP_TimerHandler(uint32_t status, uintptr_t context) {
    appData.keyPressedTime++;    
    if (appData.isKeyTyping == true && MORSE_IS_TIMEOUT(appData.keyPressedTime)) {
        appData.morseResult = MORSE_ParseCode();
        appData.isKeyPressed = true;
        appData.isParsingComplete = true;
        appData.isKeyTyping = false;        
        LED_IND_DIT_Clear();
        LED_IND_DAH_Clear();
        SYS_CONSOLE_PRINT("parsed: %c, %2x\r\n", appData.morseResult.ascii, appData.morseResult.ascii);
    }
}

void APP_KeyInputHandler(GPIO_PIN pin, uintptr_t context) {    
    if (!INPUT_KEY_Get()) {
        
        if (DIP_BUZZER_Get()) {
            OCMP3_Enable();    
        }
        
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "on key pressed\r\n");        
        if (appData.isKeyTyping == false) {
            appData.isKeyTyping = true;
            SYS_CONSOLE_PRINT("signal input start\r\n");
        }        
        appData.keyPressedTime = 0;
    
    } else {
        OCMP3_Disable();
        SYS_CONSOLE_PRINT("released: %u\r\n", appData.keyPressedTime);
        switch (MORSE_InputSignal(appData.keyPressedTime)) {
            case DIT:
                // send dot to host
                if (DIP_TYPING_IND_Get()) {
                    appData.key = USB_HID_KEYBOARD_KEYPAD_KEYBOARD_PERIOD_AND_GREATER_THAN;
                    appData.inputCount += 2;    
                }
                if (DIP_LED_IND_Get()) {
                    LED_IND_DIT_Set();
                    LED_IND_DAH_Clear();    
                }                
                appData.isKeyPressed = true;
                break;
                
            case DAH:
                // send underscore to host
                if (DIP_TYPING_IND_Get()) {
                    appData.key = USB_HID_KEYBOARD_KEYPAD_KEYBOARD_MINUS_AND_UNDERSCORE;
                    appData.inputCount += 2;
                }
                if (DIP_LED_IND_Get()) {
                    LED_IND_DAH_Set();
                    LED_IND_DIT_Clear();    
                }                
                appData.isKeyPressed = true;
                break;
                
            default:                
                LED_IND_DIT_Clear();
                LED_IND_DAH_Clear();
                break;
        }                
    }
}


USB_DEVICE_HID_EVENT_RESPONSE APP_USBDeviceHIDEventHandler
(
    USB_DEVICE_HID_INDEX hidInstance,
    USB_DEVICE_HID_EVENT event,
    void * eventData,
    uintptr_t userData
) {
    APP_DATA * appDataObject = (APP_DATA *)userData;

    switch(event)
    {
        case USB_DEVICE_HID_EVENT_REPORT_SENT:

            /* This means the mouse report was sent.
             We are free to send another report */

            appDataObject->isReportSentComplete = true;
            break;

        case USB_DEVICE_HID_EVENT_REPORT_RECEIVED:

            /* This means we have received a report */
            appDataObject->isReportReceived = true;
            break;

        case USB_DEVICE_HID_EVENT_SET_IDLE:

             /* Acknowledge the Control Write Transfer */
           USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);

            /* save Idle rate received from Host */
            appDataObject->idleRate
                   = ((USB_DEVICE_HID_EVENT_DATA_SET_IDLE*)eventData)->duration;
            break;

        case USB_DEVICE_HID_EVENT_GET_IDLE:

            /* Host is requesting for Idle rate. Now send the Idle rate */
            USB_DEVICE_ControlSend(appDataObject->deviceHandle, & (appDataObject->idleRate),1);

            /* On successfully receiving Idle rate, the Host would acknowledge back with a
               Zero Length packet. The HID function driver returns an event
               USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT to the application upon
               receiving this Zero Length packet from Host.
               USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT event indicates this control transfer
               event is complete */

            break;

        case USB_DEVICE_HID_EVENT_SET_PROTOCOL:
            /* Host is trying set protocol. Now receive the protocol and save */
            appDataObject->activeProtocol
                = ((USB_DEVICE_HID_EVENT_DATA_SET_PROTOCOL *)eventData)->protocolCode;

              /* Acknowledge the Control Write Transfer */
            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case  USB_DEVICE_HID_EVENT_GET_PROTOCOL:

            /* Host is requesting for Current Protocol. Now send the Idle rate */
             USB_DEVICE_ControlSend(appDataObject->deviceHandle, &(appDataObject->activeProtocol), 1);

             /* On successfully receiving Idle rate, the Host would acknowledge
               back with a Zero Length packet. The HID function driver returns
               an event USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT to the
               application upon receiving this Zero Length packet from Host.
               USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT event indicates
               this control transfer event is complete */
             break;

        case USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT:
            break;

        default:
            break;
    }

    return USB_DEVICE_HID_EVENT_RESPONSE_NONE;
}

void APP_USBDeviceEventHandler(USB_DEVICE_EVENT event,
        void * eventData, uintptr_t context)
{
    USB_DEVICE_EVENT_DATA_CONFIGURED *configurationValue;

    switch(event)
    {
        case USB_DEVICE_EVENT_SOF:
            
            /* This event is used for switch de-bounce. This flag is reset
             * by the switch process routine. */
            appData.sofEventHasOccurred = true;
            break;
            
        case USB_DEVICE_EVENT_RESET:
        case USB_DEVICE_EVENT_DECONFIGURED:

            /* Device got de-configured */
            appData.isConfigured = false;
            appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
//            LED_Off();
//            GPIO_RA9_Clear();
            
            break;

        case USB_DEVICE_EVENT_CONFIGURED:

            /* Device is configured */
            configurationValue = (USB_DEVICE_EVENT_DATA_CONFIGURED *)eventData;
            if(configurationValue->configurationValue == 1)
            {
                appData.isConfigured = true;

//                LED_On();
//                GPIO_RA9_Set();

                /* Register the Application HID Event Handler. */
                USB_DEVICE_HID_EventHandlerSet(appData.hidInstance,
                        APP_USBDeviceHIDEventHandler, (uintptr_t)&appData);
            }
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
//			LED_Off();
//            GPIO_RA9_Clear();
            break;

        case USB_DEVICE_EVENT_RESUMED:
            break; 

        case USB_DEVICE_EVENT_POWER_DETECTED:
            
            /* Attach the device */
            USB_DEVICE_Attach (appData.deviceHandle);
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:
            
            /* There is no VBUS. We can detach the device */
            USB_DEVICE_Detach(appData.deviceHandle);
//            LED_Off();
//            GPIO_RA9_Clear();
            
            break;
            
        case USB_DEVICE_EVENT_ERROR:
        default:            
            break;

    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

void APP_EmulateKeyboard(void)
{    
    if(appData.isKeyPressed) {        
        if (appData.isParsingComplete) {            
            if (appData.inputCount > 0) {
                // avoid pressed state
                appData.keyCodeArray.keyCode[0] =  appData.inputCount % 2 ?
                    USB_HID_KEYBOARD_KEYPAD_RESERVED_NO_EVENT_INDICATED : 
                    USB_HID_KEYBOARD_KEYPAD_KEYBOARD_DELETE;
                appData.inputCount--;                
                
            } else if (appData.morseResult.valid) {
                appData.isKeyPressed = false;
                appData.isParsingComplete = false;
                appData.morseResult.valid = false;                
                appData.keyboardModifierKeys.rightShift
                        = appData.morseResult.shiftPressed ? 1 : 0;
                appData.keyboardModifierKeys.rightAlt
                        = appData.morseResult.altPressed ? 1 : 0;
                appData.keyCodeArray.keyCode[0] = appData.morseResult.keycode;
            }
        } else {
            appData.isKeyPressed = false;
            appData.keyboardModifierKeys.modifierkeys = 0;
            appData.keyCodeArray.keyCode[0] = appData.key;
        }
    }
    else
    {
        appData.keyboardModifierKeys.modifierkeys = 0;
         appData.keyCodeArray.keyCode[0] =
                 USB_HID_KEYBOARD_KEYPAD_RESERVED_NO_EVENT_INDICATED;
    }
    
    KEYBOARD_InputReportCreate(&appData.keyCodeArray,
            &appData.keyboardModifierKeys, &keyboardInputReport);

}

void APP_StateReset(void)
{
    appData.isReportReceived = false;
    appData.isReportSentComplete = true;
    appData.key = USB_HID_KEYBOARD_KEYPAD_RESERVED_NO_EVENT_INDICATED;
    appData.keyboardModifierKeys.modifierkeys = 0;
    memset(&keyboardOutputReport.data, 0, 64);
    appData.isKeyPressed = false;        
    appData.keyPressedTime = 0;
    appData.isKeyTyping = false;    
    appData.isParsingComplete = false;
    appData.inputCount = 0;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    
    appData.deviceHandle = USB_DEVICE_HANDLE_INVALID;
    appData.isConfigured = false;

    /* Initialize the key code array */
    appData.key = USB_HID_KEYBOARD_KEYPAD_RESERVED_NO_EVENT_INDICATED;
    appData.keyCodeArray.keyCode[0] = USB_HID_KEYBOARD_KEYPAD_RESERVED_NO_EVENT_INDICATED;
    appData.keyCodeArray.keyCode[1] = USB_HID_KEYBOARD_KEYPAD_RESERVED_NO_EVENT_INDICATED;
    appData.keyCodeArray.keyCode[2] = USB_HID_KEYBOARD_KEYPAD_RESERVED_NO_EVENT_INDICATED;
    appData.keyCodeArray.keyCode[3] = USB_HID_KEYBOARD_KEYPAD_RESERVED_NO_EVENT_INDICATED;
    appData.keyCodeArray.keyCode[4] = USB_HID_KEYBOARD_KEYPAD_RESERVED_NO_EVENT_INDICATED;
    appData.keyCodeArray.keyCode[5] = USB_HID_KEYBOARD_KEYPAD_RESERVED_NO_EVENT_INDICATED;

    /* Initialize the modifier keys */
    appData.keyboardModifierKeys.modifierkeys = 0;

    /* Initialize the led state */
    memset(&keyboardOutputReport.data, 0, 64);

    /* Initialize the switch state */
    appData.isKeyPressed = false;

    /* Initialize the HID instance index.  */
    appData.hidInstance = 0;

    /* Initialize tracking variables */
    appData.isReportReceived = false;
    appData.isReportSentComplete = true;
    
    GPIO_PinInterruptCallbackRegister(INPUT_KEY_PIN, APP_KeyInputHandler, (uintptr_t)NULL);
    GPIO_PinInterruptEnable(INPUT_KEY_PIN);
    
    TMR2_CallbackRegister(APP_TimerHandler,  (uintptr_t)NULL);
    TMR2_InterruptEnable();
    TMR2_Start();
    
    TMR3_Start();    
    
    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "Morse Keyboard\r\n");
    
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{   
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
		    /* Open the device layer */
            appData.deviceHandle = USB_DEVICE_Open( USB_DEVICE_INDEX_0,
                    DRV_IO_INTENT_READWRITE );

            if(appData.deviceHandle != USB_DEVICE_HANDLE_INVALID)
            {
                /* Register a callback with device layer to get event notification (for end point 0) */
                USB_DEVICE_EventHandlerSet(appData.deviceHandle,
                        APP_USBDeviceEventHandler, 0);

                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            }
            else
            {
                /* The Device Layer is not ready to be opened. We should try
                 * again later. */
            }
            break;
        }

        case APP_STATE_WAIT_FOR_CONFIGURATION:

            /* Check if the device is configured. The
             * isConfigured flag is updated in the
             * Device Event Handler */

            if(appData.isConfigured)
            {
                /* Initialize the flag and place a request for a
                 * output report */

                appData.isReportReceived = false;

                USB_DEVICE_HID_ReportReceive(appData.hidInstance,
                        &appData.receiveTransferHandle,
                        (uint8_t *)&keyboardOutputReport,64);

                appData.state = APP_STATE_CHECK_IF_CONFIGURED;
            }

            break;

        case APP_STATE_CHECK_IF_CONFIGURED:

            /* This state is needed because the device can get
             * unconfigured asynchronously. Any application state
             * machine reset should happen within the state machine
             * context only. */

            if(appData.isConfigured)
            {
                appData.state = APP_STATE_CHECK_FOR_OUTPUT_REPORT;
            }
            else
            {
                /* This means the device got de-configured.
                 * We reset the state and the wait for configuration */

                APP_StateReset();
                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            }
            break;

        case APP_STATE_CHECK_FOR_OUTPUT_REPORT:

            if(appData.isReportReceived == true)
            {
                /* Update the LED and schedule and
                 * request */
                appData.isReportReceived = false;
                USB_DEVICE_HID_ReportReceive(appData.hidInstance,
                        &appData.receiveTransferHandle,
                        (uint8_t *)&keyboardOutputReport, 64);
            }

            appData.state = APP_STATE_EMULATE_KEYBOARD;
            break;

        case APP_STATE_EMULATE_KEYBOARD:

            if(appData.isReportSentComplete)
            {
                /* This means report can be sent*/
                APP_EmulateKeyboard();

                appData.isReportSentComplete = false;
                USB_DEVICE_HID_ReportSend(appData.hidInstance,
                    &appData.sendTransferHandle,
                    (uint8_t *)&keyboardInputReport,
                    sizeof(KEYBOARD_INPUT_REPORT));
             }

            appData.state = APP_STATE_CHECK_IF_CONFIGURED;
            break;

        case APP_STATE_ERROR:
            break;

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
