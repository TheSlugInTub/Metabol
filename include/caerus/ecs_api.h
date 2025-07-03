#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

// Opaque pointer types
typedef struct csScene_t *csSceneHandle;
typedef unsigned long long csEntityID;

// Constants
#define CS_ECS_MAX_COMPONENTS 200
#define CS_ECS_MAX_ENTITIES 1000
#define CS_ECS_INVALID_ENTITY ((csEntityID) - 1)

// Scene management
csSceneHandle csECS_CreateScene(void);
void csECS_DestroyScene(csSceneHandle scene);
void csECS_ClearScene(csSceneHandle scene);
int csECS_EntityCount(csSceneHandle);

// Entity management
csEntityID csECS_AddEntity(csSceneHandle scene);
void csECS_DestroyEntity(csSceneHandle scene, csEntityID entity);
csEntityID csECS_CloneEntity(csSceneHandle scene, csEntityID sourceEntity);
bool csECS_IsEntityValid(csEntityID entity);
csEntityID csECS_GetEntityAtIndex(csSceneHandle scene, int index);

// Component management
// We now automatically register components by size and hash
typedef size_t csComponentTypeID;

// Get a component type ID from a component type name (using string
// hashing) This is a macro that generates a unique ID for each
// component type at compile time
#define CS_ECS_COMPONENT_TYPE(component_type)                                  \
  ((csComponentTypeID)((sizeof(component_type) << 16) |                        \
                       (csECS_HashString(#component_type) & 0xFFFF)))

#define CS_ECS_COMPONENT_TYPE_N(component_name, size)                          \
  ((csComponentTypeID)((size << 16) |                                          \
                       (csECS_HashString(component_name) & 0xFFFF)))

// String hashing function (exposed for the macro above)
unsigned int csECS_HashString(const char *str);

// Generic component operations
void *csECS_AssignComponent(csSceneHandle scene, csEntityID entity,
                            csComponentTypeID componentTypeId,
                            size_t componentSize);
void *csECS_GetComponent(csSceneHandle scene, csEntityID entity,
                         csComponentTypeID componentTypeId);
void csECS_RemoveComponent(csSceneHandle scene, csEntityID entity,
                           csComponentTypeID componentTypeId);

// Convenience macros for component operations with automatic type
// handling
#define CS_ECS_ASSIGN(scene, entity, component_type)                           \
  ((component_type *)csECS_AssignComponent(                                    \
      scene, entity, CS_ECS_COMPONENT_TYPE(component_type),                    \
      sizeof(component_type)))

#define CS_ECS_ASSIGN_N(scene, entity, component_name, size)                   \
  (csECS_AssignComponent(scene, entity,                                        \
                         CS_ECS_COMPONENT_TYPE_N(component_name, size), size))

#define CS_ECS_GET(scene, entity, component_type)                              \
  ((component_type *)csECS_GetComponent(                                       \
      scene, entity, CS_ECS_COMPONENT_TYPE(component_type)))

#define CS_ECS_GET_N(scene, entity, component_name, size)                      \
  (csECS_GetComponent(scene, entity,                                           \
                      CS_ECS_COMPONENT_TYPE_N(component_name, size)))

#define CS_ECS_REMOVE(scene, entity, component_type)                           \
  csECS_RemoveComponent(scene, entity, CS_ECS_COMPONENT_TYPE(component_type))

#define CS_ECS_REMOVE_N(scene, entity, component_name, size)                   \
  csECS_RemoveComponent(scene, entity,                                         \
                        CS_ECS_COMPONENT_TYPE_N(component_name, size))

// System management
typedef void (*csSystemFunction)();
void csECS_AddSystem(csSystemFunction system, bool isStartSystem);
void csECS_UpdateSystems(void);
void csECS_StartStartSystems(void);

// Entity iteration
typedef struct csEntityIterator_t *csEntityIteratorHandle;

// Creates an iterator for entities with all specified component types
csEntityIteratorHandle
csECS_CreateEntityIterator(csSceneHandle scene,
                           const csComponentTypeID *componentTypeIds,
                           int componentCount);
csEntityIteratorHandle csECS_CreateAllEntityIterator(
    csSceneHandle scene); // Iterates all valid entities
csEntityID csECS_IteratorNext(
    csEntityIteratorHandle iterator); // Returns csECS_INVALID_ENTITY
                                      // when no more entities
void csECS_DestroyEntityIterator(csEntityIteratorHandle iterator);

// Helper macro for creating iterators with specific component types
#define CS_ECS_ITER_START(scene, ...)                                          \
  do {                                                                         \
    csComponentTypeID _types[] = {__VA_ARGS__};                                \
    csEntityIteratorHandle _iter = csECS_CreateEntityIterator(                 \
        scene, _types, sizeof(_types) / sizeof(_types[0]));                    \
    csEntityID _entity;                                                        \
    while ((_entity = csECS_IteratorNext(_iter)) != CS_ECS_INVALID_ENTITY) {

#define CS_ECS_ITER_END()                                                      \
  }                                                                            \
  csECS_DestroyEntityIterator(_iter);                                          \
  }                                                                            \
  while (0)

#ifdef __cplusplus
}
#endif
