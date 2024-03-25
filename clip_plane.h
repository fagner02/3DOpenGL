#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glext.h>


class ClipPlane {
    float clipAngle = glm::radians(0.0);
    float planeHeight = 0;
    glm::vec4 clipPlane;
    void calculateClipPlane();
    void applyClipPlane(int shaderProgram);
};