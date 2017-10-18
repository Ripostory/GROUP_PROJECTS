#version 330

//take in position and normal
smooth in vec3 normal; 
smooth in vec3 viewDir;
smooth in vec3 lightDir;
smooth in vec3 reflectDir;
in float attenuation;
in vec2 texCoordMod;

uniform sampler2D texture;
uniform sampler2D normalMap;

out vec4 frag_color;

void main(void)
{	
	vec3 L = normalize(lightDir);
	vec3 R = normalize(reflectDir);
	vec3 V = normalize(viewDir);
	vec3 N = normalize(normal);
	
	float diffuse = clamp(dot(L, N), 0, 1);
	float specular = pow(max(dot(V, R), 0), 15.0f);
	vec3 albedo = texture2D(texture, texCoordMod).rgb;
	frag_color = vec4((albedo * diffuse + specular) * attenuation, 1.0f);
}



