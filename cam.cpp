#include "cam.h"

Camera::Camera(float width, float height) {
    proj = glm::perspective(glm::radians(90.0f), (float)(width / height), 0.1f, 100.0f);
    view = glm::lookAt(pos, pos + orientation, up);
}

void Camera::applyMatrix(int shaderProgram) {
    int projLoc = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}

void Camera::applyLightPos(int shaderProgram, glm::vec3 lightPos) {
    int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
}

void Camera::applyLightColor(int shaderProgram, glm::vec3 lightColor) {
    int lightPosLoc = glGetUniformLocation(shaderProgram, "lightColor");
    glUniform3f(lightPosLoc, lightColor.x, lightColor.y, lightColor.z);
}