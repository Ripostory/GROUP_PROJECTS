#version 330

//take in position and normal
smooth in vec3 color;
in vec3 fragPos;
in vec3 normal;
in vec3 planetNormal;
in vec3 viewDir;
in vec2 texCoordMod;

uniform sampler2D texture;
uniform sampler2D normalMap;

out vec4 frag_color;

in vec3 lightPos;

void main(void)
{	
	//temporary light color
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	//temporary radius
	float radius = 8.0f;
	//basic ambience
	float ambience = 0.0f;
	vec3 ambientColor = ambience * lightColor;
	
	//get norm and light direction;
	vec3 lightDir = lightPos - fragPos;
	//get diffuse strength
	vec3 newNormal = normalize(normal * (texture2D(normalMap, vec2(texCoordMod.x, -texCoordMod.y)).xyz));
	float diff = dot(newNormal, lightDir) / (length(lightDir) * length(newNormal));
	diff = clamp(diff, 0, 1);
	
	vec3 diffuse = diff * lightColor;
	
	
	//atmosphere effects
	vec3 atmColor = vec3(0.3f, 0.5f, 0.7f);
	vec3 atmHorizon = vec3(0.8f, 0.2f, 0.1f);
	float horizonBrightness = max(dot(normal, lightDir), 0)/50;
	vec3 atmFinal = mix(atmHorizon, atmColor, pow(diff, 0.5f));
	float atmBrightness = pow(sin(acos(dot(planetNormal, viewDir))), 5.0f);
	atmBrightness = clamp(atmBrightness, 0 ,1);
	vec4 atmOut = vec4(atmFinal * atmBrightness * horizonBrightness, 1.0f);
	vec4 base = (texture2D(texture, vec2(texCoordMod.x, -texCoordMod.y)) * vec4((ambientColor.rgb + diffuse.rgb), 1.0));
	frag_color =  1- (1-base) * (1-atmOut);
}