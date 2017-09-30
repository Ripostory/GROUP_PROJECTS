#version 330

//take in position and normal
smooth in vec3 color;
in vec3 fragPos;
in vec3 normal;
in vec3 planetNormal;
in vec3 viewDir;
in vec2 texCoordMod;

uniform sampler2D texture;

out vec4 frag_color;

in vec3 lightPos;

void main(void)
{	
	//light color
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	
	//atmosphere effects
	vec3 atmHorizon = vec3(0.8f, 0.4f, 0.1f);
	vec3 atmFringe = vec3(0.9f, 0.2f, 0.1f);
	
	//get brightness based on view direction
	float atmBrightness = pow(sin(acos(dot(planetNormal, viewDir))), 4.0f);
	atmBrightness = atmBrightness * 5;

	//final output color
	vec3 atmColor = mix(atmFringe, atmHorizon, normalize(pow(atmBrightness, 2.0f)));

	//raw output for atmosphere effect and planet lighting
	vec4 atmOut = vec4(atmColor * atmBrightness, 1.0f);
	vec4 base = (texture2D(texture, vec2(texCoordMod.x, -texCoordMod.y), 1.0));

	//screen output mix	
	frag_color = (1 - (1 - base) * ( 1- vec4(0.4, 0.0, 0.0, 1.0))) + atmOut;
}