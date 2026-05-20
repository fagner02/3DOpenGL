#pragma once
#include <iostream>
#include <string.h>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif

#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "texture.h"
#include "vertex_array.h"

#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include <IL/il.h>
#include <glm/glm.hpp>

class Model : public VertexArray {
  public:
    std::string filename;
    Model();

    void loadFile(const char *name);

    void saveFile(std::string destfile, glm::mat4 transform);

    void draw(int shaderProgram);

    void loadTextures(const aiScene *scene);

    VAOBuffers getModelBuffers(const char *name);

  private:
    std::vector<Texture> textures;
};
