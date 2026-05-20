#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>

class Camera {
  public:
    bool isOrthographic;
    glm::vec3 pos = glm::vec3(0.0, 0.1, 2.0);
    glm::vec3 orientation = glm::vec3(0.0, 0.0, -0.1);
    glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
    glm::mat4 proj = glm::mat4(1.0);
    glm::mat4 view = glm::mat4(1.0);
    float width;
    float height;
    Camera(float width, float height, glm::vec3 _pos = glm::vec3(0.0, 0.1, 2.0),
           bool orthographic = false);
    void orthographic();
    void perspective();
    void toggleOrthographic();
    void applyMatrix(int shaderProgram);
    void applyLightPos(int shaderProgram, glm::vec3 lightPos);
    void applyLightColor(int shaderProgram, glm::vec3 lightColor);
    void applyLightProj(int shaderProgram, glm::mat4 proj);
};