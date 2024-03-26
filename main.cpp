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
        // cam.pos = glm::rotate(cam.pos, glm::radians((float)((xpos - mousePos.x) * 0.5)), glm::vec3(0.0f, 1.0f, 0.0f));
        // cam.pos = glm::rotate(cam.pos, glm::radians((float)((ypos - mousePos.y) * 0.5)), glm::vec3(1.0f, 0.0f, 0.0f));
        cam.orientation = glm::rotate(cam.orientation, glm::radians((float)(-(xpos - mousePos.x) * 0.5)), glm::vec3(0.0f, 1.0f, 0.0f));
        cam.orientation = glm::rotate(cam.orientation, glm::radians((float)(-(ypos - mousePos.y) * 0.5)), glm::cross(cam.up, cam.pos));
        cam.view = glm::lookAt(cam.pos, cam.pos + cam.orientation, cam.up);
        mousePos.x = xpos;
        mousePos.y = ypos;
    }

    if (right_pressed) {
        // cam.pos = glm::rotate(cam.pos, glm::radians((float)(-(xpos - mousePos.x))), glm::cross(glm::cross(cam.up, cam.pos), cam.pos));
        cam.pos = glm::rotate(cam.pos, glm::radians((float)(-(ypos - mousePos.y))), glm::cross(cam.up, cam.pos));
        // cam.orientation = glm::rotate(cam.orientation, glm::radians((float)(-(xpos - mousePos.x))), glm::vec3(0.0f, 1.0f, 0.0f));
        cam.orientation = glm::rotate(cam.orientation, glm::radians((float)(-(ypos - mousePos.y))), glm::cross(cam.up, cam.pos));
        cam.view = glm::lookAt(cam.pos, cam.pos + cam.orientation, cam.up);
        mousePos.x = xpos;
        mousePos.y = ypos;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    cam.view = glm::translate(cam.view, glm::vec3(xoffset, yoffset, 0.0));
}
// ClipPlane clipPlane;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_LEFT) {
        cam.pos += 0.1f * glm::normalize(glm::cross(cam.orientation, cam.up));
        // cam.orientation -= 0.1f * glm::normalize(glm::cross(cam.orientation, cam.up));
    }
    if (key == GLFW_KEY_RIGHT) {
        cam.pos -= 0.1f * glm::normalize(glm::cross(cam.orientation, cam.up));
        // cam.orientation += 0.1f * glm::normalize(glm::cross(cam.orientation, cam.up));
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
    // if (key == GLFW_KEY_A) {
    //     clipPlane.clipAngle += 0.1;
    // }
    // if (key == GLFW_KEY_D) {
    //     clipPlane.clipAngle -= 0.1;
    // }

    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        follow_cursor = !follow_cursor;
    }
    cam.view = glm::lookAt(cam.pos, cam.pos + cam.orientation, cam.up);
    // clipPlane.calculateClipPlane();
}

