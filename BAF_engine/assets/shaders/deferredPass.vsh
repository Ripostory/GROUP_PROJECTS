#version 330
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texcoord;
layout (location = 3) in vec3 v_tangent;
          
uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec2 texCoordModel;
out vec3 fragPos;
out mat3 TBN;
        
void main(void) 
{ 
  vec4 v = vec4(v_position, 1.0); 
  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v; 
  texCoordModel = v_texcoord; 
  fragPos = vec3(modelMatrix * vec4(v_position, 1.0));
  
  //build TBN matrix
  vec3 v_bitangent = cross(v_tangent, v_normal);
  TBN = mat3(v_tangent, v_bitangent, v_normal);
  TBN = mat3(modelMatrix) * TBN;
}
          