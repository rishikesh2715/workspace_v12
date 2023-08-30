/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== uartecho.c ========
 */
//#include <stdint.h>
//#include <stddef.h>
//
///* Driver Header files */
//#include <ti/drivers/GPIO.h>
//#include <ti/drivers/UART.h>
//
///* Driver configuration */
//#include "ti_drivers_config.h"
//
///*
// *  ======== mainThread ========
// */
//void *mainThread(void *arg0)
//{
//    char        input;
//    const char  echoPrompt[] = "Echoing characters:\r\n";
//    UART_Handle uart;
//    UART_Params uartParams;
//
//    /* Call driver init functions */
//    GPIO_init();
//    UART_init();
//
//    /* Configure the LED pin */
//    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
//
//    /* Turn on user LED */
//    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
//
//    /* Create a UART with data processing off. */
//    UART_Params_init(&uartParams);
//    uartParams.writeDataMode = UART_DATA_BINARY;
//    uartParams.readDataMode = UART_DATA_BINARY;
//    uartParams.readReturnMode = UART_RETURN_FULL;
//    uartParams.baudRate = 115200;
//
//    uart = UART_open(CONFIG_UART_0, &uartParams);
//
//    if (uart == NULL) {
//        /* UART_open() failed */
//        while (1);
//    }
//
//    UART_write(uart, echoPrompt, sizeof(echoPrompt));
//
//    /* Loop forever echoing */
//    while (1) {
//        UART_read(uart, &input, 1);
//        UART_write(uart, &input, 1);
//    }
//}

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>

/* Driver configuration */
#include "ti_drivers_config.h"

void *mainThread(void *arg0)
{
    char        input;
    const char  echoPrompt[] = "Welcome to MSP432:\r\n";
    UART_Handle uart;
    UART_Params uartParams;

    /* Call driver init functions */
    GPIO_init();
    UART_init();

    /* Configure the LED pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Turn on user LED */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 115200;

    uart = UART_open(CONFIG_UART_0, &uartParams);

    if (uart == NULL) {
        /* UART_open() failed */
        while (1);
    }

    UART_write(uart, echoPrompt, sizeof(echoPrompt));

    const char helpOutput[] = ": Displays all the supported commands\r\n-about: Displays author name, version, and compile date/time\r\n";
    char aboutOutput[80];
    sprintf(aboutOutput, " Rishikesh, Assignment_1, ver_12.4.0.00007, %s, %s\r\n", __TIME__, __DATE__);

    char command[128] = "";
    int i = 0;

    /* Loop forever processing input */
    while (1) {
        UART_read(uart, &input, 1);

        // If the user hits 'Enter', process the command
        if (input == '\r') {
            if (strcmp(command, "-help") == 0) {
                UART_write(uart, helpOutput, strlen(helpOutput));
            } else if (strcmp(command, "-about") == 0) {
                UART_write(uart, aboutOutput, strlen(aboutOutput));
            } else {
                UART_write(uart, command, strlen(command));
            }

            memset(command, 0, sizeof(command));
            i = 0;
            UART_write(uart, "\r\n", 2);
        } else if (input == '\b' && i > 0) { // Handle backspace
            i--;
            command[i] = 0;
            UART_write(uart, " \b", 2);
        } else {
            UART_write(uart, &input, 1);
            if (i < 127) {
                command[i] = input;
                i++;
            }
        }
    }
}

