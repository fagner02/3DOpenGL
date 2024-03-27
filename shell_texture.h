#pragma once
#include <iostream>
#include <vector>
#include "vertex_array.h"
#include "extrude_point.h"

class ShellTexture {
    std::vector<VertexArray*> meshes;
public:
    int shellNum = 100;
    float shellHeight = 0.1;
    bool hasIndexes;

    ShellTexture(VAOBuffers buffers);

    void draw(int shaderProgram);
};