#include <stdint.h>

int split_u32_to_8(uint32_t src, uint8_t *dst, int pos) {
// Split uint32_t to bytes, and insert to dst at pos
	dst[pos++] = (src >> 24) & 0xFF;
	dst[pos++] = (src >> 16) & 0xFF;
	dst[pos++] = (src >> 8 ) & 0xFF;
	dst[pos++] = src & 0xFF;
	return pos;
}

int split_u16_to_8(uint16_t src, uint8_t *dst, int pos) {
// Split uint16_t to bytes, and insert to dst at pos
	dst[pos++] = (src >> 8) & 0xFF;
	dst[pos++] = src & 0xFF;
	return pos;
}

uint32_t merge_u32_from_8(uint8_t *src, int *pos) {
// Merge four 8bit into uint32_t, while updating pos
    uint32_t dst = 0;

    dst |= ( (uint32_t) src[*pos]   << 24);
    dst |= ( (uint32_t) src[*pos+1] << 16);
    dst |= ( (uint32_t) src[*pos+2] << 8);
    dst |= ( (uint32_t) src[*pos+3] << 0);
    *pos += 4;

    return dst;
}

uint16_t merge_u16_from_8(uint8_t *src, int *pos) {
// Merge two 8bit into uint16_t, while updating pos
    uint16_t dst = 0;

    dst |= ( (uint16_t) src[*pos]   << 8);
    dst |= ( (uint16_t) src[*pos+1] << 0);
    *pos += 2;

    return dst;
}

