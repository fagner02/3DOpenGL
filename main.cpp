#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <fstream>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif
#include "./include/GL/glut.h"
#include "./include/GL/glext.h"
#include "./shaders/shader.h"
#include "./include/glm/glm.hpp"
#include "./include/glm/gtc/type_ptr.hpp"
#include "./include/glm/gtx/rotate_vector.hpp"
#include "./include/GLFW/glfw3.h"
#include "./vertex_array.h"
#include "./sphere.h"
#include "./cube.h"
#include "point.h"
#include "line.h"
#include "cam.h"

using namespace std;

double width = 800, height = 600;
Camera cam(width, height);
bool down = false;
bool follow_cursor = false;
bool right_pressed = false;

struct MousePos {
    double x;
    double y;
} mousePos;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    std::cout << action;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &mousePos.x, &mousePos.y);
        down = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        down = false;
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            glfwGetCursorPos(window, &mousePos.x, &mousePos.y);
            right_pressed = true;
        } else {
            right_pressed = false;
        }
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (down || follow_cursor) {
        cam.orientation = glm::rotate(cam.orientation, glm::radians((float)((xpos - mousePos.x))), glm::vec3(0.0f, 1.0f, 0.0f));
        cam.orientation = glm::rotate(cam.orientation, glm::radians((float)(-(ypos - mousePos.y))), glm::vec3(1.0f, 0.0f, 0.0f));
        cam.view = glm::lookAt(cam.pos, cam.pos + cam.orientation, cam.up);
        mousePos.x = xpos;
        mousePos.y = ypos;
    }
    if (right_pressed) {
        cam.pos = glm::rotate(cam.pos, glm::radians((float)(-(xpos - mousePos.x))), glm::vec3(0.0f, 1.0f, 0.0f));
        cam.pos = glm::rotate(cam.pos, glm::radians((float)(-(ypos - mousePos.y))), glm::vec3(1.0f, 0.0f, 0.0f));
        cam.view = glm::lookAt(cam.pos, cam.pos + cam.orientation, cam.up);
        mousePos.x = xpos;
        mousePos.y = ypos;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    cam.view = glm::translate(cam.view, glm::vec3(xoffset, yoffset, 0.0));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_LEFT) {
        cam.pos += 0.1f * glm::normalize(glm::cross(cam.orientation, cam.up));
    }
    if (key == GLFW_KEY_RIGHT) {
        cam.pos -= 0.1f * glm::normalize(glm::cross(cam.orientation, cam.up));
    }
    if (key == GLFW_KEY_DOWN) {
        cam.pos -= 0.1f * glm::normalize(glm::cross(glm::cross(cam.orientation, cam.up), cam.orientation));
    }
    if (key == GLFW_KEY_UP) {
        cam.pos += 0.1f * glm::normalize(glm::cross(glm::cross(cam.orientation, cam.up), cam.orientation));
    }
    if (key == GLFW_KEY_EQUAL) {
        cam.pos += 0.1f * glm::normalize(cam.orientation);
    }
    if (key == GLFW_KEY_MINUS) {
        cam.pos -= 0.1f * glm::normalize(cam.orientation);
    }
    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        follow_cursor = !follow_cursor;
    }
    cam.view = glm::lookAt(cam.pos, cam.pos + cam.orientation, cam.up);
}

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

