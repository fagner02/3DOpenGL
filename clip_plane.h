#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>

class ClipPlane {
  public:
    float clipAngle = glm::radians(4.0);
    float planeHeight = 0;
    glm::vec4 clipPlane;

    void calculateClipPlane();
    void applyClipPlane(int shaderProgram);
};