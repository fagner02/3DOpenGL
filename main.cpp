#define GLM_ENABLE_EXPERIMENTAL
#include <fstream>
#include <iostream>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif

#include "./shaders/shader.h"
#include "./shapes/cube.h"
#include "./shapes/line.h"
#include "./shapes/point.h"
#include "./shapes/sphere.h"
#include "./vertex_array.h"
#include "cam.h"
#include "clip_plane.h"
#include "model.h"
#include "shapes/plane.h"
#include "shell_texture.h"
#include "texture_picking.h"

#include "include/imgui/imgui.h"
#include "include/imgui/imgui_impl_glfw.h"
#include "include/imgui/imgui_impl_opengl3.h"

#include "include/imgui/ImGuizmo.h"

#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

using namespace std;

struct ModelData {
    Texture *texture;
    ShellTexture *shellTexture;
    bool enableShellTexture;
};

double width = 1200, height = 600;
Camera cam(width, height);
bool down = false;
bool click = false;
bool gizmo_down = false;
bool follow_cursor = false;
bool right_pressed = false;
ImGuizmo::OPERATION operation;

Shader lightlessShader;

Model gizmoModel;

Picking pickingTexture;

std::vector<Model> models = std::vector<Model>();
std::vector<ModelData> modelsData = std::vector<ModelData>();
std::vector<Texture> textures = std::vector<Texture>();

Texture *defaultTexture = nullptr;

Texture *selectedTexture = nullptr;

struct MousePos {
    double x;
    double y;
} mousePos;

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {

    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (ImGuizmo::IsOver() || ImGuizmo::IsUsingAny()) {
            gizmo_down = true;
            cout << "gizmo down\n";
        }
        glfwGetCursorPos(window, &mousePos.x, &mousePos.y);
        click = true;
        down = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        gizmo_down = false;
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

static void cursor_position_callback(GLFWwindow *window, double xpos,
                                     double ypos) {
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }
    if (gizmo_down) {
        cout << "gizmo move\n";
        click = false;
        return;
    }

    if (down) {
        click = false;
    }

    if (down || follow_cursor) {
        cam.orientation = glm::rotate(
            cam.orientation, glm::radians((float)(-(xpos - mousePos.x) * 0.5)),
            cam.up);
        cam.orientation = glm::rotate(
            cam.orientation, glm::radians((float)(-(ypos - mousePos.y) * 0.5)),
            glm::cross(cam.up, cam.pos));
        cam.view = glm::lookAt(cam.pos, cam.pos + cam.orientation, cam.up);

        mousePos.x = xpos;
        mousePos.y = ypos;
    }

    if (right_pressed) {
        cam.up = glm::vec3(0, 1, 0);
        cam.pos = glm::rotate(
            cam.pos, glm::radians((float)(-(xpos - mousePos.x))), cam.up);
        cam.pos =
            glm::rotate(cam.pos, glm::radians((float)(-(ypos - mousePos.y))),
                        glm::cross(cam.up, cam.pos));
        cam.up = cam.up;
        cam.orientation =
            glm::rotate(cam.orientation,
                        glm::radians((float)(-(xpos - mousePos.x))), cam.up);
        cam.orientation = glm::rotate(
            cam.orientation, glm::radians((float)(-(ypos - mousePos.y))),
            glm::cross(cam.up, cam.pos));
        cam.view = glm::lookAt(cam.pos, cam.pos + cam.orientation, cam.up);
        cam.up =
            glm::rotate(cam.up, glm::radians((float)(-(ypos - mousePos.y))),
                        (glm::cross(cam.up, cam.pos)));
        mousePos.x = xpos;
        mousePos.y = ypos;
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }

    cam.view = glm::translate(cam.view, glm::vec3(xoffset, yoffset, 0.0));
}

