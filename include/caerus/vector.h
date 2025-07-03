#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    void** data; // Array of void pointers to store any type
    size_t elemSize;
    size_t size;     // Current number of elements in the vector
    size_t capacity; // Amount of data allocated for the vector
} smVector;

// Initialize the vector
smVector* smVector_Create(size_t elemSize, size_t initial_capacity);

// Push an element into a vector
int smVector_PushBack(smVector* vector, const void* element);

// Get the data at an index of the vector
void* smVector_Get(smVector* vector, size_t index);

// Remove an element from the vector by index
int smVector_Remove(smVector* vector, size_t index);

// Free the memory used by the vector
void smVector_Free(smVector* vector);

// Set the size to zero, the memory remains allocated for future use
void smVector_Clear(smVector* vector);
