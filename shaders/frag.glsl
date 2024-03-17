#version 330 core
out vec4 FragColor;

in vec3 Color;
in vec3 Pos;
in vec3 Normal;

uniform vec3 lightPos;

void main() {
    vec3 lightDirection = normalize(lightPos - Pos);
    float diffuse = max(dot(normalize(Normal), lightDirection), 0.0f);
    FragColor = vec4(Color, 1.0f) * diffuse;
}