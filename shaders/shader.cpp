#include "shader.h"

Shader::Shader(){}

Shader::Shader(char* vertex_filename, char* frag_filename) {
    std::ifstream file(vertex_filename, std::ifstream::binary);

    file.seekg(0, file.end);
    int length = file.tellg();
    file.seekg(0, file.beg);

    char* vertex = new char[length];
    file.read(vertex, length);

    file.close();

    file = std::ifstream(frag_filename, std::ifstream::binary);

    file.seekg(0, file.end);
    length = file.tellg();
    file.seekg(0, file.beg);

    char* frag = new char[length];
    file.read(frag, length);

    file.close();

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    int fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertex, nullptr);
    glShaderSource(fragShader, 1, &frag, nullptr);

    glCompileShader(vertexShader);
    glCompileShader(fragShader);

    int fragC, vertexC, fragL, vertexL;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexC);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragC);

    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &vertexL);
    glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &fragL);

    
    int writtenChars = 0;
    char* log = new char [vertexL];
    glGetShaderInfoLog(vertexShader, vertexL, &writtenChars, log);
    std::cout << log << "\n";
    free(log);

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
}

void Shader::useShader() {
    glUseProgram(shaderProgram);
}

void Shader::deleteShader() {
    glDeleteProgram(shaderProgram);
}
