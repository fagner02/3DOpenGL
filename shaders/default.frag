#version 330 core
out vec4 FragColor;

in vec3 color;
in vec3 pos;
in vec3 normal;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main() {
    vec3 lightDirection = normalize(lightPos - pos);
    float diffuse = max(dot(normalize(normal), lightDirection), 0.0f);
    vec3 ccolor =  color * lightColor * (diffuse + 0.2);
    FragColor = vec4(ccolor, 1.0);
}