#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "../vertex_array.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>
#include <vector>

class Line : public VertexArray {
  public:
    Line(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 color = glm::vec3(1.0f));
    void draw(int shaderProgram);
};

class Lines : public VertexArray {
  public:
    Lines(std::vector<glm::vec3> positions, std::vector<glm::vec3> color);
    void draw(int shaderProgram);
};