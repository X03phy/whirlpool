#include "whirlpool.h"

static void whirlpool_init( t_whirlpool_ctx *ctx )
{
	memset( ctx->buffer, 0, sizeof( ctx->buffer ) );
	memset( ctx->state, 0, sizeof( ctx->state ) );

	ctx->bitlen = 0;
	ctx->buffer_len = 0;
}

static void whirlpool_transform( t_whirlpool_ctx *ctx )
{
	uint32_t W[64];
	uint32_t a, b, c, d, e, f, g, h;
	uint32_t t1, t2;
	uint8_t i;

	for ( i = 0; i < 16; ++i )
		W[i] = ( ctx->buffer[i * 4] << 24 ) | ( ctx->buffer[i * 4 + 1] << 16 ) |
			   ( ctx->buffer[i * 4 + 2] << 8 ) | ( ctx->buffer[i * 4 + 3] );

	for ( ; i < 64; ++i )
		W[i] = SIG1( W[i - 2] ) + W[i - 7] + SIG0( W[i - 15] ) + W[i - 16];

	a = ctx->state[0]; b = ctx->state[1]; c = ctx->state[2]; d = ctx->state[3];
	e = ctx->state[4]; f = ctx->state[5]; g = ctx->state[6]; h = ctx->state[7];

	for ( i = 0; i < 64; ++i )
	{
		t1 = h + EP1( e ) + CH( e, f, g ) + K[i] + W[i];
		t2 = EP0( a ) + MAJ( a, b, c );
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
	ctx->state[5] += f;
	ctx->state[6] += g;
	ctx->state[7] += h;
}

static void whirlpool_update( t_whirlpool_ctx *ctx, const uint8_t *data, const size_t len )
{
	for ( size_t i = 0; i < len; ++i )
	{
		ctx->buffer[ctx->buffer_len++] = data[i];
		ctx->bitlen += 8;

		if ( ctx->buffer_len == 64 )
		{
			whirlpool_transform( ctx );
			ctx->buffer_len = 0;
		}
	}
}

static void whirlpool_final( uint8_t hash[32], t_whirlpool_ctx *ctx )
{
	ctx->buffer[ctx->buffer_len++] = 0x80;

	if ( ctx->buffer_len > 56 )
	{
		while ( ctx->buffer_len < 64 )
			ctx->buffer[ctx->buffer_len++] = 0x00;
		whirlpool_transform( ctx );
		ctx->buffer_len = 0;
	}

	while ( ctx->buffer_len < 56 )
		ctx->buffer[ctx->buffer_len++] = 0x00;

	ctx->buffer[ctx->buffer_len++] = ( ctx->bitlen >> 56 ) & 0xff;
	ctx->buffer[ctx->buffer_len++] = ( ctx->bitlen >> 48 ) & 0xff;
	ctx->buffer[ctx->buffer_len++] = ( ctx->bitlen >> 40 ) & 0xff;
	ctx->buffer[ctx->buffer_len++] = ( ctx->bitlen >> 32 ) & 0xff;
	ctx->buffer[ctx->buffer_len++] = ( ctx->bitlen >> 24 ) & 0xff;
	ctx->buffer[ctx->buffer_len++] = ( ctx->bitlen >> 16 ) & 0xff;
	ctx->buffer[ctx->buffer_len++] = ( ctx->bitlen >> 8 ) & 0xff;
	ctx->buffer[ctx->buffer_len++] = ctx->bitlen & 0xff;

	whirlpool_transform( ctx );

	for ( uint8_t i = 0; i < 8; ++i )
	{
		hash[i * 4] = ( ctx->state[i] >> 24 ) & 0xff;
		hash[i * 4 + 1] = ( ctx->state[i] >> 16 ) & 0xff;
		hash[i * 4 + 2] = ( ctx->state[i] >> 8 ) & 0xff;
		hash[i * 4 + 3] = ( ctx->state[i] ) & 0xff;
	}
}

void whirlpool( const uint8_t *data, size_t len, uint8_t hash[32] )
{
	t_whirlpool_ctx ctx;

	whirlpool_init( &ctx );
	whirlpool_update( &ctx, data, len );
	whirlpool_final( hash, &ctx );
}
