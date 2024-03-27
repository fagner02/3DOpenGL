#pragma once
#include <iostream>
#include <vector>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "include/GL/glut.h"
#include "include/GL/glext.h"
#include "vertex_array.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <IL/il.h>

struct Mesh {
    unsigned int baseVertex;
    unsigned int baseIndex;
    unsigned int indexNum;
    unsigned int materialIndex;
};

class Model : public VertexArray {
public:
    std::vector<Mesh> meshes;
    Model();
    ~Model();

    void loadFile(const char* name);

    void CreateLightList();

    void EnableLights();

    void DisableLights();

    void draw();

    void loadTextures(const aiScene* scene);

    VAOBuffers getModelBuffers(const char* name);
private:
    std::vector<Texture> textures;
};
