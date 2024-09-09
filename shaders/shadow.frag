#version 330 core
out vec4 FragColor;

in vec4 pos;

void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
    vec3 coord = (pos.xyz /pos.w) ;

    FragColor = vec4(coord.z, coord.z, coord.z, 1.0);
}  