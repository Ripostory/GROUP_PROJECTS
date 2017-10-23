#version 330
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texcoord;
          
uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec2 texCoordModel;
out vec3 normal;
out vec3 fragPos;
        
void main(void) 
{ 
  vec4 v = vec4(v_position, 1.0); 
  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v; 
  normal = vec3(modelMatrix * vec4(v_normal, 0.0f));
  texCoordModel = v_texcoord; 
  fragPos = vec3(modelMatrix * vec4(v_position, 1.0));
}
          