void char_callback(GLFWwindow *window, unsigned int codepoint) {
    ImGui_ImplGlfw_CharCallback(window, codepoint);
}
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {

    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

    if (ImGui::GetIO().WantCaptureKeyboard) {
        return;
    }

    if (key == GLFW_KEY_LEFT) {
        cam.pos += 0.1f * glm::normalize(glm::cross(cam.orientation, cam.up));
    }
    if (key == GLFW_KEY_RIGHT) {
        cam.pos -= 0.1f * glm::normalize(glm::cross(cam.orientation, cam.up));
    }
    if (key == GLFW_KEY_DOWN) {
        cam.pos -=
            0.1f * glm::normalize(glm::cross(
                       glm::cross(cam.up, cam.orientation), cam.orientation));
    }
    if (key == GLFW_KEY_UP) {
        cam.pos +=
            0.1f * glm::normalize(glm::cross(
                       glm::cross(cam.up, cam.orientation), cam.orientation));
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

    if (key == GLFW_KEY_R) {
        operation = ImGuizmo::ROTATE;
    }
    if (key == GLFW_KEY_T) {
        operation = ImGuizmo::TRANSLATE;
    }
    if (key == GLFW_KEY_S) {
        operation = ImGuizmo::SCALE;
    }

    cam.view = glm::lookAt(cam.pos, cam.pos + cam.orientation, cam.up);
}

void window_size_callback(GLFWwindow *window, int _width, int _height) {
    if (_height == 0)
        return;
    height = _height;
    width = _width;
    cam.proj = glm::perspective(glm::radians(45.0f), (float)(width / height),
                                0.1f, 100.0f);
    glViewport(0, 0, width, height);
}

static void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void printVec3(glm::vec3 vec) {
    for (size_t i = 0; i < 3; i++) {
        std::cout << vec[i] << " ";
    }
}

void addModel(VertexArray *model) {
    pickingTexture.objs.push_back(model);
    modelsData.push_back({
        texture : defaultTexture,
        shellTexture : nullptr,
        enableShellTexture : false
    });
}

int main() {
    if (!glfwInit()) {
        std::cout << "erro";
        return -1;
    }

    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow *window =
        glfwCreateWindow(width, height, "Opengl Graphics", nullptr, nullptr);
    if (window == NULL) {
        cout << "erro";
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetCharCallback(window, char_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_CLIP_DISTANCE0);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init("#version 330");

    lightlessShader =
        Shader("./shaders/lightless.vert", "./shaders/lightless.frag");
    Shader lightShader("./shaders/light.vert", "./shaders/light.frag");
    Shader defaultShader("./shaders/default.vert", "./shaders/default.frag");
    Shader picking("./shaders/picking.vert", "./shaders/picking.frag");
    Shader textureShader("./shaders/texture.vert", "./shaders/texture.frag");
    Shader shellShader("./shaders/shell.vert", "./shaders/shell.frag");
    Shader shadowShader("./shaders/shadow.vert", "./shaders/shadow.frag");

    glm::vec3 lightPos(0.5, 0.5, 1.5);
    Sphere sphere(0.3, glm::vec3(0.0, 0.0, -0.3), 50);
    Cube cube3(glm::vec3(0.0, 0.1, -0.3), 0.2);
    Cube cube(lightPos, 0.05);
    Plane floor(glm::vec3(0.0, 0.0, -0.3), 2.5);

    glm::mat4 lightProj = glm::orthoZO<double>(-1, 1, -1, 1, 0.1, 100);

    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    const unsigned int SHADOW_WIDTH = 1048, SHADOW_HEIGHT = 1048;

    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
                 SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Camera shadowCam(SHADOW_WIDTH, SHADOW_HEIGHT, lightPos);
    shadowCam.view = glm::lookAt(lightPos, glm::vec3(0, 0, 0), shadowCam.up);

    Model ant;
    ant.loadFile("./3ds/ant.obj");

    textures.push_back(Texture("./textures/checkers.jpg"));
    Texture azulejo("./textures/azulejo.jpg");
    defaultTexture = &azulejo;
    textures.push_back(azulejo);

    pickingTexture.init(width, height);

    glm::vec3 lightColor(0.5, 0.9, 0.5);
    unsigned int selected = 0;

    addModel(&cube3);
    addModel(&floor);
    addModel(&ant);
    cout << "hiiii\n";
    static char text[1024] = "./3ds/maclaren.3DS";
    while (!glfwWindowShouldClose(window)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuizmo::BeginFrame();

        ImGui::Begin("Controls");
        ImGui::InputText("Text Area", text, IM_ARRAYSIZE(text));
        if (ImGui::Button("Load##model")) {
            std::cout << "clicked\n";
            models.push_back(Model());
            models[models.size() - 1].loadFile(text);
            addModel(&models[models.size() - 1]);
            selected = models.size();
        }

        if (selectedTexture != nullptr) {
            ImGui::BeginGroup();

            ImGui::Image((void *)(intptr_t)selectedTexture->textureId,
                         ImVec2(32, 32));

            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Text("Selected Texture");
            if (selected > 0 && selected < modelsData.size() + 1 &&
                ImGui::Button("Load")) {
                modelsData[selected - 1].texture = selectedTexture;
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
        }
        if (ImGui::Checkbox("Orthographic", &cam.isOrthographic)) {
            cam.toggleOrthographic();
        }

        if (selected > 0 && selected < modelsData.size() + 1 &&
            ImGui::Checkbox("Enable Shell Texture",
                            &modelsData[selected - 1].enableShellTexture)) {
            if (modelsData[selected - 1].shellTexture == nullptr) {
                cout << pickingTexture.objs[selected - 1]->meshes.size();
                if (pickingTexture.objs[selected - 1]->meshes.size() > 0) {
                    modelsData[selected - 1].shellTexture = new ShellTexture(
                        pickingTexture.objs[selected - 1], 50, 0.2, 70,
                        pickingTexture.objs[selected - 1]->modelMatrix,
                        pickingTexture.objs[selected - 1]->meshes);
                } else {

                    modelsData[selected - 1].shellTexture = new ShellTexture(
                        pickingTexture.objs[selected - 1], 50, 0.2, 70,
                        pickingTexture.objs[selected - 1]->modelMatrix);
                }
            }
        }

        ImGui::End();

        ImGui::Begin("Texture List");

        ImGui::BeginChild("ImageList", ImVec2(0, 300), true);

        size_t columns = 3;
        for (size_t i = 0; i < textures.size(); i++) {
            if (i > 0 && i % columns != 0) {
                ImGui::SameLine();
            }

            if (ImGui::Selectable(("##cell" + std::to_string(i)).c_str(), false,
                                  ImGuiSelectableFlags_None, ImVec2(64, 68))) {
                selectedTexture = &textures[i];
            }

            ImVec2 pos = ImGui::GetItemRectMin();
            pos.x += 4;
            pos.y += 4;
            ImGui::GetWindowDrawList()->AddImage(
                (void *)(intptr_t)textures[i].textureId, pos,
                ImVec2(pos.x + 64, pos.y + 64));
        }

        ImGui::EndChild();

        ImGui::End();

        ImGui::EndFrame();

        pickingTexture.enableWriting();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        picking.useShader();
        cam.applyMatrix(picking.shaderProgram);
        for (size_t i = 1; i <= modelsData.size(); i++) {
            pickingTexture.applyIndex(picking.shaderProgram, i);
            pickingTexture.objs[i - 1]->draw(picking.shaderProgram);
        }

        pickingTexture.disableWriting();

        if (!down && click) {
            PixelInfo pixel =
                pickingTexture.ReadPixel(mousePos.x, height - mousePos.y);
            selected = pixel.ObjectID;
            click = false;
            std::cout << pixel.ObjectID << "\n";
        }

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        shadowShader.useShader();

        for (int i = 0; i < modelsData.size(); i++) {
            shadowCam.applyMatrix(shadowShader.shaderProgram);
            pickingTexture.objs[i]->draw(shadowShader.shaderProgram);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.5, 0.2, 0.3, 1);

        defaultShader.useShader();
        for (int i = 0; i < modelsData.size(); i++) {
            if (modelsData[i].enableShellTexture)
                continue;
            if (selected == i + 1) {
                lightColor = glm::vec3(1, 0.5, 0.5);
            } else {
                lightColor = glm::vec3(1, 1, 1);
            }

            int texLoc =
                glGetUniformLocation(defaultShader.shaderProgram, "depthMap");
            glUniform1i(texLoc, 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, depthMap);

            if (modelsData[i].texture != nullptr) {
                int useTex =
                    glGetUniformLocation(defaultShader.shaderProgram, "useTex");
                glUniform1i(useTex, 1);
                modelsData[i].texture->bindTexture(defaultShader.shaderProgram);
            }
            cam.applyMatrix(defaultShader.shaderProgram);
            cam.applyLightPos(defaultShader.shaderProgram, lightPos);
            cam.applyLightColor(defaultShader.shaderProgram, lightColor);
            cam.applyLightProj(defaultShader.shaderProgram,
                               shadowCam.proj * shadowCam.view);
            pickingTexture.objs[i]->draw(defaultShader.shaderProgram);
        }
        for (int i = 0; i < modelsData.size(); i++) {
            if (!modelsData[i].enableShellTexture ||
                modelsData[i].shellTexture == nullptr) {
                continue;
            }
            shellShader.useShader();
            cam.applyMatrix(shellShader.shaderProgram);
            cam.applyLightPos(shellShader.shaderProgram, lightPos);
            cam.applyLightColor(shellShader.shaderProgram, lightColor);
            modelsData[i].shellTexture->draw(shellShader.shaderProgram);
        }
        lightColor = glm::vec3(1, 1, 1);

        lightShader.useShader();
        cam.applyMatrix(lightShader.shaderProgram);
        cube.draw(lightShader.shaderProgram);

        lightlessShader.useShader();
        cam.applyMatrix(lightlessShader.shaderProgram);

        textureShader.useShader();
        cam.applyMatrix(shellShader.shaderProgram);
        cam.applyLightPos(shellShader.shaderProgram, lightPos);

        if (selected > 0 && selected < modelsData.size() + 1) {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());
            ImGuizmo::SetRect(0, 0, (float)width, (float)height);

            bool changed = ImGuizmo::Manipulate(
                glm::value_ptr(cam.view), glm::value_ptr(cam.proj), operation,
                ImGuizmo::WORLD,
                glm::value_ptr(pickingTexture.objs[selected - 1]->modelMatrix));
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}