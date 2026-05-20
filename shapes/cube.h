#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "../vertex_array.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>
#include <vector>

class Cube : public VertexArray {
  public:
    glm::vec3 pos;
    Cube(glm::vec3 pos, float size = 0.1);
    void draw(int shaderProgram);
    static VAOBuffers calcCubeVertices(glm::vec3 pos, float size);
};