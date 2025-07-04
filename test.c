#include <stdio.h>

// TEMPLATE BEGIN
template T, U
float DoStuff(T var, U var1)
{
    return var + var1;
}
// TEMPLATE END

// TEMPLATE BEGIN
template T, U 
typedef struct Hashmap
{
    T key[100];
    U value[100];
} Hashmap;
// TEMPLATE END

int main(int argc, char** argv)
{
    int bruh = 2;
    float a = 2.14123f;
    float res = DoStuff<int, float>(bruh, a);
    printf("Result: %f\n", res);

    Hashmap<long double, char> map;
}
