#version 330 core
out vec4 FragColor;

in vec3 color;
in vec3 pos;
in vec3 normal;

uniform vec3 lightPos;

void main(){
    vec3 lightDirection = normalize(lightPos - pos);
    float diffuse = max(dot(lightDirection, normalize(normal)), 0.0);
    FragColor = vec4(color, 1.0) * diffuse;
}