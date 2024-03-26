#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <vector>
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/glm/gtx/rotate_vector.hpp"
#include "../vertex_array.h"

class Cube : public VertexArray {
public:

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec3> normals;
    glm::vec3 pos;
    Cube(glm::vec3 pos, float size = 0.1);
    void draw(int shaderProgram);
};