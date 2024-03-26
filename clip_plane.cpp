#include "clip_plane.h"

void ClipPlane::calculateClipPlane() {
    glm::vec3 planeNormal(0.0, -1.0, 0.0);
    planeNormal = glm::rotate(planeNormal, clipAngle, glm::vec3(0.0, 1.0, 1.0));
    // glm::vec3 planeNormal(sin(clipAngle), -1.0, 0.0);
    glm::vec3 planePoint(0.0, planeHeight, 0.0);
    clipPlane = glm::vec4(planeNormal.x, planeNormal.y, planeNormal.z, -glm::dot(planeNormal, planePoint));
}

void ClipPlane::applyClipPlane(int shaderProgram) {
    int clipPLaneLoc = glGetUniformLocation(shaderProgram, "clipPlane");
    glUniform4fv(clipPLaneLoc, 1, glm::value_ptr(clipPlane));
}
