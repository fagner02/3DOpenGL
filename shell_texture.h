#pragma once
#include <iostream>
#include <vector>
#include "vertex_array.h"
#include "extrude_point.h"
#include "model.h"

class ShellTexture {
    std::vector<VertexArray*> meshes;
    std::vector<Mesh> modelMeshes;
public:
    int shellNum = 50;
    float shellHeight = 0.1;
    int divisions;
    bool hasIndexes;

    ShellTexture(VAOBuffers buffers, int shellNum, float shellHeight, int divisions, std::vector<Mesh> modelMeshes);

    void draw(int shaderProgram);
};