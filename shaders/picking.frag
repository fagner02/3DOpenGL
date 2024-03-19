#version 330

uniform uint gObjectIndex;

out uvec4 FragColor;

void main()
{
   FragColor = uvec4(gObjectIndex, 0.0, gl_PrimitiveID, 0.1);
}