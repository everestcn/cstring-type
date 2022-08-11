#include "cstring.h"

#define DEBUG
#define WARNING

#define MAX_POOL 6  //2 ^ 6
#define MAX_BIN  2 << (MAX_POOL - 1)  //2 ^ 6
#define MEMORY_MAX 50

static size_t            memory_size = 0;
static unsigned int      err_num     = 0;
static struct string_t **memory_pool = NULL;



inline static size_t get_memory( size_t m ) {
    return ( m % MAX_POOL ) ? ( ( m >> MAX_POOL ) + 1 ) << MAX_POOL : m;
}

static void init( void ) {
    int pos = 0;

    for ( register struct string_t **i = memory_pool; pos < memory_size; pos++ ) {
        if ( *( i + pos ) ) delete_str( *( i + pos ) );
        if ( err_num )      print_err();


    }

    if ( memory_pool )
        free( memory_pool );

};

inline static void check_memory( void ) {
    if ( !memory_pool ) {
        setlocale(LC_ALL, "");
        memory_pool = ( struct string_t ** )calloc( get_memory( memory_size + 1 ), sizeof( struct string_t ) );
        atexit( init );
    }
    else if ( get_memory( memory_size ) != get_memory( memory_size + 1 ) ) {
        memory_pool = ( struct string_t ** )realloc( memory_pool, get_memory( memory_size + 1 ) * sizeof( struct string_t ) );
    }
}


//private function, may delete

static void new_empty_st( struct string_t *str ) {
    check_memory();

    memory_pool[ memory_size++ ] = str;

    str -> string = NULL; str -> length = 0;
    //memory_pool
}


static void new_num_st  ( struct string_t *str, S_LENGTH length ) {
    check_memory();

    memory_pool[ memory_size++ ] = str;

    str -> string = ( wchar_t * )calloc( ( size_t )length, sizeof( wchar_t ) );
    str -> length = length;

}


static void new_str_t    ( struct string_t *str, wchar_t * src ) {
    check_memory();

    memory_pool[ memory_size++ ] = str;

    //pre
    size_t length = wcslen( src );
    
    int memory = get_memory( length );

    wchar_t *n_str = ( wchar_t * )calloc( memory, sizeof( wchar_t ) );
    wcscat( n_str, src );

    str -> string = n_str;
    str -> length = length;

}

//public functions

struct string_t *new_empty_s( void ) {
    check_memory();
    return memory_pool[ memory_size++ ] = ( struct string_t * )calloc( 1, sizeof( struct string_t ) );
}

struct string_t *new_str( const wchar_t * str ) {
    check_memory();

    size_t length   = wcslen( str ) + 1;
    size_t memory_u = get_memory( length );

    struct string_t *temp_s = ( struct string_t * )calloc( 1, sizeof( struct string_t ) );
                     temp_s -> string = ( wchar_t * )calloc( memory_u, sizeof( wchar_t ) );

    wcscpy( temp_s -> string, str );
    temp_s -> length = length - 1;

    return memory_pool[ memory_size++ ] = temp_s;

}

struct string_t *new_str_cpy( struct string_t *src ) {
    //check_memory();
    return new_str( str( src ) );
}

struct string_t *new_num_s( S_LENGTH length ) {
    check_memory();

    struct string_t *temp_s = ( struct string_t * )calloc( 1, sizeof( struct string_t ) );
    temp_s -> string = ( wchar_t * )calloc( get_memory( length ), sizeof( wchar_t ) );
    temp_s -> length = length;

    return memory_pool[ memory_size++ ] = temp_s;

}

void  add_str    ( struct string_t  *str, struct string_t *src ) {
    str -> string = ( wchar_t * )realloc( str -> string, get_memory( str -> length + src -> length + 1 ) * sizeof( wchar_t ) );
    wcscat( str -> string, src -> string );

    str -> length += src -> length;
    str -> string[ str -> length ] = '\0';
}

wchar_t get_item   ( struct string_t  *str, long long index ) {
    if ( index < str -> length || -index < str -> length )
        return ( index >= 0 ) ? str -> string[ index ] : str -> string[ str -> length + index ];

    return '\0';
}

void  set_item   ( struct string_t  *str, long long index, wchar_t c ) {
    if ( index < str -> length || -index < str -> length )
        ( index >= 0 ) ? ( str -> string[ index ] = c ) : ( str -> string[ str -> length + index ] = c );
    return ;
}

void  copy_str   ( struct string_t  *str, struct string_t *src ) {
    if ( NULL != str -> string ) free( str -> string ); 
    str -> string = ( wchar_t * )calloc( get_memory( src -> length ), sizeof( wchar_t ) );
    wcscpy( str -> string, src -> string );
    
    //str -> string = src.string;

}


struct string_t *get_sub_str( struct string_t  *str, long long start, long long end ) {
    wchar_t *sub_str = ( wchar_t * )calloc( get_memory( end - start ), sizeof( wchar_t ) );
    wchar_t *c = str -> string + start;

    for ( register int i = start; i < end && i < str -> length; i++ ) {
        sub_str[ i - start ] = *(c + i);
    }

    //struct string_t sub_str_obj;

    struct string_t *sub_str_obj = new_str( sub_str );

    //new_str_t( &sub_str_obj, sub_str );

    free( sub_str );

    return sub_str_obj;
}

void  delete_str ( struct string_t *str ) {
    #ifdef DEBUG
        wprintf( L"delete object %-30ls%10p\n", str ? str ->string : L"(null)", str );
        //wprintf( L"delete object %-10ls %10p\n", (*( i + pos )) ? ( (*(*( i + pos ))).string ) : "(null)", *( i + pos ) );
    #endif

    for ( register size_t i = 0; i < memory_size; i++ ) {
        if ( str == *(memory_pool + i) ) {
            if ( str -> string ) {
                if ( i == memory_size - 1 ) memory_size--;
                free( str -> string );
                free( *( memory_pool + i ) );

                *( memory_pool + i ) = NULL;
                
                return ;
            }

            #ifdef WARNING
                else {
                    printf( "Warning:Try to delete a empty string object.\n" );

                    if ( i == memory_size - 1 ) memory_size--;
                    free( *( memory_pool + i ) );

                    *( memory_pool + i ) = NULL;

                    return ;
                }
            #endif
        }
    }

    err_num = 1;
}

int io_getstr( struct string_t *str ) {
    int ch, length = 0;

    wchar_t *io_str = ( wchar_t * )calloc( 1 << MAX_POOL, sizeof( wchar_t ) );
    
    while ( ( ch = getwchar() ) != L'\n' ) {
        if ( length <= get_memory( ( size_t )length ) ) io_str[ length++ ] = ch;
        else {
            io_str = ( wchar_t * )realloc( io_str, get_memory( length ) * sizeof( wchar_t ) );
            io_str[ length++ ] = ch;
        }
    }

    copy_str( str, &(( struct string_t ){ io_str, wcslen( io_str ) }) );

    return 0;

}

void print_err( void ) {
    if ( err_num )
        puts( (char *[]){ "(null)", "Try To Delete A Null Object." }[ err_num ] );
    err_num = 0;
}