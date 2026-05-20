#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "../extrude_point.h"
#include "../vertex_array.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>
#include <vector>

class Sphere : public VertexArray {
  private:
    static glm::vec3 calcPoint(int i, int j, double &z_step, double &step,
                               double &h, double &diameter, int &sign1,
                               int &sign2, bool flip);

  public:
    glm::vec3 pos;
    Sphere(double radius, glm::vec3 pos = glm::vec3(0.0), int steps = 100,
           glm::vec3 color = glm::vec3(0.8));
    static VAOBuffers calcSphereBuffers(double radius, glm::vec3 pos, int steps,
                                        glm::vec3 color = glm::vec3(0.8));
    void draw(int shaderProgram);
};