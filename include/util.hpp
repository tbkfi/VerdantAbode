#pragma once

#include <stdint.h>

int split_u32_to_8(uint32_t src, uint8_t *dst, int pos);
int split_u16_to_8(uint16_t src, uint8_t *dst, int pos);
uint32_t merge_u32_from_8(uint8_t *src, int *pos);
uint16_t merge_u16_from_8(uint8_t *src, int *pos);
