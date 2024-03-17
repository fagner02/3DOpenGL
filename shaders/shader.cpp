#include "shader.h"

Shader::Shader() {}

void errorLog(int shader) {
    int status, length;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE) {
        return;
    }
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    int writtenChars = 0;
    char* log = new char[length];
    glGetShaderInfoLog(shader, length, &writtenChars, log);

    std::cout << log << "\n";
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


Shader::Shader(char* vertex_filename, char* frag_filename) {
    std::ifstream file(vertex_filename, std::ifstream::binary);

    file.seekg(0, file.end);
    int length = 1 + file.tellg();
    file.seekg(0, file.beg);

    char* vertex = new char[length];
    file.read(vertex, length);

    file.close();
    vertex[length - 1] = '\0';
    file = std::ifstream(frag_filename, std::ifstream::binary);

    file.seekg(0, file.end);
    length = 1 + file.tellg();
    file.seekg(0, file.beg);

    char* frag = new char[length];
    file.read(frag, length);

    frag[length - 1] = '\0';
    file.close();

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

    errorLog(fragShader);
    errorLog(vertexShader);
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
