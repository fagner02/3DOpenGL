#include "cam.h"

Camera::Camera(float width, float height, glm::vec3 pos) {
    proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);
    // glm::mat4 i = glm::scale(glm::identity<glm::mat4>(), glm::vec3(0.5, 0.5, 1.));
    // std::cout << pos.y << " " << pos.z << "\n";
    // if (pos.y == 0.1 && pos.z == 2.0) {
    // proj = (glm::mat4)glm::orthoZO<double>(-1, 1, -1, 1, 0.1, 100);
    // } else {
    // proj = (glm::mat4)glm::ortho<double>(-1, 1, -1, 1, 0.1, 100);
    // proj[2][2] = 0.5;
    // }
    // proj = i * glm::shear((glm::mat4)glm::orthoZO(-1., 1., -1., 1., 0.1, 100.), glm::vec3(0., 0., 0.),
    //     glm::vec2(0., cos(2) * 1.0 / tan(2)),
    //     glm::vec2(0., sin(2) * 1.0 / tan(2)),
    //     glm::vec2(0.0, 0.0));

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

void Camera::applyLightProj(int shaderProgram, glm::mat4 proj) {
    int projLoc = glGetUniformLocation(shaderProgram, "lightProj");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
}