#version 330
out vec4 frag_color;

uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D depth;

uniform vec2 gScreenSize;

float normalDistr(vec3 N, vec3 H, float roughness);
float geometrySchlick(float NV, float roughness);
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnel(float cTheta, vec3 F0);
float convertFog(float depth);

void main()
{	
	vec2 Texcoord = gl_FragCoord.xy / gScreenSize;
    //fog color
    vec3 fog = vec3(0.15,0.44,0.910)/2;

    //fog amount
    float fogAmt = convertFog(texture(depth, Texcoord).r);

    vec3 ambient =  fog * (fogAmt*0.1);
    
    frag_color = vec4(ambient, texture(albedo, Texcoord).a);

    //set fullbright if the normals are 0
    if (texture(normal, Texcoord) == vec4(0,0,0,1))
    	frag_color = texture(albedo, Texcoord);

}

float normalDistr(vec3 N, vec3 H, float roughness)
{
	// implement normal distribution function
	// (a^2)/(PI((N*H)^2(a^2-1)+1)^2)
	// where a is roughness, N is normal, and H is halfway vec
	 
	float rough2 = pow(roughness, 2);
	float NH = max(dot(N, H), 0.0);
	float NH2 = pow(NH, 2);
	
	float denominator = (NH2 * (rough2 - 1.0) + 1.0);
	denominator = 3.1415 * pow(denominator, 2);
	
	return rough2 / denominator;
}

float geometrySchlick(float NV, float roughness)
{
	//implements Schlick GGX
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;
	return NV / (NV * (1.0 - k) + k);
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	//implements Smith's method
	float NV = max(dot(N, V), 0.0);
	float NL = max(dot(N, L), 0.0);
	float ggx1 = geometrySchlick(NV, roughness);
	float ggx2 = geometrySchlick(NL, roughness);
	
	return ggx1 * ggx2;
}


vec3 fresnel(float cTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cTheta, 5.0);

}

float convertFog(float depth)
{
	float nearPane = 0.001;
	float farPane = 700.0;
	return nearPane/(farPane - depth * (farPane-nearPane));
}
