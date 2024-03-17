#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <vector>
#include "./include/glm/glm.hpp"
#include "./include/glm/gtc/type_ptr.hpp"
#include "./include/glm/gtx/rotate_vector.hpp"
#include "vertex_array.h"

class Cube : public VertexArray {
public:
    glm::vec3 pos;
    glm::mat4 modelMatrix;
    Cube(glm::vec3 pos, float size = 0.1);
};