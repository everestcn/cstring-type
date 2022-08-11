#include <stdio.h>
#include <assert.h>
#include "cstring/cstring.h"

#define AUTO_DELETE //delete it if you want to delete cstring object by yourself

cstring my_cstring;

void test_str_equ( cstring add_str_v ) {
    assert( str_cmp( *my_cstring, ==, *add_str_v ) );
}

void test_str_copy( void ) {
    cstring add_str_v = new_str_cpy( my_cstring );
    
    assert( add_str_v != my_cstring );
    assert( str_cmp( *add_str_v, ==, *my_cstring ) );
    assert( len( my_cstring ) == len( add_str_v ) );
    
    delete_str( add_str_v );

}

void test_str_add( void ) {
    add_str( my_cstring, turn_str( L"又一次！" ) );

    assert( str_cmp( *my_cstring, ==, *turn_str( L"你好，世界！又一次！" ) ) );

    delete_str( my_cstring );  my_cstring = new_str( L"你好，世界！" );

}

void test_get_length( void ) {
    assert( len( my_cstring ) == wcslen( L"你好，世界！" ) );
}

void test_get_str( void ) {
    assert( str_cmp( *my_cstring, ==, *turn_str( L"你好，世界！" ) ) );
}

void test_create_str( void ) {
    cstring str1 = new_empty_s();
    cstring str2 = new_num_s( 5 );
    cstring str3 = new_str( L"你好" );

    assert( len( str1 ) == 0 );
    assert( len( str2 ) == 5 );
    assert( str_cmp( *str3, ==, *turn_str( L"你好" ) ) );

    delete_str( str1 );
    delete_str( str2 );
    delete_str( str3 );


}

void test_delete_str( void ) {
    cstring str = new_num_s( 500 );

    delete_str( str );


}

void test_get_item( void ) {
    assert( str( my_cstring )[ 1 ] == L"你好，世界！"[ 1 ] );    //method 1
    assert( get_item( my_cstring, 1 ) == L"你好，世界！"[ 1 ] ); //method 2
    assert( get_item( my_cstring, -2 ) == L"你好，世界！"[ 4 ] );
}

void test_get_substr( void ) {
    assert( str_cmp( *get_sub_str( my_cstring, 0, 4 ), ==, *turn_str( L"你好，世" ) ) );
}

void test_change_item( void ) {
    set_item( my_cstring, 5, L'。' );
    assert( str_cmp( *my_cstring, ==, *turn_str( L"你好，世界。" ) ) );

    delete_str( my_cstring ); my_cstring = new_str( L"你好，世界！" );
}

int main( void ) {
    my_cstring = new_str( L"你好，世界！" );

    test_change_item();
    test_create_str();
    test_delete_str();
    test_get_item();
    test_get_length();
    test_get_str();
    test_get_substr();
    test_str_add();
    test_str_copy();
    test_str_equ( turn_str( L"你好，世界！" ) );

    return 0;

}