void window_size_callback(GLFWwindow* window, int _width, int _height) {
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

glm::vec3 normalize(glm::vec3 a, glm::vec3 b, double length) {
    glm::vec3 d = b - a;
    glm::vec3 d2 = d * d;
    double l = sqrt(d2.x + d2.y + d2.z);
    d = d * (float)(length / l);
    return a + d;
}

int main() {
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

    glm::vec3 lightPos(0.5, 0.5, 0.5);
    Sphere sphere(0.2, glm::vec3(0.0, 0.0, -0.3), 4);
    Cube cube(lightPos, 0.05);
    Plane plane(glm::vec3(0.0), 0.5);
    // Model model;
    // model.loadFile("./3ds/maclaren.3DS");
    // std::vector<Plane> shells;
    // int shellNum = 100;
    // float shellHeight = 0.1;
    // for (size_t i = 0; i < shellNum; i++) {
    //     shells.push_back(Plane(glm::vec3(0.0, ((float)i / shellNum) * shellHeight, 0.0), 1.0));
    // }

    // Texture azulejo("./textures/azulejo.jpg");
    // Picking pickingTexture;
    // pickingTexture.init(width, height);
    glm::vec3 lightColor(1.0f);
    unsigned int selected = 0;

    // clipPlane.calculateClipPlane();

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    for (size_t i = 0; i < sphere.vertices.size(); i++) {
        glm::vec3 normalPoint = normalize(glm::vec3(0.0) + sphere.pos, sphere.normals[i] + sphere.pos, 0.2 / 2.0);

        glm::vec3 newPoint = normalize(glm::vec3(0.0) + sphere.pos, sphere.normals[i] + sphere.pos, 0.2 / 2.0 + 0.05) - normalPoint;

        vertices.push_back(glm::vec3(0.0) + sphere.pos);
        vertices.push_back(sphere.normals[i] + sphere.pos);
        vertices.push_back(glm::vec3(0.0) + sphere.pos);
        vertices.push_back(normalPoint);
        vertices.push_back(sphere.pos);
        vertices.push_back(sphere.vertices[i]);
        vertices.push_back(sphere.vertices[i]);
        vertices.push_back(sphere.vertices[i] + newPoint);
        colors.push_back(glm::vec3(0.2, 0.2, 0.8));
        colors.push_back(colors.back() * 0.2f);
        colors.push_back(glm::vec3(0.2, 0.8, 0.2));
        colors.push_back(colors.back() * 0.2f);
        colors.push_back(glm::vec3(0.9, 0.8, 0.9));
        colors.push_back(colors.back() * 0.2f);
        colors.push_back(glm::vec3(0.9, 0.0, 0.0));
        colors.push_back(colors.back() * 0.2f);
    }
    Lines lines2(vertices, colors);
    printVec3(sphere.vertices[0]);
    std::cout << "\n";
    printVec3(sphere.normals[0] + sphere.vertices[0]);
    std::cout << "\n";
    std::cout << glm::dot(sphere.normals[0], sphere.vertices[0]);
    cube2 = new Cube(glm::vec3(0.0, 0.0, 0.0), 0.7);
    while (!glfwWindowShouldClose(window)) {
        // if (inn) continue;
        // pickingTexture.enableWriting();

        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // picking.useShader();

        // cam.applyMatrix(picking.shaderProgram);

        // pickingTexture.disableWriting();

        // PixelInfo pixel = pickingTexture.ReadPixel(mousePos.x, height - mousePos.y, 1, 1);

        // if (!down && click) {
        //     // std::cout << pixel.ObjectID << "\n";
        //     // selected = pixel.ObjectID;
        //     click = false;
        // }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // pickingTexture.applyIndex(picking.shaderProgram, 1);
        // glfwGetCursorPos(window, &mousePos.x, &mousePos.y);
        glClearColor(0.5, 0.2, 0.3, 1);

        defaultShader.useShader();

        // azulejo.bindTexture();
        cam.applyMatrix(defaultShader.shaderProgram);
        cam.applyLightPos(defaultShader.shaderProgram, lightPos);

        if (selected == 1) {
            lightColor = glm::vec3(0.7, 0.5, 0.5);
        } else {
            lightColor = glm::vec3(1.0, 0.5, 0.5);
        }
        cam.applyLightColor(defaultShader.shaderProgram, lightColor);

        plane.draw(defaultShader.shaderProgram);
        cube2->draw(defaultShader.shaderProgram);
        // sphere.draw(defaultShader.shaderProgram);
        // glm::vec3 planeNormal(1.0, -1.0, 1.0);
        // planeNormal = glm::rotate(planeNormal, clipPlane.clipAngle, glm::vec3(0.0, 0.0, 1.0));

        // glm::vec3 planePoint(0.0, clipPlane.planeHeight, 0.0);
        //std::cout << -glm::dot(planeNormal, planePoint) << "\n";
        // clipPlane.applyClipPlane(defaultShader.shaderProgram);
        // cube2.applyMatrix(defaultShader.shaderProgram);
        // cube2.bindVAO();
        // glDrawElements(GL_TRIANGLES, cube2.bufferCount, GL_UNSIGNED_INT, 0);

        lightShader.useShader();
        cam.applyMatrix(lightShader.shaderProgram);
        cube.draw(lightShader.shaderProgram);
        // pixel.Print();

        lightless.useShader();
        cam.applyMatrix(lightless.shaderProgram);
        // for (size_t i = 0; i < lines.size(); i++) {
        //     // lines[i].draw(lightless.shaderProgram);
        //     // lines1[i].draw(lightless.shaderProgram);
        //     // cout << "7fgyuinom";
        //     // lines2[i].draw(lightless.shaderProgram);
        // }
        glLineWidth(9);
        // lines2.draw(lightless.shaderProgram);

        Line line(glm::vec3(0.0), cam.orientation);
        line.draw(lightless.shaderProgram);

        // shellShader.useShader();
        // cam.applyMatrix(shellShader.shaderProgram);
        // cam.applyLightPos(shellShader.shaderProgram, lightPos);

        // if (selected == 1) {
        //     lightColor = glm::vec3(0.7, 0.5, 0.5);
        // } else {
        //     lightColor = glm::vec3(1.0, 0.5, 0.5);
        // }
        // cam.applyLightColor(shellShader.shaderProgram, lightColor);

        // model.applyMatrix(defaultShader.shaderProgram);
        // model.draw();
        // unsigned int dloc = glGetUniformLocation(shellShader.shaderProgram, "divisions");
        // glUniform1f(dloc, 70);
        // unsigned int uloc = glGetUniformLocation(shellShader.shaderProgram, "shellNum");
        // glUniform1f(uloc, shellNum);
        // unsigned int loc = glGetUniformLocation(shellShader.shaderProgram, "shellIndex");
        // for (size_t i = 0; i < shellNum; i++) {
        //     glUniform1f(loc, i);
        //     // shells[i].modelMatrix = glm::scale();
        //     shells[i].applyMatrix(shellShader.shaderProgram);
        //     shells[i].bindVAO();
        //     glDrawElements(GL_TRIANGLES, shells[i].bufferCount, GL_UNSIGNED_INT, 0);
        // }

        // vao.bindVAO();
    // glDrawElements(GL_TRIANGLES, vao.bufferCount, GL_UNSIGNED_INT, 0);
    // frontFace.bindVAO();
    // glDrawElements(GL_TRIANGLES, frontFace.bufferCount, GL_UNSIGNED_INT, 0);

    // lightless.useShader();
    // cam.applyMatrix(lightless.shaderProgram);

    // line.bindVAO();
    // glDrawArrays(GL_LINES, 0, line.bufferCount);
    // line1.bindVAO();
    // glDrawArrays(GL_LINES, 0, line1.bufferCount);
    // line2.bindVAO();
    // glDrawArrays(GL_LINES, 0, line2.bufferCount);



        // textureShader.useShader();


        glfwSwapBuffers(window);
        glfwWaitEvents();
    }
}