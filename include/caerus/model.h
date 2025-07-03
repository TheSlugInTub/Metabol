#pragma once

#include <caerus/vector.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <caerus/components.h>
#include <cglm/cglm.h>

#define MAX_BONE_INFLUENCE 4

typedef struct
{
    vec3 position;
    vec3 normal;
    vec2 texCoords;
    vec3 tangent;
    vec3 bitangent;

    int   boneIDs[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];
} smVertex;

typedef struct
{
    unsigned int ID;
    char         type[64];
    char         path[64];
} smTexture;

typedef struct
{
    smVector*    vertices; // smVertex
    smVector*    indices;  // unsigned int
    smVector*    textures; // smTexture
    unsigned int VAO, VBO, EBO;
} smMesh;

smMesh smMesh_Create(smVector* meshVertices, smVector* meshIndices,
                     smVector* meshTextures);

typedef struct
{
    char      modelPath[128];
} smModel;

void   smModel_Create(smModel* model);
void   smModel_Load(smModel* model, const char* path);
void   smModel_ProcessNode(smModel* model, struct aiNode* node,
                           const struct aiScene* scene);
smMesh smModel_ProcessMesh(smModel* model, struct aiMesh* mesh,
                           const struct aiScene* scene);
void smModel_LoadMaterialTextures(smModel*              model,
                                  struct aiMaterial*    mat,
                                  enum aiTextureType    type,
                                  const char*           typeName,
                                  const struct aiScene* scene,
                                  smVector*             textures);

unsigned int smTextureFromFile(const char* path,
                               const char* directory);

unsigned int
smTextureFromEmbeddedData(const struct aiTexture* texture);

void smAssimpVec3ToGLM(const struct aiVector3D* from, vec3 to);
void smAssimpMat4ToGLM(const struct aiMatrix4x4* from, mat4 to);
