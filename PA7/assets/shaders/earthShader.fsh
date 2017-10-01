#version 330

//take in position and normal
smooth in vec3 color;
in vec3 fragPos;
in vec3 normal;
in vec3 planetNormal;
in vec3 viewDir;
in vec3 viewPos;
in vec2 texCoordMod;

uniform sampler2D texture;
uniform sampler2D normalMap;
uniform sampler2D nightMap;
uniform sampler2D specularMap;

out vec4 frag_color;

in vec3 lightPos;

void main(void)
{	
	//light color
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 specColor = vec3(1.0f, 0.6f, 0.1f);

	//basic ambience
	float ambience = 0.0f;
	vec3 ambientColor = ambience * lightColor;
	
	//get norm and light direction;
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 indice = -lightDir;

	//get diffuse strength
	vec3 newNormal = normalize(normal * (texture2D(normalMap, vec2(texCoordMod.x, -texCoordMod.y)).xyz));

	//get specular strength
	vec3 reflection = reflect(indice, newNormal);
	float cosAngle = clamp(dot(normalize(viewDir), reflection), 0.0, 1.0);
	float specular = pow(cosAngle, 20.0f);

	float diff = dot(newNormal, lightDir);
	diff = clamp(diff, 0, 1);
	
	vec3 diffuse = diff * lightColor;
	
	
	//atmosphere effects

	vec3 atmColor = vec3(0.3f, 0.5f, 0.7f);
	vec3 atmHorizon = vec3(0.8f, 0.2f, 0.1f);
	
	//get brightness based on sun direction
	float horizonBrightness = max(dot(normal, lightDir), 0);

	//get resulting color
	vec3 atmFinal = mix(atmHorizon, atmColor, pow(diff, 1.0f));

	//get brightness based on view direction
	float atmBrightness = pow(sin(acos(dot(planetNormal, viewDir))), 4.0f)*1.5;
	atmBrightness = clamp(atmBrightness, 0 ,1);

	//raw output for atmosphere effect and planet lighting
	vec4 atmOut = vec4(atmFinal * atmBrightness * horizonBrightness, 1.0f);
	vec4 base = (texture2D(texture, vec2(texCoordMod.x, -texCoordMod.y)) * vec4((ambientColor.rgb + diffuse.rgb), 1.0));
	vec4 night = (texture2D(nightMap, vec2(texCoordMod.x, -texCoordMod.y))-0.1f)*(1.0 - vec4(diffuse, 1.0f));
	vec4 specularOut = vec4(specular * texture2D(specularMap, vec2(texCoordMod.x, -texCoordMod.y)).r * specColor, 1.0f);
	//screen output mix	
	frag_color =  (1- (1-base) * (1-atmOut)) + specularOut + night;
}