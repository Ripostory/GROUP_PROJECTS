#version 330
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texcoord;

smooth out vec3 normal; 
smooth out vec3 viewDir;
out vec2 texCoordMod;
          
uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

//FIXME: This may be more appropriate as a struct
//Types: 0 = Point 1 = Spot

uniform int lSize;
uniform vec3 lPos[16];
uniform float lRad[16];
uniform int lType[16];
uniform vec3 lRot[16];

smooth out vec3 lightDir[16];
smooth out vec3 reflectDir[16];
out float attenuation[16];
flat out int lights;

void calculateLight(int index, float radius, vec3 fragPos);
void calculateSpotLight (int index, float radius, vec3 fragPos, float cutOff, float angle);

void main(void) 
{

  //temp light and parameters
  float radius = 55.0f;
  
  //matrix calculations
  lights = lSize;
  vec4 v = vec4(v_position, 1.0); 
  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v; 
  texCoordMod = v_texcoord; 
  vec3 fragPos = vec3(modelMatrix * vec4(v_position, 1.0));
  
  //vector calculation
  viewDir = (inverse(viewMatrix) * vec4(0.0f, 0.0f, 1.0f, 0.0f)).xyz;
  normal = vec3(modelMatrix * vec4(v_normal, 0.0f));
  for (int i = 0; i < lSize; i++)
  {
		if (lType[i] == 0)
    	calculateLight(i, radius, fragPos);

		if (lType[i] == 1)
			calculateSpotLight(i, radius, fragPos, 1, 36);
  }

}

void calculateLight(int index, float radius, vec3 fragPos)
{
  lightDir[index] = lPos[index] - fragPos;
  reflectDir[index] = reflect(-lightDir[index], normal);
  attenuation[index] = pow(smoothstep(radius*2, 0 , length(lPos[index] - fragPos)), 2);
}

void calculateSpotLight (int index, float radius, vec3 fragPos, float cutOff, float angle) {

	vec3 n = normalize (normal);
	float distance = length (lPos[index] - fragPos);

	lightDir[index] = lPos[index] - fragPos;

	float NdotL = max (dot (n, normalize (lightDir[index])), 0.0);

	if (NdotL > 0.0) {

		float spotEffect = dot (normalize (lightDir[index]), normalize(-lightDir[index]));

		if (spotEffect > cutOff) {

			spotEffect = pow (spotEffect, 2);
			
			attenuation[index] = spotEffect / (1 + distance);
		}
	}

  lightDir[index] = ((lPos[index] + lRot[index]) - fragPos);
  reflectDir[index] = reflect(-lightDir[index], normal);
  attenuation[index] = pow(smoothstep(radius*2, 0 , length(lPos[index] - fragPos)), 2);
}

