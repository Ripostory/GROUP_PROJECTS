#version 330
out vec4 frag_color;

uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D worldPos;

uniform vec2 gScreenSize;

void main()
{	
	vec2 Texcoord = gl_FragCoord.xy / gScreenSize;
    //ignore background
    if (texture(albedo, Texcoord) == vec4(0,0,0,0))
        discard;
        
    //low color and high color
    vec3 sky = vec3(0,0.2,0.3);
    vec3 ground = vec3(0.1,0.1,0);
        
        
    vec3 normal = texture(normal, Texcoord).xyz;
    
    vec3 N = normalize(normal);
    vec3 L = vec3(0,1,0);
    
    float skyAngle = max(dot(L, N), 0);
    vec3 ambient = mix(ground, sky, vec3(1.0f)*skyAngle);
    
    frag_color = vec4(texture(albedo, Texcoord).rgb * ambient, 1.0f);
}