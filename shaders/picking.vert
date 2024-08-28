#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;
uniform vec4 clipPlane;

void main(){
    gl_Position = proj * view * model * vec4(aPos, 1.0f);
    gl_ClipDistance[0] = dot(vec4(aPos, 1.0), clipPlane);
}