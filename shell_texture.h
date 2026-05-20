#pragma once
#include "extrude_point.h"
#include "model.h"
#include "vertex_array.h"
#include <iostream>
#include <vector>

class ShellTexture {
    std::vector<VertexArray *> meshes;
    std::vector<Mesh> modelMeshes;

  public:
    glm::mat4 *modelMatrix;
    int shellNum = 50;
    float shellHeight = 0.1;
    int divisions;
    bool hasIndexes;

    ShellTexture(VAOBuffers buffers, int shellNum, float shellHeight,
                 int divisions, glm::mat4 *modelMatrix,
                 std::vector<Mesh> modelMeshes);
    ShellTexture(VAOBuffers buffers, int shellNum, float shellHeight,
                 int divisions, glm::mat4 *modelMatrix);
    void initialize(VAOBuffers buffers, int shellNum, float shellHeight,
                    int divisions, glm::mat4 *modelMatrix);
    void applyMatrix(unsigned int shaderProgram);
    void draw(unsigned int shaderProgram);
    VertexArray *getFirstShell();
};