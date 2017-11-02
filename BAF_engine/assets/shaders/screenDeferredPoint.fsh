#version 330
out vec4 frag_color;

uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D worldPos;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 color;
uniform float radius;
uniform vec2 gScreenSize;

float normalDistr(vec3 N, vec3 H, float roughness);
float geometrySchlick(float NV, float roughness);
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnel(float cTheta, vec3 F0);

void main()
{	    
	vec2 Texcoord = gl_FragCoord.xy / gScreenSize;
    //ignore background
    if (texture(albedo, Texcoord) == vec4(0,0,0,0))
        discard;
        
        
    //get material properties from alphas
    float metallic = texture(normal, Texcoord).a;
    float roughness = texture(worldPos, Texcoord).a;
    
    
    vec3 fragPos = texture(worldPos, Texcoord).xyz;
    vec3 lightDir = lightPos - fragPos;
        
    vec3 finalNormal = texture(normal, Texcoord).xyz;
    vec3 viewDir = cameraPos;
    
    //vector calculation
    vec3 N = normalize(finalNormal);
    vec3 L = normalize(lightDir);
    vec3 V = normalize(viewDir);
    vec3 R = normalize(L+V); //implements blinn-phong
    
    //attenuation
    float attenuation = pow(smoothstep(radius*2, 0 , length(lightDir)), 7);
    
    //base color (albedo)
    vec3 finalAlbedo = texture(albedo, Texcoord).rgb;
    
    //radiance calcuations
    vec3 F0 = vec3(0.04); //dielectric constant
    F0 = mix(F0, finalAlbedo, metallic);
    vec3 F = fresnel(max(dot(N, V), 0.0), F0);
    
    float nDistribution = normalDistr(N, R, roughness);       
	float geometry = geometrySmith(N, V, L, roughness);   
    
    //Cook-Torrance BDRF
    vec3 top = nDistribution * geometry * F;
    float bottom = 4* max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; 
    vec3 specular = top / bottom;
    
    vec3 finalSpec = max(F, 0.0);
    vec3 finalDiff = vec3(1.0) - finalSpec;
    finalDiff *= 1.0 - metallic;
    
    float lightAngle = max(dot(N, L), 0.0);
    vec3 finalColor = (finalDiff * finalAlbedo / 3.1415 + specular) * attenuation * color * lightAngle;
    
    //gamma correction
    finalColor = finalColor / (finalColor + vec3(1.0));
    finalColor = pow(finalColor, vec3(1.0/2.2));
    
    frag_color = vec4(finalColor, 1.0f);
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

