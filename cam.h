#include <iostream>
#include "./include/glm/glm.hpp"
#include "./include/glm/gtc/type_ptr.hpp"
#ifdef _WIN32
#include <windows.h>
#endif
#include "./include/GL/glut.h"
#include "./include/GL/glext.h"

class Camera {
public:
    glm::vec3 pos = glm::vec3(0.0, 0.1, 2.0);
    glm::vec3 orientation = glm::vec3(0.0, 0.0, -0.1);
    glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
    glm::mat4 proj = glm::mat4(1.0);
    glm::mat4 view = glm::mat4(1.0);
    Camera(float width, float height);
    void applyMatrix(int shaderProgram);
    void applyLightPos(int shaderProgram, glm::vec3 lightPos);
    void applyLightColor(int shaderProgram, glm::vec3 lightColor);
};