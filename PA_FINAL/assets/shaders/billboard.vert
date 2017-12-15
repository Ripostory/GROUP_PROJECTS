#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec2 texCoord;

void main(void) 
{ 
  //a passthrough vertex shader
  vec4 v = vec4(position, 1.0);
  vec4 position = modelMatrix * vec4(0,0,0,1);
  mat4 inv = inverse(viewMatrix);
  mat4 billMatrix = mat4(inv[0], inv[1], inv[2], vec4(0,0,0,1));
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * billMatrix * v;
  texCoord = texcoord;
}
