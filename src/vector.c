#include <salamander/vector.h>

// Initialize a new vector with specified element size
smVector* smVector_Create(size_t elemSize, size_t initial_capacity)
{
    smVector* vector = (smVector*)malloc(sizeof(smVector));
    if (vector == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for vector.\n");
        return NULL;
    }

    vector->data = malloc(initial_capacity * elemSize);
    if (vector->data == NULL)
    {
        free(vector);
        fprintf(stderr,
                "Failed to allocate memory for vector data.\n");
        return NULL;
    }

    vector->elemSize = elemSize;
    vector->size = 0;
    vector->capacity = initial_capacity;
    return vector;
}

// Add an element to the vector (copies the element)
int smVector_PushBack(smVector* vector, const void* element)
{
    // Resize if needed
    if (vector->size >= vector->capacity)
    {
        size_t new_capacity = vector->capacity * 2;
        void*  new_data =
            realloc(vector->data, new_capacity * vector->elemSize);
        if (new_data == NULL)
        {
            fprintf(stderr,
                    "Failed to reallocate memory for vector data.\n");
            return -1;
        }

        vector->data = new_data;
        vector->capacity = new_capacity;
    }

    // Calculate position to insert and copy the element
    char* target =
        (char*)vector->data + (vector->size * vector->elemSize);
    memcpy(target, element, vector->elemSize);

    vector->size++;
    return 0;
}

// Get a pointer to an element
void* smVector_Get(smVector* vector, size_t index)
{
    if (index >= vector->size)
    {
        fprintf(stderr, "Index out of bounds.\n");
        return NULL;
    }
    return (char*)vector->data + (index * vector->elemSize);
}

// Remove an element by index (shifts subsequent elements)
int smVector_Remove(smVector* vector, size_t index)
{
    if (index >= vector->size)
    {
        fprintf(stderr, "Index out of bounds.\n");
        return -1;
    }

    // Calculate position to remove
    char* pos_to_remove =
        (char*)vector->data + (index * vector->elemSize);

    // Calculate position of element after the one to remove
    char* next_pos = pos_to_remove + vector->elemSize;

    // Calculate number of bytes to move
    size_t bytes_to_move =
        (vector->size - index - 1) * vector->elemSize;

    // Shift elements
    if (bytes_to_move > 0)
    {
        memmove(pos_to_remove, next_pos, bytes_to_move);
    }

    vector->size--;
    return 0;
}

// Free the memory used by the vector
void smVector_Free(smVector* vector)
{
    free(vector->data);
    free(vector);
}

void smVector_Clear(smVector* vector)
{
    vector->size = 0;
}
