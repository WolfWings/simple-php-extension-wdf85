#include <stdint.h>
#include <php.h>
#include <standard/info.h>
#include <SAPI.h>

static const char wdf85_table[85] = "!w#$x&y()*+z-{/0123456789:|<}>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[~]^_abcdefghijklmnopqrstuv";

static uint8_t decode[256];

PHP_MINIT_FUNCTION( wdf85_init ) {
	for ( int i = 0; i < 256; i++ ) {
		decode[i] = 127;
	}
	for ( int i = 0; i < 85; i++ ) {
		decode[(int)wdf85_table[i]] = i;
	}
	return SUCCESS;
}

static char block_encode[5];

static void block_encoder( uint32_t buffer ) {
	block_encode[ 4 ] = wdf85_table[ buffer % 85 ]; buffer /= 85;
	block_encode[ 3 ] = wdf85_table[ buffer % 85 ]; buffer /= 85;
	block_encode[ 2 ] = wdf85_table[ buffer % 85 ]; buffer /= 85;
	block_encode[ 1 ] = wdf85_table[ buffer % 85 ];
	block_encode[ 0 ] = wdf85_table[ buffer / 85 ];
}

PHP_FUNCTION( wdf85_encode ) {
	zend_string *input;
	zend_string *output;
	uint32_t buffer = 0;
	int chunks = 0;
	int oloc = 0;

	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_STR( input )
	ZEND_PARSE_PARAMETERS_END();

	output = zend_string_alloc( ZSTR_LEN( input ) + ( ( ZSTR_LEN( input ) + 3 ) >> 2 ), 0 );

	for ( size_t i = 0; i < ZSTR_LEN( input ); i++ ) {
		buffer <<= 8;
		buffer += ( (int) ZSTR_VAL( input )[ i ] ) & 255;
		chunks++;
		if ( chunks == 4 ) {
			chunks = 0;
			block_encoder( buffer );
			buffer = 0;
			ZSTR_VAL( output )[ oloc + 0 ] = block_encode[ 0 ];
			ZSTR_VAL( output )[ oloc + 1 ] = block_encode[ 1 ];
			ZSTR_VAL( output )[ oloc + 2 ] = block_encode[ 2 ];
			ZSTR_VAL( output )[ oloc + 3 ] = block_encode[ 3 ];
			ZSTR_VAL( output )[ oloc + 4 ] = block_encode[ 4 ];
			oloc += 5;
		}
	}

	if ( chunks > 0 ) {
		buffer = buffer << ( ( 4 - chunks ) * 8 );

		block_encoder( buffer );

		while ( chunks > 0 ) {
			ZSTR_VAL( output )[ oloc + chunks ] = block_encode[ chunks ];
			chunks--;
		}
		ZSTR_VAL( output )[ oloc + chunks ] = block_encode[ chunks ];
	}

	ZSTR_VAL( output )[ ZSTR_LEN( output ) ] = '\0';

	RETVAL_STR( output );
}

static char block_decode[4];

static void block_decoder( uint32_t buffer ) {
	block_decode[ 3 ] = buffer;
	block_decode[ 2 ] = buffer >>  8;
	block_decode[ 1 ] = buffer >> 16;
	block_decode[ 0 ] = buffer >> 24;
}

PHP_FUNCTION( wdf85_decode ) {
	zend_string *input;
	zend_string *output;
	uint32_t buffer = 0;
	uint32_t onechar;
	int chunks = 0;
	int oloc = 0;

	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_STR( input )
	ZEND_PARSE_PARAMETERS_END();

	if ( ( ZSTR_LEN( input ) % 5 ) == 1 ) {
		RETURN_NULL()
	}

	output = zend_string_alloc( ZSTR_LEN( input ) - ( ( ZSTR_LEN( input ) + 4 ) / 5 ), 0 );

	for ( size_t i = 0; i < ZSTR_LEN( input ); i++ ) {
		onechar = decode[ ( (int) ZSTR_VAL( input )[ i ] ) & 255 ];
		if ( onechar == 127 ) {
			zend_string_release_ex( output, 0 );
			RETURN_NULL()
		}

		if ( chunks == 4 ) {
			chunks = 0;
			if ( buffer > ( UINT32_MAX / 85u ) ) {
				zend_string_release_ex( output, 0 );
				RETURN_NULL()
			}
			buffer = buffer * 85u;
			if ( buffer > ( UINT32_MAX - onechar ) ) {
				zend_string_release_ex( output, 0 );
				RETURN_NULL()
			}
			buffer += onechar;
			block_decoder( buffer );
			buffer = 0;
			ZSTR_VAL( output )[ oloc + 0 ] = block_decode[ 0 ];
			ZSTR_VAL( output )[ oloc + 1 ] = block_decode[ 1 ];
			ZSTR_VAL( output )[ oloc + 2 ] = block_decode[ 2 ];
			ZSTR_VAL( output )[ oloc + 3 ] = block_decode[ 3 ];
			oloc += 4;
		} else {
			chunks++;
			buffer = buffer * 85u;
			buffer += onechar;
		}
	}

	if ( chunks > 0 ) {
		switch( chunks ) {
		case 2:
			buffer *= 85u;
			buffer += 84u;
		case 3:
			buffer *= 85u;
			buffer += 84u;
		case 4:
			if ( buffer > ( UINT32_MAX / 85u ) ) {
				zend_string_release_ex( output, 0 );
				RETURN_NULL()
			}
			buffer *= 85u;
			if ( buffer > ( UINT32_MAX - 84u ) ) {
				zend_string_release_ex( output, 0 );
				RETURN_NULL()
			}
			buffer += 84u;
		}

		block_decoder( buffer );

		while ( chunks > 0 ) {
			chunks--;
			ZSTR_VAL( output )[ oloc + chunks ] = block_decode[ chunks ];
		}
	}

	ZSTR_VAL( output )[ ZSTR_LEN( output ) ] = '\0';

	RETVAL_STR( output );
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX( arginfo_string_from_single_string, 0, 1, IS_STRING, 1 )
	ZEND_ARG_TYPE_INFO( 0, string, IS_STRING, 0 )
ZEND_END_ARG_INFO()

static const zend_function_entry wdf85_functions[] = {
	PHP_FE( wdf85_encode, arginfo_string_from_single_string )
	PHP_FE( wdf85_decode, arginfo_string_from_single_string )
	PHP_FE_END
};

PHP_MINFO_FUNCTION( wdf85 ) {
	php_info_print_table_start();
	php_info_print_table_colspan_header( 2, "wdf85" );
	php_info_print_table_row( 2, "Enabled", "yes" );
	php_info_print_table_end();
}

zend_module_entry wdf85_module_entry = {
	STANDARD_MODULE_HEADER
,	"wdf85"
,	wdf85_functions
,	PHP_MINIT( wdf85_init )
,	NULL
,	NULL
,	NULL
,	PHP_MINFO( wdf85 )
,	"0.0.1"
,	STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE( wdf85 )
