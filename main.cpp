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
#include "./shapes/sphere.h"
#include "./shapes/cube.h"
#include "./shapes/point.h"
#include "./shapes/line.h"
#include "cam.h"
#include "texture_picking.h"
#include "model.h"
#include "shapes/plane.h"
#include "clip_plane.h"
#include "shell_texture.h"

using namespace std;

double width = 800, height = 600;
Camera cam(width, height);
bool down = false;
bool click = false;
bool follow_cursor = false;
bool right_pressed = false;

struct MousePos {
    double x;
    double y;
} mousePos;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &mousePos.x, &mousePos.y);
        click = true;
        down = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        down = false;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            glfwGetCursorPos(window, &mousePos.x, &mousePos.y);
            right_pressed = true;
        } else {
            right_pressed = false;
        }
    }
}
Cube* cube2 = nullptr;

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (down) {
        click = false;
    }

    if (down || follow_cursor) {
        cam.orientation = glm::rotate(cam.orientation, glm::radians((float)(-(xpos - mousePos.x) * 0.5)), cam.up);
        cam.orientation = glm::rotate(cam.orientation, glm::radians((float)(-(ypos - mousePos.y) * 0.5)), glm::cross(cam.up, cam.pos));
        cam.view = glm::lookAt(cam.pos, cam.pos + cam.orientation, cam.up);

        mousePos.x = xpos;
        mousePos.y = ypos;
    }

    if (right_pressed) {
        cam.up = glm::vec3(0, 1, 0);
        cam.pos = glm::rotate(cam.pos, glm::radians((float)(-(xpos - mousePos.x))), cam.up);
        cam.pos = glm::rotate(cam.pos, glm::radians((float)(-(ypos - mousePos.y))), glm::cross(cam.up, cam.pos));
        cam.up = cam.up;
        cam.orientation = glm::rotate(cam.orientation, glm::radians((float)(-(xpos - mousePos.x))), cam.up);
        cam.orientation = glm::rotate(cam.orientation, glm::radians((float)(-(ypos - mousePos.y))), glm::cross(cam.up, cam.pos));
        cam.view = glm::lookAt(cam.pos, cam.pos + cam.orientation, cam.up);
        cam.up = glm::rotate(cam.up, glm::radians((float)(-(ypos - mousePos.y))), (glm::cross(cam.up, cam.pos)));
        mousePos.x = xpos;
        mousePos.y = ypos;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    cam.view = glm::translate(cam.view, glm::vec3(xoffset, yoffset, 0.0));
}
ClipPlane clipPlane;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_LEFT) {
        cam.pos += 0.1f * glm::normalize(glm::cross(cam.orientation, cam.up));
        // cam.up += 0.1f * glm::normalize(glm::cross(cam.orientation, cam.up));
        // cam.orientation -= 0.1f * glm::normalize(glm::cross(cam.orientation, cam.up));
    }
    if (key == GLFW_KEY_RIGHT) {
        cam.pos -= 0.1f * glm::normalize(glm::cross(cam.orientation, cam.up));
        // cam.up -= 0.1f * glm::normalize(glm::cross(cam.orientation, cam.up));
        // cam.orientation += 0.1f * glm::normalize(glm::cross(cam.orientation, cam.up));
    }
    if (key == GLFW_KEY_DOWN) {
        cam.pos -= 0.1f * glm::normalize(glm::cross(glm::cross(cam.up, cam.orientation), cam.orientation));
        // cam.up -= 0.1f * glm::normalize(cam.up);
    }
    if (key == GLFW_KEY_UP) {
        cam.pos += 0.1f * glm::normalize(glm::cross(glm::cross(cam.up, cam.orientation), cam.orientation));
        // cam.up += 0.1f * glm::normalize(cam.up);
    }
    if (key == GLFW_KEY_EQUAL) {
        cam.pos += 0.1f * glm::normalize(cam.orientation);
    }
    if (key == GLFW_KEY_MINUS) {
        cam.pos -= 0.1f * glm::normalize(cam.orientation);
    }
    // cam.up = glm::cross(cam.pos, cam.orientation);
    if (key == GLFW_KEY_A) {
        clipPlane.clipAngle += 0.1;
    }
    if (key == GLFW_KEY_D) {
        clipPlane.clipAngle -= 0.1;
    }
    // cam.up = glm::abs(glm::cross(glm::abs(glm::cross(glm::vec3(0.0, 1.0, 0.0), cam.pos)), cam.pos));
    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        follow_cursor = !follow_cursor;
    }
    cam.view = glm::lookAt(cam.pos, cam.pos + cam.orientation, cam.up);
    clipPlane.calculateClipPlane();
}

void window_size_callback(GLFWwindow* window, int _width, int _height) {
    if (_height == 0) return;
    height = _height;
    width = _width;
    cam.proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);
    glViewport(0, 0, width, height);
}

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void printVec3(glm::vec3 vec) {
    for (size_t i = 0; i < 3; i++) {
        std::cout << vec[i] << " ";
    }
}

