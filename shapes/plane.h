#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <vector>
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/glm/gtx/rotate_vector.hpp"
#include "../vertex_array.h"

class Plane : public VertexArray {
public:

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    Plane(glm::vec3 pos, float length);
    void draw(int shaderProgram);
};