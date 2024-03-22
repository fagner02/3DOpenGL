#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aUV;

out vec3 color;
out vec3 normal;
out vec3 pos;
out vec2 uv;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;

void main() {
    gl_Position = proj * view * model * vec4(aPos, 1.0f);
    color = aColor;
    normal = aNormal;
    pos = aPos;
    uv = aUV;
}