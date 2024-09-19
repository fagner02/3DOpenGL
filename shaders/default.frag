#version 330 core
out vec4 FragColor;

in vec3 color;
in vec3 pos;
in vec4 posLight;
in vec3 normal;
in vec2 texCoord;

uniform int useTex;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D tex;
uniform sampler2D depthMap;

void main() {
    vec3 lightDirection = normalize(lightPos - pos);
    vec3 lightCoord = (posLight.xyz / posLight.w) * 0.5+0.5;
    float closestDepth = texture(depthMap, lightCoord.xy).r;
    float currentDepth = lightCoord.z;
    float unit = 1.0/2048.0;
    float winlen = 1;
    float window = winlen * unit;
    float matlen = pow(winlen*2+1, 2);
    float shadow = 0;
    float bias = max(0.0005 * (1.0 - dot(normal, lightDirection)), 0.0005);  
    // bias = 0.0;
    if(lightCoord.x <=1&&lightCoord.y<=1&&lightCoord.x >=0&&lightCoord.y>=0){
        for(float i = -window; i <= window; i+= unit){
            vec2 newCoord = lightCoord.xy+vec2(i,0);
            if(newCoord.x>1||newCoord.x<0) {
                shadow+=1.0;
                continue;
            }
            for(float j = -window; j <= window; j += unit){
                newCoord = lightCoord.xy+vec2(i,j);
                if(newCoord.y>1||newCoord.y<0) {
                    shadow+=1.0;
                    continue;
                }
                float closestDepth = texture(depthMap, newCoord).r;
                float currentDepth = lightCoord.z;
                shadow += currentDepth - bias > closestDepth ? 0.1 : 1.0;
            }
        }
        shadow /= matlen;
    }else{
        shadow=1.0;
    }
    
   
    float diffuse = max(dot(normalize(normal), lightDirection), 0.0f);
    vec3 lightValue = lightColor * (diffuse * shadow + 0.4);

    if (useTex == 1){
        FragColor = vec4(texture(tex, texCoord).xyz * lightValue, 1.0);
    }else{
        FragColor = vec4(color * lightValue, 1.0);
    }
}