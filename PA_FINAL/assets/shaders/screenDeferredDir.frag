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

void main()
{	
	vec2 Texcoord = gl_FragCoord.xy / gScreenSize;
    //ignore background
    if (texture(albedo, Texcoord) == vec4(0,0,0,0))
        discard;
        
    vec3 fragPos = texture(worldPos, Texcoord).xyz;
    vec3 lightDir = lightPos - fragPos;
        
    vec3 normal = texture(normal, Texcoord).xyz;
    vec3 viewDir = cameraPos;
    
    vec3 N = normalize(normal);
    vec3 L = normalize(lightDir);
    vec3 V = normalize(viewDir);
    vec3 R = normalize(reflect(-lightDir, normal));
    float attenuation = pow(smoothstep(radius*2, 0 , length(lightDir)), 7);
    
    float diffuse = max(dot(L, N), 0);
    float specular = pow(max(dot(V, R), 0), 15.0f);
    frag_color = vec4(((color * texture(albedo, Texcoord).rgb * diffuse) + specular)*attenuation, 1.0f);
    //frag_color = vec4(normal, 1.0f);
}