#ifndef WHIRLPOOL_H
#define WHIRLPOOL_H

/* For uintX_t */
#include <stdint.h>

/* For size_t */
#include <stddef.h>

typedef struct s_whirlpool_ctx
{
	uint32_t state[8];  // H0 to H7
	uint64_t bitlen;    // Total length in bits
	uint8_t buffer[64]; // 512 bits (64 bytes) buffer
	size_t buffer_len;  // Actual length of the buffer
} t_whirlpool_ctx;

void whirlpool( const uint8_t *data, size_t len, uint8_t hash[32] );

#endif
