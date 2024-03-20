#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <vector>
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/glm/gtx/rotate_vector.hpp"
#include "../vertex_array.h"

class Sphere : public VertexArray {
private:
    glm::vec3 calcPoint(int i, int j, double& z_step, double& step, double& h, double& diameter, int& sign1, int& sign2, bool flip);
    glm::vec3 normalize(glm::vec3 a, glm::vec3 b, double length);
public:
    Sphere(double radius, glm::vec3 pos = glm::vec3(0.0), int steps = 100, glm::vec3 color = glm::vec3(0.8));
};