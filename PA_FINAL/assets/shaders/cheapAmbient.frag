#version 330
out vec4 frag_color;

uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D worldPos;

uniform vec2 gScreenSize;

float normalDistr(vec3 N, vec3 H, float roughness);
float geometrySchlick(float NV, float roughness);
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnel(float cTheta, vec3 F0);

void main()
{	
	vec2 Texcoord = gl_FragCoord.xy / gScreenSize;

    //low color and high color
    vec3 sky = vec3(0.7,0.9,0.9);
    vec3 side = vec3(0.4,0.4,0.4);
    vec3 ground = vec3(0.4,0.4,0.4);
        
    //get material properties from alphas
    float metallic = texture(normal, Texcoord).a;
    float roughness = texture(worldPos, Texcoord).a;
    vec3 finalAlbedo = texture(albedo, Texcoord).rgb;
        
    vec3 finalNormal = texture(normal, Texcoord).xyz;
    
    vec3 N = normalize(finalNormal);
    vec3 L = vec3(0,1,0);
    
	//radiance calcuations
    vec3 F0 = vec3(0.04); //dielectric constant
    F0 = mix(F0, finalAlbedo, metallic);
    vec3 F = fresnel(max(0.5, 0.0), F0);
    
    vec3 finalSpec = max(F, 0.0);
    vec3 finalDiff = vec3(1.0) - finalSpec;
    finalDiff *= 1.0 - metallic;
    
    float skyAngle = max(dot(L, N), 0);
    float invskyAngle = max(dot(-L, N), 0);
    float sideAngle = 1 - skyAngle + invskyAngle;
    vec3 finalColor = sky*skyAngle + ground*invskyAngle + side*sideAngle;
    vec3 ambient = (finalDiff * finalAlbedo) * finalColor;
    
    frag_color = vec4(ambient, texture(albedo, Texcoord).a);
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
