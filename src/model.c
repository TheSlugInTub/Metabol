// #include <salamander/model.h>
// #include <salamander/utils.h>
// #include <salamander/state.h>
// #include <salamander/renderer.h>
// #include <salamander/imgui_layer.h>
// #include <salamander/shader.h>
// #include <salamander/components.h>
// #include <salamander/light_3d.h>
// #include <salamander/config.h>
// #include <stb_image.h>
// #include <assert.h>
// 
// GLenum glCheckError_(const char* file, int line)
// {
//     GLenum errorCode;
//     while ((errorCode = glGetError()) != GL_NO_ERROR)
//     {
//         char error[256];
//         switch (errorCode)
//         {
//             case GL_INVALID_ENUM:
//                 strcpy(error, "INVALID_ENUM");
//                 break;
//             case GL_INVALID_VALUE:
//                 strcpy(error, "INVALID_VALUE");
//                 break;
//             case GL_INVALID_OPERATION:
//                 strcpy(error, "INVALID_OPERATION");
//                 break;
//             case GL_STACK_OVERFLOW:
//                 strcpy(error, "STACK_OVERFLOW");
//                 break;
//             case GL_STACK_UNDERFLOW:
//                 strcpy(error, "STACK_UNDERFLOW");
//                 break;
//             case GL_OUT_OF_MEMORY:
//                 strcpy(error, "OUT_OF_MEMORY");
//                 break;
//             case GL_INVALID_FRAMEBUFFER_OPERATION:
//                 strcpy(error, "INVALID_FRAMEBUFFER_OPERATION");
//                 break;
//         }
//         printf("%s | %s ( %d )\n", error, file, line);
//     }
//     return errorCode;
// }
// 
// #define glCheckError() glCheckError_(__FILE__, __LINE__)
// 
// smMesh smMesh_Create(smVector* meshVertices, smVector* meshIndices,
//                      smVector* meshTextures)
// {
//     smMesh mesh = {};
// 
//     mesh.vertices = meshVertices;
//     mesh.indices = meshIndices;
//     mesh.textures = meshTextures;
// 
//     // create buffers/arrays
//     glGenVertexArrays(1, &mesh.VAO);
//     glGenBuffers(1, &mesh.VBO);
//     glGenBuffers(1, &mesh.EBO);
// 
//     glBindVertexArray(mesh.VAO);
//     // load data into vertex buffers
//     glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
// 
//     // A great thing about structs is that their memory layout is
//     // sequential for all its items. The effect is that we can simply
//     // pass a pointer to the struct and it translates perfectly to a
//     // glm::vec3/2 array which again translates to 3/2 floats which
//     // translates to a byte array.
// 
//     smVertex* vertex = (smVertex*)smVector_Get(mesh.vertices, 0);
// 
//     glBufferData(GL_ARRAY_BUFFER,
//                  mesh.vertices->size * sizeof(smVertex), vertex,
//                  GL_STATIC_DRAW);
// 
//     unsigned int* index =
//         (unsigned int*)smVector_Get(mesh.indices, 0);
// 
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER,
//                  mesh.indices->size * sizeof(unsigned int), index,
//                  GL_STATIC_DRAW);
// 
//     // set the vertex attribute pointers
//     // vertex Positions
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(smVertex),
//                           (void*)0);
//     // vertex normals
//     glEnableVertexAttribArray(1);
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(smVertex),
//                           (void*)offsetof(smVertex, normal));
//     // vertex texture coords
//     glEnableVertexAttribArray(2);
//     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(smVertex),
//                           (void*)offsetof(smVertex, texCoords));
//     // vertex tangent
//     glEnableVertexAttribArray(3);
//     glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(smVertex),
//                           (void*)offsetof(smVertex, tangent));
//     // vertex bitangent
//     glEnableVertexAttribArray(4);
//     glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(smVertex),
//                           (void*)offsetof(smVertex, bitangent));
// 
//     return mesh;
// }
// 
// void smMesh_SetupBones(smMesh* mesh)
// {
//     // ids
//     glEnableVertexAttribArray(5);
//     glVertexAttribIPointer(5, 4, GL_INT, sizeof(smVertex),
//                            (void*)offsetof(smVertex, boneIDs));
// 
//     // weights
//     glEnableVertexAttribArray(6);
//     glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(smVertex),
//                           (void*)offsetof(smVertex, weights));
//     glBindVertexArray(0);
// }
// 
// void smMesh_Destroy(smMesh* mesh)
// {
//     smVector_Free(mesh->vertices);
//     smVector_Free(mesh->indices);
//     smVector_Free(mesh->textures);
// }
// 
// void smBoneInfoMap_Create(smBoneInfoMap* map, int capacity)
// {
//     map->entries =
//         (smBoneMapEntry*)calloc(capacity, sizeof(smBoneMapEntry));
//     map->capacity = capacity;
//     map->count = 0;
//     for (int i = 0; i < capacity; i++)
//     {
//         map->entries[i].used = false;
//     }
// }
// 
// void smBoneInfoMap_Destroy(smBoneInfoMap* map)
// {
//     free(map->entries);
//     map->entries = NULL;
//     map->capacity = 0;
//     map->count = 0;
// }
// 
// smBoneInfo* smBoneInfoMap_Find(smBoneInfoMap* map, const char* name)
// {
//     for (int i = 0; i < map->capacity; i++)
//     {
//         if (map->entries[i].used &&
//             strcmp(map->entries[i].name, name) == 0)
//         {
//             return &map->entries[i].info;
//         }
//     }
//     return NULL;
// }
// 
// void smBoneInfoMap_Insert(smBoneInfoMap* map, const char* name,
//                           smBoneInfo info)
// {
//     // Simple implementation - in a real app, you'd want a better hash
//     // map
//     for (int i = 0; i < map->capacity; i++)
//     {
//         if (!map->entries[i].used)
//         {
//             strncpy(map->entries[i].name, name, 127);
//             map->entries[i].name[127] = '\0';
//             map->entries[i].info = info;
//             map->entries[i].used = true;
//             map->count++;
//             return;
//         }
//     }
//     // No space available - in a real app, you'd resize the map
//     fprintf(stderr, "ERROR: Bone map is full\n");
// }
// 
// void smAssimpMat4ToGLM(const struct aiMatrix4x4* from, mat4 to)
// {
//     to[0][0] = from->a1;
//     to[0][1] = from->b1;
//     to[0][2] = from->c1;
//     to[0][3] = from->d1;
//     to[1][0] = from->a2;
//     to[1][1] = from->b2;
//     to[1][2] = from->c2;
//     to[1][3] = from->d2;
//     to[2][0] = from->a3;
//     to[2][1] = from->b3;
//     to[2][2] = from->c3;
//     to[2][3] = from->d3;
//     to[3][0] = from->a4;
//     to[3][1] = from->b4;
//     to[3][2] = from->c4;
//     to[3][3] = from->d4;
// }
// 
// void smAssimpVec3ToGLM(const struct aiVector3D* from, vec3 to)
// {
//     to[0] = from->x;
//     to[1] = from->y;
//     to[2] = from->z;
// }
// 
// void smModel_Create(smModel* model)
// {
//     model->loadedTextures = smVector_Create(sizeof(smTexture), 2);
//     model->meshes = smVector_Create(sizeof(smMesh), 2);
// 
//     model->boneCounter = 0;
// 
//     // Initialize bone map with some reasonable capacity
//     smBoneInfoMap_Create(&model->boneInfoMap, 100);
// }
// 
// void smModel_Load(smModel* model, const char* path)
// {
//     if (!model->extractTexture)
//     {
//         model->texture = smUtils_LoadTexture(model->texturePath);
//     }
// 
//     // Extract directory from path
//     strncpy(model->directory, path, sizeof(model->directory) - 1);
//     model->directory[sizeof(model->directory) - 1] = '\0';
// 
//     // Find last slash to extract directory
//     char* lastSlash = strrchr(model->directory, '/');
//     if (lastSlash != NULL)
//     {
//         *(lastSlash + 1) = '\0';
//     }
//     else
//     {
//         // If no slash found, set directory to empty string
//         model->directory[0] = '\0';
//     }
// 
//     // Read file via ASSIMP
//     const struct aiScene* scene = aiImportFile(
//         path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
//                   aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
// 
//     // Check for errors
//     if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
//         !scene->mRootNode)
//     {
//         fprintf(stderr, "ERROR::ASSIMP:: %s\n", aiGetErrorString());
//         return;
//     }
// 
//     // Process ASSIMP's root node recursively
//     smModel_ProcessNode(model, scene->mRootNode, scene);
// }
// 
// // Process node recursively
// void smModel_ProcessNode(smModel* model, struct aiNode* node,
//                          const struct aiScene* scene)
// {
//     // Process each mesh at the current node
//     for (unsigned int i = 0; i < node->mNumMeshes; i++)
//     {
//         struct aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//         smMesh         processedMesh =
//             smModel_ProcessMesh(model, mesh, scene);
// 
//         smVector_PushBack(model->meshes, &processedMesh);
//     }
// 
//     // Process children nodes
//     for (unsigned int i = 0; i < node->mNumChildren; i++)
//     {
//         smModel_ProcessNode(model, node->mChildren[i], scene);
//     }
// }
// 
// // Process mesh
// smMesh smModel_ProcessMesh(smModel* model, struct aiMesh* mesh,
//                            const struct aiScene* scene)
// {
//     smVector* vertices; // Vector of Vertex
//     smVector* indices;  // Vector of unsigned int
//     smVector* textures; // Vector of Texture
// 
//     // Initialize vectors
//     vertices = smVector_Create(sizeof(smVertex), 10);
//     indices = smVector_Create(sizeof(unsigned int), 10);
//     textures = smVector_Create(sizeof(smTexture), 2);
// 
//     // Process vertices
//     for (unsigned int i = 0; i < mesh->mNumVertices; i++)
//     {
//         smVertex vertex;
//         smModel_SetVertexBoneDataToDefault(&vertex);
// 
//         // Position
//         smAssimpVec3ToGLM(&mesh->mVertices[i], vertex.position);
// 
//         // Normal
//         if (mesh->mNormals)
//         {
//             smAssimpVec3ToGLM(&mesh->mNormals[i], vertex.normal);
//         }
// 
//         // Texture coordinates
//         if (mesh->mTextureCoords[0])
//         {
//             vertex.texCoords[0] = mesh->mTextureCoords[0][i].x;
//             vertex.texCoords[1] = mesh->mTextureCoords[0][i].y;
//         }
//         else
//         {
//             vertex.texCoords[0] = 0.0f;
//             vertex.texCoords[1] = 0.0f;
//         }
// 
//         smVector_PushBack(vertices, (void*)&vertex);
//     }
// 
//     // Process indices
//     for (unsigned int i = 0; i < mesh->mNumFaces; i++)
//     {
//         struct aiFace face = mesh->mFaces[i];
//         for (unsigned int j = 0; j < face.mNumIndices; j++)
//         {
//             smVector_PushBack(indices, (void*)&face.mIndices[j]);
//         }
//     }
// 
//     // Process material/textures
//     if (model->extractTexture && mesh->mMaterialIndex >= 0)
//     {
//         struct aiMaterial* material =
//             scene->mMaterials[mesh->mMaterialIndex];
// 
//         // Load diffuse maps
//         smModel_LoadMaterialTextures(
//             model, material, aiTextureType_DIFFUSE, "texture_diffuse",
//             scene, textures);
// 
//         // Load specular maps
//         smModel_LoadMaterialTextures(
//             model, material, aiTextureType_SPECULAR,
//             "texture_specular", scene, textures);
// 
//         // Load normal maps
//         smModel_LoadMaterialTextures(
//             model, material, aiTextureType_HEIGHT, "texture_normal",
//             scene, textures);
// 
//         // Load height maps
//         smModel_LoadMaterialTextures(
//             model, material, aiTextureType_AMBIENT, "texture_height",
//             scene, textures);
//     }
// 
//     // Extract bone weights
//     smModel_ExtractBoneWeightForVertices(model, vertices, mesh,
//                                          scene);
// 
//     smMesh result = smMesh_Create(vertices, indices, textures);
// 
//     return result;
// }
// 
// // Set default bone data for vertex
// void smModel_SetVertexBoneDataToDefault(smVertex* vertex)
// {
//     for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
//     {
//         vertex->boneIDs[i] = -1;
//         vertex->weights[i] = 0.0f;
//     }
// }
// 
// // Set bone data for vertex
// void smModel_SetVertexBoneData(smVertex* vertex, int boneID,
//                                float weight)
// {
//     for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
//     {
//         if (vertex->boneIDs[i] < 0)
//         {
//             vertex->weights[i] = weight;
//             vertex->boneIDs[i] = boneID;
//             break;
//         }
//     }
// }
// 
// // Extract bone weights for vertices
// void smModel_ExtractBoneWeightForVertices(smModel*       model,
//                                           smVector*      vertices,
//                                           struct aiMesh* mesh,
//                                           const struct aiScene* scene)
// {
//     size_t vertexCount = vertices->size;
// 
//     for (int boneIndex = 0; boneIndex < (int)mesh->mNumBones;
//          ++boneIndex)
//     {
//         int         boneID = -1;
//         const char* boneName = mesh->mBones[boneIndex]->mName.data;
// 
//         // Find bone in map or create new entry
//         smBoneInfo* found =
//             smBoneInfoMap_Find(&model->boneInfoMap, boneName);
// 
//         if (found == NULL)
//         {
//             // Create new bone info
//             smBoneInfo newBoneInfo;
//             newBoneInfo.ID = model->boneCounter;
//             smAssimpMat4ToGLM(&mesh->mBones[boneIndex]->mOffsetMatrix,
//                               newBoneInfo.offset);
// 
//             smBoneInfoMap_Insert(&model->boneInfoMap, boneName,
//                                  newBoneInfo);
//             boneID = model->boneCounter;
//             model->boneCounter++;
//         }
//         else
//         {
//             boneID = found->ID;
//         }
// 
//         assert(boneID != -1);
// 
//         struct aiVertexWeight* weights =
//             mesh->mBones[boneIndex]->mWeights;
//         int numWeights = mesh->mBones[boneIndex]->mNumWeights;
// 
//         for (int weightIndex = 0; weightIndex < numWeights;
//              ++weightIndex)
//         {
//             int   vertexId = weights[weightIndex].mVertexId;
//             float weight = weights[weightIndex].mWeight;
// 
//             assert(vertexId < vertices->size);
// 
//             smVertex* vertex =
//                 (smVertex*)smVector_Get(vertices, vertexId);
//             smModel_SetVertexBoneData(vertex, boneID, weight);
//         }
//     }
// }
// 
// // Load material textures
// void smModel_LoadMaterialTextures(smModel*              model,
//                                   struct aiMaterial*    mat,
//                                   enum aiTextureType    type,
//                                   const char*           typeName,
//                                   const struct aiScene* scene,
//                                   smVector*             textures)
// {
//     // Get texture count for this type
//     unsigned int textureCount = aiGetMaterialTextureCount(mat, type);
// 
//     for (unsigned int i = 0; i < textureCount; i++)
//     {
//         struct aiString path;
//         aiGetMaterialTexture(mat, type, i, &path, NULL, NULL, NULL,
//                              NULL, NULL, NULL);
// 
//         // Check if texture was loaded before
//         bool skip = false;
// 
//         for (size_t j = 0; j < model->loadedTextures->size; j++)
//         {
//             smTexture* loadedTexture =
//                 (smTexture*)smVector_Get(model->loadedTextures, j);
//             if (strcmp(loadedTexture->path, path.data) == 0)
//             {
//                 smVector_PushBack(textures, (void*)loadedTexture);
//                 skip = true;
//                 break;
//             }
//         }
// 
//         if (!skip)
//         {
//             smTexture texture;
//             strncpy(texture.path, path.data,
//                     sizeof(texture.path) - 1);
//             texture.path[sizeof(texture.path) - 1] = '\0';
// 
//             // Check if this is an embedded texture
//             if (path.data[0] == '*')
//             {
//                 // Extract embedded texture index
//                 unsigned int embeddedIndex = atoi(path.data + 1);
//                 if (embeddedIndex < scene->mNumTextures)
//                 {
//                     const struct aiTexture* embeddedTexture =
//                         scene->mTextures[embeddedIndex];
//                     texture.ID =
//                         smTextureFromEmbeddedData(embeddedTexture);
//                 }
//             }
//             else
//             {
//                 texture.ID =
//                     smTextureFromFile(path.data, model->directory);
//             }
// 
//             strncpy(texture.type, typeName, sizeof(texture.type) - 1);
//             texture.type[sizeof(texture.type) - 1] = '\0';
// 
//             smVector_PushBack(textures, &texture);
//             smVector_PushBack(model->loadedTextures, &texture);
//         }
//     }
// }
// 
// // Free model resources
// void smModel_Destroy(smModel* model)
// {
//     for (size_t i = 0; i < model->meshes->size; i++)
//     {
//         smMesh* mesh = (smMesh*)smVector_Get(model->meshes, i);
//         smMesh_Destroy(mesh);
//     }
// 
//     smVector_Free(model->meshes);
//     smVector_Free(model->loadedTextures);
// 
//     // Free bone map
//     smBoneInfoMap_Destroy(&model->boneInfoMap);
// }
// 
// unsigned int smTextureFromFile(const char* path,
//                                const char* directory)
// {
//     char fullPath[128];
// 
//     // Combine directory and path
//     if (path[0] == '/' || (path[0] != '\0' && path[1] == ':'))
//     {
//         // Absolute path
//         strncpy(fullPath, path, sizeof(fullPath) - 1);
//     }
//     else
//     {
//         // Relative path - combine with directory
//         snprintf(fullPath, sizeof(fullPath), "%s%s", directory, path);
//     }
// 
//     fullPath[sizeof(fullPath) - 1] = '\0'; // Ensure null termination
// 
//     // Call your existing texture loading function
//     return smUtils_LoadTexture(fullPath);
// }
// 
// unsigned int
// smTextureFromEmbeddedData(const struct aiTexture* texture)
// {
//     unsigned int textureID;
//     glGenTextures(1, &textureID);
// 
//     // Determine if the texture is compressed (stored as a file format
//     // like jpg/png) or raw (stored as raw RGBA data)
//     if (texture->mHeight == 0)
//     {
//         // Compressed texture - load with stbi from memory
//         int width, height, nrComponents;
// 
//         // The texture data is stored in mData as a file format (e.g.,
//         // PNG, JPG)
//         unsigned char* data = stbi_load_from_memory(
//             (unsigned char*)texture->pcData,
//             texture
//                 ->mWidth, // In this case, mWidth is the size in bytes
//             &width, &height, &nrComponents, 0);
// 
//         if (data)
//         {
//             GLenum format;
//             if (nrComponents == 1)
//                 format = GL_RED;
//             else if (nrComponents == 3)
//                 format = GL_RGB;
//             else if (nrComponents == 4)
//                 format = GL_RGBA;
// 
//             glBindTexture(GL_TEXTURE_2D, textureID);
//             glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
//                          format, GL_UNSIGNED_BYTE, data);
//             glGenerateMipmap(GL_TEXTURE_2D);
// 
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
//                             format == GL_RGBA ? GL_CLAMP_TO_EDGE
//                                               : GL_REPEAT);
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
//                             format == GL_RGBA ? GL_CLAMP_TO_EDGE
//                                               : GL_REPEAT);
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
//                             GL_NEAREST_MIPMAP_NEAREST);
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
//                             GL_NEAREST);
// 
//             glBindTexture(GL_TEXTURE_2D, 0);
//             stbi_image_free(data);
//         }
//         else
//         {
//             printf("Embedded texture failed to load\n");
//             stbi_image_free(data);
//             return smUtils_LoadTexture(
//                 "res/textures/MissingTexture.png");
//         }
//     }
//     else
//     {
//         // Raw texture - texture->mHeight contains height, and data is
//         // directly in texture->pcData
//         int width = texture->mWidth;
//         int height = texture->mHeight;
// 
//         // Assuming the format is RGBA (most common for embedded
//         // textures) If you know the format is different, you'll need
//         // to adapt this
//         GLenum format = GL_RGBA;
// 
//         glBindTexture(GL_TEXTURE_2D, textureID);
//         glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
//                      format, GL_UNSIGNED_BYTE, texture->pcData);
//         glGenerateMipmap(GL_TEXTURE_2D);
// 
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
//                         format == GL_RGBA ? GL_CLAMP_TO_EDGE
//                                           : GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
//                         format == GL_RGBA ? GL_CLAMP_TO_EDGE
//                                           : GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
//                         GL_NEAREST_MIPMAP_NEAREST);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
//                         GL_NEAREST);
// 
//         glBindTexture(GL_TEXTURE_2D, 0);
//     }
// 
//     return textureID;
// }
// 
// void smModel_Draw(smModel* mesh, smTransform* trans, smShader shader)
// {
//     if (mesh->meshes == NULL)
//     {
//         smModel_Create(mesh);
//     }
// 
//     smShader_Use(shader);
// 
//     smShader_SetVec3(shader, "viewPos", smState.camera.position);
//     smShader_SetFloat(shader, "specularity", mesh->specularity);
// 
//     for (int i = 0; i < mesh->meshes->size; ++i)
//     {
//         smMesh* actualMesh = (smMesh*)smVector_Get(mesh->meshes, i);
// 
//         if (mesh->extractTexture)
//         {
//             // bind appropriate textures
//             unsigned int diffuseNr = 1;
//             unsigned int specularNr = 1;
//             unsigned int normalNr = 1;
//             unsigned int heightNr = 1;
// 
//             for (unsigned int j = 0; j < actualMesh->textures->size;
//                  j++)
//             {
//                 glActiveTexture(GL_TEXTURE0 + j);
// 
//                 char       number[128];
//                 smTexture* texture =
//                     (smTexture*)smVector_Get(actualMesh->textures, j);
//                 if (strcmp(texture->type, "texture_diffuse") == 0)
//                 {
//                     strcpy(number, "");
//                     sprintf(number, "%d", diffuseNr++);
//                 }
//                 else if (strcmp(texture->type, "texture_specular") ==
//                          0)
//                 {
//                     strcpy(number, "");
//                     sprintf(number, "%d", specularNr++);
//                 }
//                 else if (strcmp(texture->type, "texture_normal") == 0)
//                 {
//                     strcpy(number, "");
//                     sprintf(number, "%d", normalNr++);
//                 }
//                 else if (strcmp(texture->type, "texture_height") == 0)
//                 {
//                     strcpy(number, "");
//                     sprintf(number, "%d", heightNr++);
//                 }
// 
//                 char name[128];
//                 sprintf(name, "%s", texture->type);
// 
//                 // now set the sampler to the correct texture unit
//                 glUniform1i(glGetUniformLocation(shader.ID, name), j);
// 
//                 unsigned int id = ((smTexture*)smVector_Get(
//                                        actualMesh->textures, j))
//                                       ->ID;
// 
//                 // and finally bind the texture
//                 glBindTexture(GL_TEXTURE_2D, id);
//             }
//         }
//         else
//         {
//             smShader_SetTexture2D(shader, "texture_diffuse",
//                                   mesh->texture, 0);
//             smShader_SetTexture2D(shader, "texture_normal",
//                                   mesh->normalTexture, 1);
//             smShader_SetTexture2D(shader, "texture_specular",
//                                   mesh->specularTexture, 2);
//         }
// 
//         smShader_SetMat4(shader, "projection", smState.persProj);
// 
//         mat4 view;
//         smCamera_GetViewMatrix(&smState.camera, view);
// 
//         smShader_SetMat4(shader, "view", view);
// 
//         mat4 transform;
// 
//         glm_mat4_identity(transform);
// 
//         glm_translate(transform,
//                       (vec3) {trans->position[0], trans->position[1],
//                               trans->position[2]});
//         glm_quat_rotate(transform, trans->rotation, transform);
//         glm_scale(transform, (vec3) {trans->scale[0], trans->scale[1],
//                                      trans->scale[2]});
// 
//         smShader_SetMat4(shader, "model", transform);
// 
//         glActiveTexture(GL_TEXTURE0);
// 
//         // draw mesh
//         glBindVertexArray(actualMesh->VAO);
//         glDrawElements(GL_TRIANGLES,
//                        (unsigned int)actualMesh->indices->size,
//                        GL_UNSIGNED_INT, 0);
// 
//         glBindVertexArray(0);
//     }
// }
// 
// void smMeshRenderer_StartSys()
// {
//     SM_ECS_ITER_START(smState.scene,
//                       SM_ECS_COMPONENT_TYPE(smMeshRenderer))
//     {
//         smMeshRenderer* mesh =
//             SM_ECS_GET(smState.scene, _entity, smMeshRenderer);
// 
//         smModel_Create(mesh);
// 
//         smModel_Load(mesh, mesh->modelPath);
//     }
//     SM_ECS_ITER_END();
// }
// 
// void smMeshRenderer_Sys()
// {
//     // Pre-compute light data to avoid repeated iterations
//     struct LightData
//     {
//         vec3   position;
//         vec3   direction;
//         float  radius;
//         vec4   color;
//         float  intensity;
//         float  falloff;
//         bool   castsShadows;
//         GLuint depthCubemap;
//     } lights[SM_MAX_LIGHTS];
//     int lightCount = 0;
// 
//     // First pass: Shadow map generation for all lights
//     SM_ECS_ITER_START(smState.scene, SM_ECS_COMPONENT_TYPE(smLight3D))
//     {
//         smLight3D* light =
//             SM_ECS_GET(smState.scene, _entity, smLight3D);
// 
//         // Skip if we've reached max lights
//         if (lightCount >= SM_MAX_LIGHTS)
//             break;
// 
//         // Store light data for main rendering pass
//         lights[lightCount] = (struct LightData) {
//             .position = {light->position[0], light->position[1],
//                          light->position[2]},
//             .radius = light->radius,
//             .color = {light->color[0], light->color[1],
//                       light->color[2], light->color[3]},
//             .intensity = light->intensity,
//             .castsShadows = light->castsShadows,
//             .depthCubemap = light->depthCubemap,
//             .falloff = light->falloff,
//             .direction = {light->direction[0], light->direction[1],
//                           light->direction[2]}};
// 
//         // Only generate shadows if this light casts them
//         if (light->castsShadows)
//         {
//             // Prepare shadow map
//             glViewport(0, 0, SM_SHADOW_WIDTH, SM_SHADOW_HEIGHT);
//             glBindFramebuffer(GL_FRAMEBUFFER, light->depthMapFBO);
// 
//             // Use our optimized shadow shader (no geometry shader)
//             smShader_Use(sm_shadowShader3d);
// 
//             if (light->directional)
//             {
//                 // Clear depth buffer for this face
//                 glFramebufferTexture2D(GL_FRAMEBUFFER,
//                                        GL_DEPTH_ATTACHMENT,
//                                        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
//                                        light->depthCubemap, 0);
//                 glClear(GL_DEPTH_BUFFER_BIT);
// 
//                 // Set the shadow matrix for this specific face
//                 mat4* shadowTransform =
//                     (mat4*)smVector_Get(light->shadowTransforms, 0);
//                 smShader_SetMat4(sm_shadowShader3d, "shadowMatrix",
//                                  (*shadowTransform));
// 
//                 // Set common uniforms
//                 smShader_SetFloat(sm_shadowShader3d, "far_plane",
//                                   light->radius);
//                 smShader_SetVec3(sm_shadowShader3d, "lightPos",
//                                  light->position);
// 
//                 // Render all meshes to this cubemap face
//                 SM_ECS_ITER_START(
//                     smState.scene,
//                     SM_ECS_COMPONENT_TYPE(smMeshRenderer))
//                 {
//                     smMeshRenderer* mesh = SM_ECS_GET(
//                         smState.scene, _entity, smMeshRenderer);
//                     smTransform* trans = SM_ECS_GET(
//                         smState.scene, _entity, smTransform);
// 
//                     if (mesh->invisible)
//                         continue;
// 
//                     smModel_Draw(mesh, trans, sm_shadowShader3d);
//                 }
//                 SM_ECS_ITER_END();
//             }
//             else
//             {
//                 // For each face of the cubemap
//                 for (unsigned int face = 0; face < 6; ++face)
//                 {
//                     // Clear depth buffer for this face
//                     glFramebufferTexture2D(
//                         GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
//                         GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
//                         light->depthCubemap, 0);
//                     glClear(GL_DEPTH_BUFFER_BIT);
// 
//                     // Set the shadow matrix for this specific face
//                     mat4* shadowTransform = (mat4*)smVector_Get(
//                         light->shadowTransforms, face);
//                     smShader_SetMat4(sm_shadowShader3d,
//                                      "shadowMatrix",
//                                      (*shadowTransform));
// 
//                     // Set common uniforms
//                     smShader_SetFloat(sm_shadowShader3d, "far_plane",
//                                       light->radius);
//                     smShader_SetVec3(sm_shadowShader3d, "lightPos",
//                                      light->position);
// 
//                     // Render all meshes to this cubemap face
//                     SM_ECS_ITER_START(
//                         smState.scene,
//                         SM_ECS_COMPONENT_TYPE(smMeshRenderer))
//                     {
//                         smMeshRenderer* mesh = SM_ECS_GET(
//                             smState.scene, _entity, smMeshRenderer);
//                         smTransform* trans = SM_ECS_GET(
//                             smState.scene, _entity, smTransform);
// 
//                         if (mesh->invisible)
//                             continue;
// 
//                         smModel_Draw(mesh, trans, sm_shadowShader3d);
//                     }
//                     SM_ECS_ITER_END();
//                 }
//             }
//         }
// 
//         glBindFramebuffer(GL_FRAMEBUFFER, 0);
//         lightCount++;
//     }
//     SM_ECS_ITER_END();
// 
//     // Reset viewport for main rendering
//     glViewport(0, 0, smState.window->width, smState.window->height);
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 
//     smShader_Use(sm_shader3d);
// 
//     // Bind light textures once before main rendering loop
//     for (int i = 0; i < lightCount; i++)
//     {
//         glActiveTexture(GL_TEXTURE4 + i);
//         glBindTexture(GL_TEXTURE_CUBE_MAP, lights[i].depthCubemap);
// 
//         char uniformName[64];
//         sprintf(uniformName, "depthMap[%d]", i);
//         smShader_SetInt(sm_shader3d, uniformName, i + 4);
//     }
// 
//     // Set null textures for remaining slots if any
//     for (int i = lightCount; i < SM_MAX_LIGHTS; i++)
//     {
//         glActiveTexture(GL_TEXTURE4 + i);
//         glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
// 
//         char uniformName[64];
//         sprintf(uniformName, "depthMap[%d]", i);
//         smShader_SetInt(sm_shader3d, uniformName, i + 4);
//     }
// 
//     // Set total light count
//     smShader_SetInt(sm_shader3d, "numLights", lightCount);
// 
//     // Prepare light uniforms in a single block call
//     for (int i = 0; i < lightCount; i++)
//     {
//         char uniformName[64];
// 
//         sprintf(uniformName, "light[%d].pos", i);
//         smShader_SetVec3(sm_shader3d, uniformName,
//                          lights[i].position);
// 
//         sprintf(uniformName, "light[%d].radius", i);
//         smShader_SetFloat(sm_shader3d, uniformName, lights[i].radius);
// 
//         sprintf(uniformName, "light[%d].color", i);
//         smShader_SetVec3(sm_shader3d, uniformName, lights[i].color);
// 
//         sprintf(uniformName, "light[%d].intensity", i);
//         smShader_SetFloat(sm_shader3d, uniformName,
//                           lights[i].intensity);
// 
//         sprintf(uniformName, "light[%d].falloff", i);
//         smShader_SetFloat(sm_shader3d, uniformName,
//                           lights[i].falloff);
// 
//         sprintf(uniformName, "light[%d].direction", i);
//         smShader_SetVec3(sm_shader3d, uniformName,
//                           lights[i].direction);
// 
//         sprintf(uniformName, "light[%d].castShadows", i);
//         smShader_SetBool(sm_shader3d, uniformName,
//                          lights[i].castsShadows);
//     }
// 
//     // Main rendering pass with pre-computed light data
//     SM_ECS_ITER_START(smState.scene,
//                       SM_ECS_COMPONENT_TYPE(smMeshRenderer))
//     {
//         smMeshRenderer* mesh =
//             SM_ECS_GET(smState.scene, _entity, smMeshRenderer);
//         smTransform* trans =
//             SM_ECS_GET(smState.scene, _entity, smTransform);
// 
//         if (mesh->invisible)
//             continue;
// 
//         smModel_Draw(mesh, trans, sm_shader3d);
//     }
//     SM_ECS_ITER_END();
// }
// 
// void smMeshRenderer_Draw(smMeshRenderer* mesh)
// {
//     if (smImGui_CollapsingHeader("Mesh Renderer"))
//     {
//         smImGui_InputText("ModelPath", mesh->modelPath, 128, 0);
//         smImGui_InputText("TexturePath", mesh->texturePath, 128, 0);
//         smImGui_InputText("NormalTexturePath",
//                           mesh->normalTexturePath, 128, 0);
//         smImGui_InputText("SpecularTexturePath",
//                           mesh->specularTexturePath, 128, 0);
// 
//         smImGui_DragFloat("Specularity", &mesh->specularity, 0.2f);
//         smImGui_Checkbox("Gamma correction", &mesh->gammaCorrection);
//         smImGui_Checkbox("Extract texture", &mesh->extractTexture);
//         smImGui_Checkbox("Invisible", &mesh->invisible);
// 
//         if (smImGui_Button("Load Model"))
//         {
//             smModel_Load(mesh, mesh->modelPath);
// 
//             if (!mesh->extractTexture)
//             {
//                 mesh->texture =
//                     smUtils_LoadTexture(mesh->texturePath);
//             }
//         }
// 
//         if (smImGui_Button("Reset paths"))
//         {
//             strcpy(mesh->modelPath, "");
//             strcpy(mesh->texturePath, "");
//             strcpy(mesh->normalTexturePath, "");
//             strcpy(mesh->specularTexturePath, "");
//         }
//     }
// }
// 
// smJson smMeshRenderer_Save(smMeshRenderer* mesh)
// {
//     smJson j = smJson_Create();
// 
//     smJson_SaveString(j, "ModelPath", mesh->modelPath);
//     smJson_SaveString(j, "TexturePath", mesh->texturePath);
//     smJson_SaveString(j, "NormalTexturePath",
//                       mesh->normalTexturePath);
//     smJson_SaveString(j, "SpecularTexturePath",
//                       mesh->specularTexturePath);
//     smJson_SaveFloat(j, "Specularity", mesh->specularity);
//     smJson_SaveBool(j, "GammaCorrection", mesh->gammaCorrection);
//     smJson_SaveBool(j, "ExtractTexture", mesh->extractTexture);
//     smJson_SaveBool(j, "Invisible", mesh->invisible);
// 
//     return j;
// }
// 
// void smMeshRenderer_Load(smMeshRenderer* mesh, smJson j)
// {
//     strcpy(mesh->modelPath, "");
//     strcpy(mesh->texturePath, "");
//     strcpy(mesh->normalTexturePath, "");
//     strcpy(mesh->specularTexturePath, "");
// 
//     smJson_LoadString(j, "ModelPath", mesh->modelPath);
//     smJson_LoadString(j, "TexturePath", mesh->texturePath);
//     smJson_LoadString(j, "NormalTexturePath",
//                       mesh->normalTexturePath);
//     smJson_LoadString(j, "SpecularTexturePath",
//                       mesh->specularTexturePath);
//     smJson_LoadFloat(j, "Specularity", &mesh->specularity);
//     smJson_LoadBool(j, "GammaCorrection", &mesh->gammaCorrection);
//     smJson_LoadBool(j, "ExtractTexture", &mesh->extractTexture);
//     smJson_LoadBool(j, "Invisible", &mesh->invisible);
// }
