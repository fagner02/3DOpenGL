#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <windows.h>
#include <GL/glut.h>
#include <GL/glext.h>


class ClipPlane {
public:
    float clipAngle = glm::radians(4.0);
    float planeHeight = 0;
    glm::vec4 clipPlane;

    void calculateClipPlane();
    void applyClipPlane(int shaderProgram);
};