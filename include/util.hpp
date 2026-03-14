/* VerdantAbode
 * util.hpp
 *
 * Tuomo Björk
*/
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <hardware/i2c.h>

#include "pico/stdlib.h"
#include "FreeRTOS.h"

#include "system.hpp"


// This file, ideally, shouldn't exist. Instead its contents
// should be integrated into their own sensible definition/implementations.
//
// It effectively exists for quick testing, hacks, and small utility funcs.
//
void init_i2c0(void);
void init_i2c1(void);

bool eeprom_save(SYSTEM::DATA* ctx);
bool eeprom_load(SYSTEM::DATA* ctx);

bool eeprom_read(uint16_t address, uint8_t *buffer, int count);
bool eeprom_write(uint16_t address, uint8_t *buffer, int count);

uint16_t crc16(const uint8_t *pData, size_t len);

int split_u32_to_8(uint32_t src, uint8_t *dst, int pos);
int split_u16_to_8(uint16_t src, uint8_t *dst, int pos);
uint32_t merge_u32_from_8(uint8_t *src, int *pos);
uint16_t merge_u16_from_8(uint8_t *src, int *pos);
