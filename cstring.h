#ifndef CSTRING_H_
#define CSTRING_H_

#ifdef __cpluscplus
    #include <string>
    #undef NULL
    #define NULL nullptr
#else
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>

    #define S_LENGTH   unsigned long long
    #define init_str() atexit(init)

    #define len(str_t)   strlen((str_t)->string)
    #define str(str_t)          ((str_t)->string)
    #define str_cmp(str1,op,str2)  (strcmp((str1).string,(str2).string) op 0)
    #define turn_str(c_str)     ((struct string_t){c_str,strlen(c_str)})

    #ifdef ENABLE_REPLACE
        typedef struct string_t * cstring;
    #endif

    struct string_t { char *string; S_LENGTH length; };

    struct string_t * new_empty_s( void );
    struct string_t * new_num_s  ( S_LENGTH length );
    struct string_t * new_str    ( const char * src );

    void             add_str    ( struct string_t  *str, struct string_t src );
    void             copy_str   ( struct string_t  *str, struct string_t src );

    char             get_item   ( struct string_t  str, long long index );
    void             set_item   ( struct string_t  *str, long long index, char c );
    struct string_t *get_sub_str( struct string_t  str, long long start, long long end );

    void             delete_str ( struct string_t *str );


    int              io_getstr  ( struct string_t *str );   

#endif

#endif