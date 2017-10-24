#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texcoord;

uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(void) 
{ 
  //a passthrough vertex shader
  vec4 v = vec4(position, 1.0); 
  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v; 
}
          