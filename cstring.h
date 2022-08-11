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
    #include <stddef.h>
    #include <locale.h>
    #include <wchar.h>
    #include <wctype.h>

    #define ENABLE_REPLACE

    #define S_LENGTH   unsigned long long
    #define init_str() atexit(init)

    #define len(str_t)   ((str_t)->length)
    #define str(str_t)          ((str_t)->string)
    #define str_cmp(str1,op,str2)  (wcscmp((str1).string,(str2).string) op 0)
    #define turn_str(c_str)     &((struct string_t){c_str,wcslen(c_str)})


    #ifdef ENABLE_REPLACE
        typedef struct string_t * cstring;
    #endif

    struct string_t { wchar_t *string; S_LENGTH length; };

    struct string_t * new_empty_s( void );
    struct string_t * new_num_s  ( S_LENGTH length );
    struct string_t * new_str    (   const wchar_t * src );
    struct string_t * new_str_cpy( struct string_t *src );

    void             add_str    ( struct string_t  *str, struct string_t *src );
    void             copy_str   ( struct string_t  *str, struct string_t *src );

    wchar_t          get_item   ( struct string_t  *str, long long index );
    void             set_item   ( struct string_t  *str, long long index, wchar_t c );
    struct string_t *get_sub_str( struct string_t  *str, long long start, long long end );

    void             delete_str ( struct string_t *str );


    int              io_getstr  ( struct string_t *str );   
    void             print_err  ( void );
    int              is_err     ( void );

#endif

#endif