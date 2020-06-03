/*
 *  The MIT License (MIT)
 *  Copyright (C) 2019  Seeed Technology Co.,Ltd.
 */
#ifndef _ESP_LL_SPI_H_
#define _ESP_LL_SPI_H_

#include <Arduino.h>
#include <esp_config.h>
#include "SPI.h"

int at_spi_begin(void);
int at_spi_write(const uint8_t* buf, uint16_t len, int loop_wait = 50);
int at_spi_read(uint8_t* buf, uint16_t len, int loop_wait = 50);

#endif//_ESP_LL_SPI_H_
