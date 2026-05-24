#version 330

uniform uint gObjectIndex;
uniform int objsLength;
out vec4 FragColor;

void main() {
   if(objsLength>0) {
      float c = float(gObjectIndex)/float(objsLength);
      FragColor = vec4(c,c,c,1.0f);
   }
}