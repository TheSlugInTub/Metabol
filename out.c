#include <stdio.h>

// // TEMPLATE BEGIN
// template T, U
// float DoStuff(T var, U var1)
// {
//     return var + var1;
// }
// // TEMPLATE END

// Generated specialized functions for DoStuff
float DoStuff_int_float(int var, float var1)
{
    return var + var1;
}


// // TEMPLATE BEGIN
// template T, U 
// typedef struct Hashmap
// {
//     T key[100];
//     U value[100];
// } Hashmap;
// // TEMPLATE END

// Generated specialized structs for Hashmap
typedef struct Hashmap_long_double_char
{
    long double key[100];
    char value[100];
} Hashmap_long_double_char;


int main(int argc, char** argv)
{
    int bruh = 2;
    float a = 2.14123f;
    float res = DoStuff_int_float(bruh, a);
    printf("Result: %f\n", res);

    Hashmap_long_double_char map;
}
