#include "shader.h"

Shader::Shader() {}

void errorLog(int shader, char* filename, char* source) {
    int status, length;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE) {
        return;
    }
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    int writtenChars = 0;
    char* log = new char[length];
    glGetShaderInfoLog(shader, length, &writtenChars, log);

    std::cout << "FILE: " << filename << ".\n" << source << "\n" << log << "\n";
    free(log);
}

void pError(int program) {
    GLint success = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        GLchar errorLog[1024] = { 0 };
        glGetProgramInfoLog(program, 1024, NULL, errorLog);
        std::cout << "error linking program: " << errorLog;
    }

}

void vE(int program) {
    int success = GL_FALSE;
    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE) {
        GLchar errorLog[1024] = { 0 };
        glGetProgramInfoLog(program, 1024, NULL, errorLog);
        std::cout << "error validating shader program; Details: " << errorLog;

    }
}

char* Shader::loadSource(char* filename) {
    std::ifstream file(filename, std::ifstream::binary);
    if (file.fail()) {
        std::cout << "Error loading shader source file\n";
    }

    file.seekg(0, file.end);
    int length = 1 + file.tellg();
    file.seekg(0, file.beg);

    char* vertex = new char[length];
    file.read(vertex, length);

    file.close();
    vertex[length - 1] = '\0';
    return vertex;
}
Shader::Shader(char* vertex_filename, char* frag_filename) {

    char* vertex = loadSource(vertex_filename);
    char* frag = loadSource(frag_filename);

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    int fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertex, nullptr);
    glShaderSource(fragShader, 1, &frag, nullptr);

    glCompileShader(vertexShader);
    glCompileShader(fragShader);

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);

    errorLog(fragShader, frag_filename, frag);
    errorLog(vertexShader, vertex_filename, vertex);
    pError(shaderProgram);
    vE(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
}

void Shader::useShader() {
    glUseProgram(shaderProgram);
}

void Shader::deleteShader() {
    glDeleteProgram(shaderProgram);
}
