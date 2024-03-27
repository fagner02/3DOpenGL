#version 330 core
out vec4 FragColor;

const uint k = 1103515245U;

vec2 hash(uvec2 x)
{
    x = ((x>>8U)^x.yx)*k;
    x = ((x>>8U)^x.yx)*k;
    x = ((x>>8U)^x.yx)*k;
    
    return vec2(x)*(1.0/float(0xffffffffU));
}

in vec3 pos;
in vec3 color;
in vec3 normal;
in vec2 uv;

uniform float shellIndex;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float divisions;
uniform float shellNum;

void main() {
    vec2 p = floor(uv * divisions)/divisions;
    float even = (mod(round(p.x/(1.0/divisions)), 2.0)==0.0)?0.0:1.0;
    vec2 displace = (vec2(0.0, (1.0/divisions)/2)* even);
    vec2 p2 = floor((uv+displace) * divisions)/divisions;
    vec2 b = hash(uvec2(p2/(1.0/divisions)));
    
    vec2 muv = mod((uv+displace)*(divisions), 1.0);
    float dist = distance(muv, vec2(0.5,0.5));
    float radius = 1.0-(shellIndex/((b.x*0.8+0.2)*shellNum))*0.75;
    bool r = dist>radius || dist>radius;

    vec3 lightDirection = normalize(lightPos - pos);
    float diffuse = max(dot(normalize(normal), lightDirection), 0.0f);
    vec3 ccolor = vec3(b.y*0.2,b.x*0.8+0.2, b.x*0.2) *lightColor* (diffuse+0.2)* (1-radius);
    FragColor = vec4(ccolor, shellIndex <= ((b.x*0.8+0.2)*shellNum) && !r);
    // FragColor = vec4(uv, 0.0,1.0);
}