int main() {
    if (!glfwInit()) {
        std::cout << "erro";
        return -1;
    }
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width, height, "Opengl Graphics", nullptr, nullptr);
    if (window == NULL) {
        cout << "erro";
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    Shader lightless("./shaders/lightless.vert", "./shaders/lightless.frag");
    Shader lightShader("./shaders/light.vert", "./shaders/light.frag");
    Shader shader("./shaders/vertex_shader.glsl", "./shaders/frag.glsl");

    glm::vec3 lightPos(0.5, 0.5, 0.5);
    Sphere sphere(0.2, glm::vec3(0.0, 0.0, -0.3));
    Cube cube2(glm::vec3(0.0, 0.0, 0.0), 0.7);
    Cube cube(lightPos, 0.05);

    VertexArray vao = VertexArray({
            -0.5f,	0.0f,	0.5f,
            -0.5f,	0.0f,	-0.5f,
            0.5f,	0.0f,	-0.5f,
            0.5f,	0.0f,	0.5f,
            -0.5f,	0.0f,	0.5f,
            -0.5f,	0.0f,	-0.5f,
            0.0f,	0.8f,	0.0f,
            -0.5f,	0.0f,	-0.5f,
            0.5f,	0.0f,	-0.5f,
            0.0f,	0.8f,	0.0f,
            0.5f,	0.0f,	-0.5f,
            0.5f,	0.0f,	0.5f,
            0.0f,	0.8f,	0.0f,
            0.5f,	0.0f,	0.5f,
            -0.5f,	0.0f,	0.5f,
            0.0f,	0.8f,	0.0f,
        }, {
            0.83f,	0.70f,	0.44f,
            0.83f,	0.70f,	0.44f,
            0.83f,	0.70f,	0.44f,
            0.83f,	0.70f,	0.44f,
            0.83f,	0.70f,	0.44f,
            0.83f,	0.70f,	0.44f,
            0.92f,	0.86f,	0.76f,
            0.83f,	0.70f,	0.44f,
            0.83f,	0.70f,	0.44f,
            0.92f,	0.86f,	0.76f,
            0.83f,	0.70f,	0.44f,
            0.83f,	0.70f,	0.44f,
            0.92f,	0.86f,	0.76f,
            0.83f,	0.70f,	0.44f,
            0.83f,	0.70f,	0.44f,
            0.92f,	0.86f,	0.76f,
        }, {
            0.0f,	-1.0f,	0.0f,
            0.0f,	-1.0f,	0.0f,
            0.0f,	-1.0f,	0.0f,
            0.0f,	-1.0f,	0.0f,
            -0.8f,	0.5f,	0.0f,
            -0.8f,	0.5f,	0.0f,
            -0.8f,	0.5f,	0.0f,
            0.0f,	0.5f,	-0.8f,
            0.0f,	0.5f,	-0.8f,
            0.0f,	0.5f,	-0.8f,
            0.8f,	0.5f,	0.0f,
            0.8f,	0.5f,	0.0f,
            0.8f,	0.5f,	0.0f,
            0.0f,	0.5f,	0.8f,
            0.0f,	0.5f,	0.8f,
            0.0f,	0.5f,	0.8f
        }, {
            0, 1, 2, // Bottom side
            0, 2, 3, // Bottom side
            4, 6, 5, // Left side
            7, 9, 8, // Non-facing side
            10, 12, 11, // Right side
            13, 15, 14 // Facing side
        });

    glm::vec3 points[] = {
        glm::vec3(-0.35, 0.35, -0.35),
        glm::vec3(-0.35, 0.35, 0.35),
        glm::vec3(0.35, 0.35, -0.35),
        glm::vec3(0.35, 0.35, 0.35)
    };

    glm::vec3 n = glm::cross(points[2] - points[1], points[0] - points[1]);

    vector<float> lineV;
    for (int i = 0; i < 4; i++) {
        for (int j = 0;j < 3;j++) {
            lineV.push_back(points[i][j]);
        }
    }
    VertexArray frontFace(
        lineV, {
            0.8,0.2,0.5,
            0.8,0.2,0.5,
            0.8,0.2,0.5,
            0.8,0.2,0.5,
        }, {
            n.x, n.y, n.z,
            n.x, n.y, n.z,
            n.x, n.y, n.z,
            n.x, n.y, n.z,
        },
        {
            0, 1, 2,
            1, 2, 3
        }
        );
    Line line(glm::vec3(0), n);
    Line line1(points[0], points[1], glm::vec3(0.8, 0.5, 0.5));
    Line line2(points[2], points[1], glm::vec3(0.0, 0.8, 0.0));

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.5, 0.2, 0.3, 1);

        shader.useShader();
        cam.applyMatrix(shader.shaderProgram);
        cam.applyLightPos(shader.shaderProgram, lightPos);
        cube.applyMatrix(shader.shaderProgram);
        cube2.bindVAO();
        glDrawElements(GL_TRIANGLES, cube2.bufferCount, GL_UNSIGNED_INT, 0);
        sphere.bindVAO();
        glDrawArrays(GL_TRIANGLES, 0, sphere.bufferCount);
        // vao.bindVAO();
        // glDrawElements(GL_TRIANGLES, vao.bufferCount, GL_UNSIGNED_INT, 0);
        frontFace.bindVAO();
        glDrawElements(GL_TRIANGLES, frontFace.bufferCount, GL_UNSIGNED_INT, 0);

        lightless.useShader();
        cam.applyMatrix(lightless.shaderProgram);

        line.bindVAO();
        glDrawArrays(GL_LINES, 0, line.bufferCount);
        line1.bindVAO();
        glDrawArrays(GL_LINES, 0, line1.bufferCount);
        line2.bindVAO();
        glDrawArrays(GL_LINES, 0, line2.bufferCount);

        lightShader.useShader();
        cam.applyMatrix(lightShader.shaderProgram);
        cube.applyMatrix(lightShader.shaderProgram);
        cube.bindVAO();
        glDrawElements(GL_TRIANGLES, cube.bufferCount, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwWaitEvents();
    }
}