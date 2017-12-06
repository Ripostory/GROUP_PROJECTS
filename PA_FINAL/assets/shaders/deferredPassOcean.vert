#version 330
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texcoord;
layout (location = 3) in vec3 v_tangent;
          
uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform float offset;

out vec2 texCoordModel;
out vec3 fragPos;
out mat3 TBN;
        
void main(void) 
{ 
  vec4 v = vec4(v_position, 1.0); 
  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v; 

  //use worldpos texture coordinates
  texCoordModel = (v.xz*0.05)+(offset*0.2);
  fragPos = vec3(modelMatrix * vec4(v_position, 1.0));
  
  //build TBN matrix
  vec3 tangent = vec3(1,0,0);
  vec3 v_bitangent = cross(tangent, v_normal);
  TBN = mat3(tangent, v_bitangent, v_normal);
  TBN = mat3(modelMatrix) * TBN;
}
          
