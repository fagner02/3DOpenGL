#version 330 core
out vec4 FragColor;

in vec3 Color;
in vec3 Pos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main() {
    vec3 lightDirection = normalize(lightPos - Pos);
    float diffuse = max(dot(normalize(Normal), lightDirection), 0.0f);
    FragColor = (vec4(Color, 1.0f) * vec4(lightColor, 1.0)) * (diffuse + 0.2);
}