int main() {
    // glm::mat4 mat = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 mat = glm::identity<glm::mat4>();
    // glm::mat4 mat = glm::mat4(
    //     0, -1, 0, 0,
    //     1, 0, 0, 0,
    //     0, 0, 1, 0,
    //     0, 0, 0, 1) *
    //     glm::mat4(
    //         1, 0, 0, 3,
    //         0, 1, 0, 0,
    //         0, 0, 1, 0,
    //         0, 0, 0, 1
    //     );

    // glm::vec3 eye = glm::vec3(0, 1, 0);
    // glm::vec3 center = glm::vec3(1, 2, 1);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4;j++) {
            std::cout << std::fixed << mat[i][j] << ", ";
        }
        std::cout << "\n";
    }
    glm::vec4 res = mat * glm::vec4(1, 1, 1, 1);
    std::cout << res[0] << ", " << res[1] << ", " << res[2] << ", " << res[3];

    if (!glfwInit()) {
        std::cout << "erro";
        return -1;
    }

    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(width, height, "Opengl Graphics", nullptr, nullptr);
    if (window == NULL) {
        cout << "erro";
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_CLIP_DISTANCE0);

    Shader lightless("./shaders/lightless.vert", "./shaders/lightless.frag");
    Shader lightShader("./shaders/light.vert", "./shaders/light.frag");
    Shader defaultShader("./shaders/default.vert", "./shaders/default.frag");
    Shader picking("./shaders/picking.vert", "./shaders/picking.frag");
    Shader textureShader("./shaders/texture.vert", "./shaders/texture.frag");
    Shader shellShader("./shaders/shell.vert", "./shaders/shell.frag");

    glm::vec3 lightPos(0.5, 0.5, 1.5);
    Sphere sphere(0.3, glm::vec3(0.0, 0.0, -0.3), 50);
    Cube cube3(glm::vec3(0.0, 0.0, -0.3), 2.5);
    Cube cube(lightPos, 0.05);
    Plane floor(glm::vec3(0.0, 0.0, -0.3), 2.5);

    Model model;
    model.loadFile("./3ds/maclaren.3ds");
    Model ant;
    ant.loadFile("./3ds/ant2.obj");

    glm::mat4 modelMatrix = glm::rotate(glm::scale(glm::identity<glm::mat4>(), glm::vec3(2000.0, 2000.0, 2000.0)), (float)glm::radians(-90.0), glm::vec3(1.0, 0.0, 0.0)) +
        glm::translate(glm::vec3(0, 0.5, 0));
    //ant.saveFile("./3ds/ant2.obj", modelMatrix);
    Texture azulejo("./textures/refri.bmp");
    Picking pickingTexture;
    pickingTexture.init(width, height);
    glm::vec3 lightColor(0.5, 0.9, 0.5);
    unsigned int selected = 0;

    // ShellTexture shellTexture(model.getModelBuffers("./3ds/maclaren.3ds"), 50, 0.2, 70, model.meshes);
    ShellTexture shellTexture(floor.calcPlaneBuffers(glm::vec3(0.0, 0.0, -0.3), 2.5), 50, 0.2, 70);
    // pickingTexture.objs.push_back(&sphere);
    // pickingTexture.objs.push_back(&cube3);
    // pickingTexture.objs.push_back(&floor);
    pickingTexture.objs.push_back(&ant);

    cube2 = new Cube(glm::vec3(0.0, 0.0, 0.0), 0.7);
    while (!glfwWindowShouldClose(window)) {
        pickingTexture.enableWriting();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        picking.useShader();
        cam.applyMatrix(picking.shaderProgram);
        clipPlane.applyClipPlane(picking.shaderProgram);
        for (size_t i = 1; i <= pickingTexture.objs.size(); i++) {
            pickingTexture.applyIndex(picking.shaderProgram, i);
            pickingTexture.objs[i - 1]->draw(picking.shaderProgram);
        }

        pickingTexture.disableWriting();

        if (!down && click) {
            PixelInfo pixel = pickingTexture.ReadPixel(mousePos.x, height - mousePos.y);
            selected = pixel.ObjectID;
            click = false;
            std::cout << pixel.ObjectID << "\n";
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.5, 0.2, 0.3, 1);

        defaultShader.useShader();

        for (int i = 0; i < pickingTexture.objs.size();i++) {
            if (selected == i + 1) {
                lightColor = glm::vec3(1, 0.5, 0.5);
            } else {
                lightColor = glm::vec3(1, 1, 1);
            }

            cam.applyMatrix(defaultShader.shaderProgram);
            cam.applyLightPos(defaultShader.shaderProgram, lightPos);
            cam.applyLightColor(defaultShader.shaderProgram, lightColor);
            clipPlane.applyClipPlane(defaultShader.shaderProgram);
            pickingTexture.objs[i]->draw(defaultShader.shaderProgram);
        }
        lightColor = glm::vec3(1, 1, 1);

        shellShader.useShader();
        cam.applyMatrix(shellShader.shaderProgram);
        cam.applyLightPos(shellShader.shaderProgram, lightPos);
        cam.applyLightColor(shellShader.shaderProgram, lightColor);
        shellTexture.draw(shellShader.shaderProgram);

        lightShader.useShader();
        cam.applyMatrix(lightShader.shaderProgram);
        cube.draw(lightShader.shaderProgram);

        lightless.useShader();
        cam.applyMatrix(lightless.shaderProgram);

        glLineWidth(9);

        Line line(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
        line.draw(lightless.shaderProgram);
        Line line1(glm::vec3(0, 0, 0), glm::vec3(0.5, 1, -0.5));
        line1.draw(lightless.shaderProgram);
        Line line2(glm::vec3(0, 0, -1), glm::vec3(0.5, 1, -0.5));
        line2.draw(lightless.shaderProgram);
        Line line3(glm::vec3(0, 0, 0), glm::vec3(-1, 0.5, 0));
        line3.draw(lightless.shaderProgram);

        textureShader.useShader();
        azulejo.bindTexture();
        cam.applyMatrix(shellShader.shaderProgram);
        cam.applyLightPos(shellShader.shaderProgram, lightPos);

        glfwSwapBuffers(window);
        glfwWaitEvents();
    }
}