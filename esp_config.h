/**
 * \file            esp_config.h
 * \brief           Configuration for ESP
 */

/*
 * Copyright (c) 2019 Tilen MAJERLE
 *  
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of ESP-AT library.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 * Version:         $_version_$
 */
#ifndef ESP_HDR_CONFIG_H
#define ESP_HDR_CONFIG_H

/* User specific config */
#define ESP_CFG_AT_ECHO                     1
#define ESP_CFG_SYS_PORT                    ESP_SYS_PORT_CMSIS_OS
#define ESP_CFG_INPUT_USE_PROCESS           1

#define ESP_CFG_PING                        1

#define ESP_CFG_NETCONN                     1

#define ESP_CFG_DNS                         1

#define ESP_CFG_RESET_ON_INIT               0
#define ESP_CFG_RESTORE_ON_INIT             1

#define ESP_CFG_SMART                       1

#define ESP_CFG_WPS                         1

#define ESP_CFG_MDNS                        1

#define ESP_CFG_SNTP                        1

#define ESP_CFG_DBG                         ESP_DBG_ON
#define ESP_CFG_DBG_OUT(fmt, ...)           do { xprintf(fmt, ## __VA_ARGS__); } while (0)
extern
#ifdef __cplusplus
"C" 
#endif
int xprintf( const char * format, ... );

#ifndef __cplusplus
// #define printf                              xprintf
#endif
// The log console port
// #define Serial Serial1

// Communication port selection between UART/SPI.
#define ESP_CFG_USE_SPI                     1

// esp communication UART port
#define ARDUINO_SERIAL_TO_ESP32AT RTL8720D //Serial2
// esp communication SPI  port
#define ARDUINO_SPI_TO_ESPAT      RTL8720D_SPI
#define ARDUINO_SPI_PIN_CS        RTL8720D_SS_PIN
#define ARDUINO_SPI_PIN_SYNC      RTL8720D_GPIO0



/* Include default configuration setup */
#include "esp/esp_config_default.h"

#endif /* ESP_HDR_CONFIG_H */
