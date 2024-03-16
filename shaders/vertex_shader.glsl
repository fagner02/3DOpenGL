#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

out vec3 color;
out vec3 normal;
out vec3 pos;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;

void main(){
    vec3 transformedPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = proj * view * vec4(transformedPos, 1.0);
    color = aColor;
    normal = aNormal;
    pos = transformedPos;
}