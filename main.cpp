#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <fstream>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif
#include "./gui_glut/gui.h"
#include "./include/GL/glext.h"
#include "./bib/Vetor3D.h"
#include "./shaders/shader.h"
#include "./include/glm/glm.hpp"
#include "./include/glm/gtc/type_ptr.hpp"
#include "./include/glm/gtx/rotate_vector.hpp"
#include "./include/GLFW/glfw3.h"
#include "./vertex_array.h"
#include "./sphere.h"


using namespace std;

Vetor3D posEsfera = Vetor3D(2,1,0);
float raioEsfera = 0.2;
float vz = -2;
float vy = -0.5;

void desenha(Shader* shader) {

    GUI::setLight(0, 0,2,0, true,false);

    // // //referencia
    // GUI::drawOrigin(0.5);
    // // //GUI::drawOriginAL(5);
    // GUI::setColor(0.8,0.0,0.0, 1,true);
    // GUI::drawFloor(5,5);

    // GUI::setColor(0.0,0.8,0.0, 1,true);
    // GUI::drawSphere(posEsfera.x,posEsfera.y,posEsfera.z, raioEsfera);
    // posEsfera.x -= 0.001;
    //raioEsfera += 0.001;

    //GUI::drawBox(-1,-1,-1, 1,1,1, true);

    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    GLuint vbo[4] = { -1,-1,-1,-1 };
    GLuint vao[4] = { -1,-1,-1,-1 };

  

    const float vao_pos[] =
    {
//   x      y     z
    -0.5, -0.5, -0.5,
    0, 0.5, 0,
    0.5,-0.5,-0.5,

    0.5, -0.5, -0.5,
    0, 0.5, 0,
    0.5, -0.5, 0.5,

    0.5, 0.5, 0.0f,
    0.5, -0.5, 0,
    };
const float vao_col[]=
    {
//      r   g    b
     1.0f,0.0f,0.0f,
     0.0f,1.0f,0.0f,
     0.0f,0.0f,1.0f,
     0.0f,0.0f,0.0f,
    };

    glDisable(GL_CULL_FACE);
    glGenVertexArrays(4,vao);
    glGenBuffers(4, vbo);
    
    glBindVertexArray(vao[0]);
        glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vao_pos),vao_pos,GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

        glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vao_col),vao_col,GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // glutSwapBuffers();

    // glClearColor(0, 0, 0, 1);
    // glClear(GL_COLOR_BUFFER_BIT);
    // glUseProgram(shaderProgram);
    shader->useShader();
    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 view = glm::mat4(1.0);
    glm::mat4 proj = glm::mat4(1.0);

    proj = glm::perspective(glm::radians(5.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    view = glm::translate(view, glm::vec3(0.0, vy, vz));

    int modelLoc = glGetUniformLocation(shader->shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    int projLoc = glGetUniformLocation(shader->shaderProgram, "proj");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

    int viewLoc = glGetUniformLocation(shader->shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    glBindVertexArray(vao[0]);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    shader->deleteShader();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // glDeleteVertexArrays(4,vao);
    // glDeleteBuffers(4, vbo);
    
    //  GLuint vbo[4] = { -1,-1,-1,-1 };
    // GLuint vao[4] = { -1,-1,-1,-1 };

//     const float vao_pos1[]=
//     {
// //   x      y     z
//     1, 1, 1,
//     0, 1, 1,
//     1, 0, 1.0f,
//     };
// const float vao_col1[]=
//     {
// //      r   g    b
//      1.0f,0.0f,0.0f,
//      0.0f,1.0f,0.0f,
//      0.0f,0.0f,1.0f,
//      0.0f,0.0f,1.0f,
// };

//     glDisable(GL_CULL_FACE);
//     glGenVertexArrays(4,vao);
//     glGenBuffers(4,vbo);
//     for (size_t i = 0; i < 4; i++)
//     {
//         // cout << vao[i] << " ";
//     }
    
//     glBindVertexArray(vao[0]);
//         glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
//         glBufferData(GL_ARRAY_BUFFER,sizeof(vao_pos1),vao_pos1,GL_STATIC_DRAW);
//         glEnableVertexAttribArray(0);
//         glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

//         glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
//         glBufferData(GL_ARRAY_BUFFER,sizeof(vao_col1),vao_col1,GL_STATIC_DRAW);
//         glEnableVertexAttribArray(1);
//         glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

//     glDisableVertexAttribArray(0);
//     glDisableVertexAttribArray(1);
//     glBindVertexArray(0);
//     glBindBuffer(GL_ARRAY_BUFFER, 0);

//     glBindVertexArray(vao[0]);
//     glEnableVertexAttribArray(0);
//     glEnableVertexAttribArray(1);
//     glDrawArrays(GL_TRIANGLES,0,4);
//     glBindVertexArray(0);

    glDeleteVertexArrays(4,vao);
    glDeleteBuffers(4,vbo);
    //frontal
    // glBegin(GL_POLYGON);
    //     glNormal3f(0,0,1);
    //     glVertex3f(0,0,1);
    //     glVertex3f(1,0,1);
    //     glVertex3f(1,1,1);
    //     glVertex3f(0,1,1);
    // glEnd();
    // //direita
    // glBegin(GL_POLYGON);
    //     glNormal3f(1,0,0);
    //     glVertex3f(1,0,0);
    //     glVertex3f(1,1,0);
    //     glVertex3f(1,1,1);
    //     glVertex3f(1,0,1);
    // glEnd();
    // //esquerda
    // glBegin(GL_POLYGON);
    //     glNormal3f(-1,0,0);
    //     glVertex3f(0,0,1);
    //     glVertex3f(0,1,1);
    //     glVertex3f(0,1,0);
    //     glVertex3f(0,0,0);
    // glEnd();
    // //traseira
    // glBegin(GL_POLYGON);
    //     glNormal3f(0,0,-1);
    //     glVertex3f(0,0,0);
    //     glVertex3f(0,1,0);
    //     glVertex3f(1,1,0);
    //     glVertex3f(1,0,0);
    // glEnd();
    // //superior
    // glBegin(GL_POLYGON);
    //     glNormal3f(0,1,0);
    //     glVertex3f(1,1,0);
    //     glVertex3f(0,1,0);
    //     glVertex3f(0, 1, 1);
    //     glVertex3f(1,1,1);
    //     glEnd();
    // //inferior
    // glBegin(GL_POLYGON);
    //     glNormal3f(0,-1,0);
    //     glVertex3f(0,0,0);
    //     glVertex3f(1,0,0);
    //     glVertex3f(1,0,1);
    //     glVertex3f(0,0,1);
    // glEnd();
    // //teto esquerdo
    // glBegin(GL_POLYGON);
    //     glNormal3f(0,1,0);
    //     glVertex3f(1,1,0);
    //     glVertex3f(0,1,0);
    //     glVertex3f(0.5,1.5,0.5);
    // glEnd();
    // //teto direito
    // glBegin(GL_POLYGON);
    //     glNormal3f(-1,1,0);
    //     glVertex3f(0,1,0);
    //     glVertex3f(0,1,1);
    //     glVertex3f(0.5,1.5,0.5);
    // glEnd();
    // Vetor3D n1 =  Vetor3D(0, 0, -1) ^ Vetor3D(-0.5f, 0.5f, -0.5f);

    // glBegin(GL_POLYGON);
    //     glNormal3f(n1.x, n1.y, n1.z);
    //     glVertex3f(1, 1, 1);
    //     glVertex3f(1, 1, 0);
    //     glVertex3f(0.5, 1.5, 0.5);
        
    // glEnd();
    
    // glBegin(GL_POLYGON);
    //     glNormal3f(n1.x, n1.y, n1.z);
    //     glVertex3f(0,1,1);
    //     glVertex3f(1,1,1);
    //     glVertex3f(0.5,1.5,0.5);
    // glEnd();
    

    // //interacao com o mouse
    // posEsfera.x += glutGUI::drx;
    // posEsfera.y += glutGUI::dry;
    // posEsfera.z += -0.1*glutGUI::dlrx;
    // raioEsfera += 0.01*glutGUI::dlx;

    GUI::displayEnd();
}



double width, height;
bool down = false;
bool right_pressed = false;
glm::mat4 view = glm::mat4(1.0);
glm::vec3 position (0.0f, 0.0f, 2.0f);
glm::vec3 orientation(0.0f, 0.0f, -1.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);

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
    if (down) {
        orientation = glm::rotate(orientation, glm::radians((float)(-(xpos - mousePos.x))), glm::vec3(0.0f, 1.0f, 0.0f));
        orientation = glm::rotate(orientation, glm::radians((float)(-(ypos - mousePos.y))), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::lookAt(position, position + orientation, up);
        mousePos.x = xpos;
        mousePos.y = ypos;
    }
    if (right_pressed) {
        position = glm::rotate(position, glm::radians((float)(-(xpos - mousePos.x))), glm::normalize(orientation));
        position = glm::rotate(position, glm::radians((float)(-(ypos - mousePos.y))), glm::normalize(orientation));
        view = glm::lookAt(position, position + orientation, up);
        mousePos.x = xpos;
        mousePos.y = ypos;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    view = glm::translate(view, glm::vec3(xoffset, yoffset, 0.0));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_LEFT) {
        position += 0.1f * glm::normalize(glm::cross(orientation, up));
    }
    if (key == GLFW_KEY_RIGHT) {
        position -= 0.1f *glm::normalize(glm::cross(orientation, up));
    }
    if (key == GLFW_KEY_DOWN) {
        position -= 0.1f *glm::normalize(glm::cross(glm::cross(orientation, up), orientation));
    }
    if (key == GLFW_KEY_UP) {
        position += 0.1f *glm::normalize(glm::cross(glm::cross(orientation, up), orientation));
    }
    if (key == GLFW_KEY_EQUAL) {
        position += 0.1f *glm::normalize(orientation);
    }
    if (key == GLFW_KEY_MINUS) {
        position -= 0.1f *glm::normalize(orientation);
    }
    view = glm::lookAt(position, position + orientation, up);
}

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

int main() {
    width = 800;
    height = 600;
    if (!glfwInit()) {
        std::cout << "erro";
        return -1;
    }
    glfwSetErrorCallback(error_callback);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
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

    Shader shader("./shaders/vertex_shader.glsl", "./shaders/frag.glsl");
    
    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 proj = glm::mat4(1.0);

    proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);
    view = glm::lookAt(position, position + orientation, up);
    
    int modelLoc = glGetUniformLocation(shader.shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    int projLoc = glGetUniformLocation(shader.shaderProgram, "proj");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

    int viewLoc = glGetUniformLocation(shader.shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    Sphere sphere(0.5);
    
    VertexArray vao = VertexArray({
        //   x      y     z
            -0.5, -0.5, -0.5,
            0, 0.5, 0,
            0.5,-0.5,-0.5,

            0.5, -0.5, -0.5,
            0, 0.5, 0,
            0.5, -0.5, 0.5
        }, {
            //      r   g    b
                 1.0f,0.0f,0.0f,
                 0.0f,1.0f,0.0f,
                 0.0f,0.0f,1.0f,
                 0.0f,0.0f,0.0f,
                 0.0f,0.0f,1.0f,
                 0.0f,0.0f,0.0f,
        }, {
            //      r   g    b
                 1.0f,0.0f,0.0f,
                 0.0f,1.0f,0.0f,
                 0.0f,0.0f,1.0f,
                 0.0f,0.0f,0.0f,
                 0.0f,0.0f,1.0f,
                 0.0f,0.0f,0.0f,
        });

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5, 0.2, 0.3, 1);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    
        shader.useShader();
        
        int modelLoc = glGetUniformLocation(shader.shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int projLoc = glGetUniformLocation(shader.shaderProgram, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        int viewLoc = glGetUniformLocation(shader.shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        sphere.bindVAO();
        glDrawElements(GL_TRIANGLES, sphere.bufferCount, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    // glutCreateWindow("wTitle");

    // glClearColor(0.6,0.6,0.0,1.0); //define a cor para limpar a imagem (cor de fundo)
    // //glClearColor(1.0,1.0,1.0,1.0); //define a cor para limpar a imagem (cor de fundo)

    // glEnable(GL_LIGHTING); //habilita iluminacao (chamada no setLight)
    // //glEnable(GL_COLOR_MATERIAL);
    // glEnable(GL_CULL_FACE); //nao mostra as faces dos dois "lados" (frente [anti-horaria] e tras [horaria])
    // //glCullFace(GL_BACK); //define qual "lado" da face nao sera mostrado (padrao = nao mostrar a de tras)
    // glEnable(GL_NORMALIZE); //mantem a qualidade da iluminacao mesmo quando glScalef eh usada

    // glShadeModel(GL_SMOOTH);
    // //glShadeModel(GL_FLAT);

    // glEnable( GL_LINE_SMOOTH );
    // //glLineWidth( 1.0 );
    // //glEnable( GL_POLYGON_SMOOTH ); //tem que modificar a maneira de fazer o blend ( GL_BLEND e GL_ALPHA )

    // glEnable(GL_DEPTH_TEST);
    // //glDepthFunc(GL_LESS);

    // glEnable(GL_BLEND); //habilita a transparencia
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    

    // // glutReshapeFunc(glutGUI::resize);
    // glutDisplayFunc(desenha);
    // glutKeyboardFunc(teclado);
    // // glutIdleFunc(glutGUI::idle);
    // // glutMouseFunc(mouseButton);
    // // glutMotionFunc(glutGUI::mouseMove);
    // glutEntryFunc(setShader);
    // glutMainLoop();
}