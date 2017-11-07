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

uniform int lSize;
uniform vec3 lPos[16];
uniform float lRad[16];
uniform int lType[16];
uniform vec3 lRot[16];

uniform sampler2D texture;
uniform sampler2D normalMap;

smooth out vec3 lightDir[16];
smooth out vec3 reflectDir[16];
out float attenuation[16];
flat out int lights;

out vec4 color;

void calculateLight(int index, float radius, vec3 fragPos);
void calculateSpotLight (int index, float radius, vec3 fragPos, float cutOff, float angle);

float calculateDiff(int index, vec3 N);
float calculateSpecular(int index, vec3 V);

void main(void) 
{ 
  //temp light and parameters
  float radius = 40.0f;
  
  
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
		if (lType [i] == 0)
    	calculateLight(i, radius, fragPos);


		if (lType[i] == 1)
			calculateSpotLight(i, radius, fragPos, 1, 36);
  }

	vec3 N = normalize(normal);
	vec3 V = normalize(viewDir);
	
	float finalDiff = 0;
	float finalSpec = 0;
	float finalAtten = 1;
	for (int i = 0; i < lights; i++)
	{
		finalDiff += calculateDiff(i, N);
		finalSpec += calculateSpecular(i, V);
		finalAtten *= attenuation[i];
	}

	vec3 albedo = texture2D(texture, texCoordMod).rgb;
	color = vec4((albedo * finalDiff) * finalAtten * 2, 1.0f);
}

void calculateLight(int index, float radius, vec3 vertPos)
{
  lightDir[index] = lPos[index] - vertPos;
  reflectDir[index] = reflect(-lightDir[index], normal);
  attenuation[index] = pow(smoothstep(radius*2, 0 , length(lPos[index] - vertPos)), 2);
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

float calculateDiff(int index, vec3 N)
{
	vec3 L = normalize(lightDir[index]);
	return clamp(dot(L, N), 0, 1);
}

float calculateSpecular(int index, vec3 V)
{
	vec3 R = normalize(reflectDir[index]);
	return pow(max(dot(V, R), 0), 15.0f);
}


