#include "cstring.h"

#define MAX_POOL 6  //2 ^ 6
#define MAX_BIN  2 << (MAX_POOL - 1)  //2 ^ 6
#define MEMORY_MAX 50

static size_t            memory_size = 0;
static struct string_t **memory_pool = NULL;

inline static size_t get_memory( size_t m ) {
    return ( m % MAX_POOL ) ? ( ( m >> MAX_POOL ) + 1 ) << MAX_POOL : m;
}

static void init( void ) {
    int pos = 0;

    for ( struct string_t **i = memory_pool; pos < memory_size; pos++ ) {
        //#ifdef DEBUG
            printf( "delete object %-10s %10p\n", (*( i + pos )) ? ( (**( i + pos )).string ) : ("(null)"), *( i + pos ) );
        //#endif
        if ( *( i + pos ) ) delete_str( *( i + pos ) );
        *( i + pos ) = NULL;
    }

    if ( memory_pool )
        free( memory_pool );

};

inline static void check_memory( void ) {
    if ( !memory_pool ) {
        memory_pool = ( struct string_t ** )calloc( get_memory( memory_size + 1 ), sizeof( struct string_t ) );
        atexit( init );
    }
    else if ( get_memory( memory_size ) != get_memory( memory_size + 1 ) ) {
        memory_pool = ( struct string_t ** )realloc( memory_pool, get_memory( memory_size + 1 ) * sizeof( struct string_t ) );
    }
}

static void new_empty_st( struct string_t *str ) {
    check_memory();

    memory_pool[ memory_size++ ] = str;

    str -> string = NULL; str -> length = 0;
    //memory_pool
}


static void new_num_st  ( struct string_t *str, S_LENGTH length ) {
    check_memory();

    memory_pool[ memory_size++ ] = str;

    str -> string = ( char * )calloc( ( size_t )length, sizeof( char ) );
    str -> length = length;

}


static void new_str_t    ( struct string_t *str, char * src ) {
    check_memory();

    memory_pool[ memory_size++ ] = str;

    //pre
    size_t length = strlen( src );
    
    int memory = get_memory( length );

    char *n_str = ( char * )calloc( memory, sizeof( char ) );
    strcat( n_str, src );

    str -> string = n_str;
    str -> length = length;

}

struct string_t *new_empty_s( void ) {
    check_memory();
    return memory_pool[ memory_size++ ] = ( struct string_t * )calloc( 1, sizeof( struct string_t ) );
}

struct string_t *new_str( const char * str ) {
    check_memory();

    size_t length   = strlen( str );
    size_t memory_u = get_memory( length );

    struct string_t *temp_s = ( struct string_t * )calloc( 1, sizeof( struct string_t ) );
                     temp_s -> string = ( char * )calloc( memory_u, sizeof( char ) );

    strcpy( temp_s -> string, str );
    temp_s -> length = length;

    return memory_pool[ memory_size++ ] = temp_s;

}

struct string_t *new_num_s( S_LENGTH length ) {
    check_memory();

    struct string_t *temp_s = ( struct string_t * )calloc( 1, sizeof( struct string_t ) );
    temp_s -> string = ( char * )calloc( length, sizeof( char ) );
    temp_s -> length = length;

    return memory_pool[ memory_size++ ] = temp_s;

}

void  add_str    ( struct string_t  *str, struct string_t src ) {
    str -> string = ( char * )realloc( str -> string, get_memory( str -> length + src.length + 1 ) );
    strcat( str -> string, src.string );

    str -> length += src.length + 1;
    str -> string[ str -> length ] = '\0';
}

char  get_item   ( struct string_t  str, long long index ) {
    if ( index < str.length || -index < str.length )
        return ( index >= 0 ) ? str.string[ index ] : str.string[ str.length + index ];

    return '\0';
}

void  set_item   ( struct string_t  *str, long long index, char c ) {
    if ( index < str -> length || -index < str -> length )
        ( index >= 0 ) ? ( str -> string[ index ] = c ) : ( str -> string[ str -> length + index ] = c );
    return ;
}

void  copy_str   ( struct string_t  *str, struct string_t src ) {
    if ( NULL != str -> string ) free( str -> string ); 
    str -> string = ( char * )calloc( get_memory( src.length ), sizeof( char ) );
    strcpy( str -> string, src.string );
    //str -> string = src.string;

}


struct string_t *get_sub_str( struct string_t  str, long long start, long long end ) {
    char *sub_str = ( char * )calloc( get_memory( end - start ), sizeof( char ) );
    char *c = str.string + start;

    for ( int i = start; i < end && i < str.length; i++ ) {
        sub_str[ i - start ] = *(c + i);
    }

    //struct string_t sub_str_obj;

    struct string_t *sub_str_obj = new_str( sub_str );

    //new_str_t( &sub_str_obj, sub_str );

    free( sub_str );

    return sub_str_obj;
}

void  delete_str ( struct string_t *str ) {
    if ( str -> string ) {
        for ( int i = 0; i < memory_size; i++ ) {
            if ( str == *(memory_pool + i) ) {
                free( str -> string ); break;
            }
        }
        
        str -> string = NULL;
        str -> length = 0;
    }
    #ifdef WARNING
        else {
            printf( "Warning:Try to delete a empty string object.\n" );
        }
    #endif

    free( str ); str = NULL;
}

int io_getstr( struct string_t *str ) {
    int ch, length = 0;

    char *io_str = ( char * )calloc( 1 << MAX_POOL, sizeof( char ) );
    
    while ( ( ch = getchar() ) != '\n' ) {
        if ( length <= get_memory( ( size_t )length ) ) io_str[ length++ ] = ch;
        else {
            io_str = ( char * )realloc( io_str, get_memory( length ) );
            io_str[ length++ ] = ch;
        }
    }

    copy_str( str, ( struct string_t ){ io_str, strlen( io_str ) } );

    return 0;

}