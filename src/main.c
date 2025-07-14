#include "sha256.h"

/* For whirlpool() */
#include <openssl/sha.h>

/* For printf() */
#include <stdio.h>

/* For memcmp() */
#include <string.h>

int main( int argc, char **argv )
{
	if ( argc != 2 )
	{
		printf( "Please, enter only 1 argument\n" );
		return ( 1 );
	}

	unsigned char hash[32];
	unsigned char my_hash[32];

	SHA256( ( const unsigned char * ) argv[1], strlen( argv[1] ), hash );
	printf( "openssl SHA256: " );
	for ( uint8_t i = 0; i < 32; ++i )
		printf( "%02x", hash[i] );
	printf( "\n" );

	sha256( ( const unsigned char * ) argv[1], strlen(argv[1]), my_hash );
	printf( "my SHA256:      " );
	for ( uint8_t i = 0; i < 32; ++i )
		printf( "%02x", my_hash[i] );
	printf("\n");

	if ( !memcmp( ( const char * ) hash, ( const char * ) my_hash, 32 ) )
		printf( "OK\n" );
	else
		printf( "KO\n" );

	return ( 0 );
}
