#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "../vertex_array.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>
#include <vector>

class Plane : public VertexArray {
  public:
    Plane(glm::vec3 pos, float length);
    static VAOBuffers calcPlaneBuffers(glm::vec3 pos, float length);
    void draw(int shaderProgram);
};