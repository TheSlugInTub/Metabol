# Metabol

A code generator for C, allowing templates and metaprogramming in C.

# Usage

metabol [input-file] [output-file]

# Tutorial

To mark a templated function or struct, you must include these comments:

```c
// TEMPLATE BEGIN
[code]
// TEMPLATE END
```

to let the parser know where the templated function/struct begins or ends.

This is the syntax to declare a template:

```c
template T, U, L
```

An example templated function would be like this:

```c
template T, U 
float Func(T var1, U var2)
{
    return var1 + var2;
}
```

And a templated struct would look like this:

```c
template T, U
typedef struct Hashmap
{
    T key[100];
    U value[100];
} Hashmap;
```
To call a templated function would be like this:

```c
int hey = 2;
double yo = 4.2;
Func<int, double>(hey, yo);
```
And a templated struct would be like this:

```c
Hashmap<long int, double> hashmap;
```

Templated functions and structs get commented out in the output and get generated as many times as there are unique instantiations of the function/structs.
The `Hashmap` struct from before would look like this in the output if instantiated like the code above:

```c
typedef struct Hashmap_long_int
{
    long int key[100];
    double value[100];
} Hashmap_long_int;
```

And a function would look like this:

```
float Func_int_double(int var1, double var2)
{
    return var1 + var2;
}
```

The identifiers have the types appended to them at the end to make them unique.

The calls also get replaced with something like this:

```
Func_int_double(hey, to);
```

# Building

```bash
git clone https://github.com/TheSlugInTub/Metabol 
cd Metabol 
mkdir bld
cmake -S . -B bld
cmake --build bld/ --config Release
```

You need to have CMake, a C++ compiler and git.
