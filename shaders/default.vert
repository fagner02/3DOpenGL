#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoords;

out vec3 color;
out vec3 normal;
out vec3 pos;
out vec4 posLight;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 lightProj;
uniform vec4 clipPlane;

void main(){
    gl_Position = proj * view * model * vec4(aPos, 1.0f);
    posLight = lightProj * model * vec4(aPos, 1.0f);
    color = aColor;
    normal = aNormal;
    pos = aPos;
    texCoords = aTexCoords;

    gl_ClipDistance[0] = dot(vec4(aPos, 1.0), clipPlane);
}