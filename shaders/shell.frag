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
uniform vec2 displace;
uniform vec2 scale;

vec2 getHex(vec2 p) {
    vec2 s = vec2(1.7320508,1);
    vec4 hC = floor(vec4(p, p - vec2(1,.5))/s.xyxy) + .5;
    
    vec4 h = vec4(p - hC.xy*s, p - (hC.zw +0.5)*s);
    
    return dot(h.xy, h.xy) < dot(h.zw, h.zw) 
        ? vec2(h.x, h.y) 
        : vec2(h.z, h.w);
}

void main() {
    vec2 p = floor(uv * divisions)/divisions;
    vec3 o = vec3(0.0);
    vec2 puv = uv;
    puv.y *= sqrt(3.0)+scale.y;
    puv.x *= 1+scale.x;
    //puv+= displace;
    puv -= o.xy = fract( puv*= mat2(divisions*sqrt(3.0), 5.8+displace.x, 0, 11.5+displace.y));
    p = ( (o.z = mod(puv.x+puv.y, 3.0)) < 2. ? (puv + o.z)  :  (puv + step(o.yx,o.xy)) ).xy / 15.;
    // float even = (mod(round(p.x/(1.0/divisions)), 2.0)==0.0)?0.0:1.0;
    // vec2 displace = (vec2(0.0, (1.0/divisions)/2)* even);
    // vec2 p2 = floor((uv+displace) * divisions)/divisions;
    vec2 b = hash(uvec2(p*1000.0));
    
    // vec2 muv = mod((uv+displace)*(divisions), 1.0);
    vec2 h = getHex((uv)*divisions);
    float dist = distance(h, vec2(sqrt(3.0)/2.0, 1.0)*0.1)+0.1;
    float radius = 1.0-(shellIndex/((b.x*0.8+0.2)*shellNum))*0.75;
    bool r = dist>radius || dist>radius;

    vec3 lightDirection = normalize(lightPos - pos);
    float diffuse = max(dot(normalize(normal), lightDirection), 0.0f);
    vec3 ccolor = vec3(b.y*0.2,b.x*0.8+0.2, b.x*0.2) *lightColor* (diffuse+0.2)* (1-radius);
     FragColor = vec4(ccolor, shellIndex <= ((b.x*0.8+0.2)*shellNum) && !r);
    // FragColor = vec4(muv, 0.0,1.0);
    
   // FragColor = vec4(h.x,p, 1.0);
}