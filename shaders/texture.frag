#version 330 core
out vec4 FragColor;

in vec3 pos;
in vec3 normal;
in vec2 texCoord;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D tex;

void main() {
    vec3 lightDirection = normalize(lightPos - pos);
    float diffuse = max(dot(normalize(normal), lightDirection), 0.0f);
    vec3 ccolor = lightColor * (diffuse + 0.2);
    FragColor = texture(tex, texCoord) * vec4(ccolor, 1.0);
}