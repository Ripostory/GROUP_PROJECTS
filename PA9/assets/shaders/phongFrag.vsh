#version 330
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texcoord;
         
smooth out vec3 normal; 
smooth out vec3 viewDir;
smooth out vec3 lightDir;
smooth out vec3 reflectDir;
out float attenuation;
out vec2 texCoordMod;
          
uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
        

void main(void) 
{ 

  //temp light and parameters
  vec3 lightPos = vec3(15,8,0);
  float radius = 80.0f;
  
  //matrix calculations
  vec4 v = vec4(v_position, 1.0); 
  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v; 
  texCoordMod = v_texcoord; 
  vec3 fragPos = vec3(modelMatrix * vec4(v_position, 1.0));
  
  //vector calculation
  viewDir = (inverse(viewMatrix) * vec4(0.0f, 0.0f, 1.0f, 0.0f)).xyz;
  lightDir = lightPos - fragPos;
  normal = vec3(modelMatrix * vec4(v_normal, 0.0f));
  reflectDir = reflect(-lightDir, normal);
  attenuation = pow(smoothstep(radius, 0 , length(lightPos - fragPos)), 2);
}
         