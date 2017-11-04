#version 330

in vec2 texCoordModel;
in vec3 fragPos;
in mat3 TBN;

uniform sampler2D texture;
uniform sampler2D normalMap;
uniform sampler2D specMap;

void main(void)
{
   //temp material properties
   float metallic = 0;
   float roughness = texture2D(specMap, texCoordModel).r;
	
   //modify textures to fit range
   metallic = mix(0.1, 1.0, metallic);
   roughness = mix(0.02, 1.0, roughness);

   //albedo > normal
   gl_FragData[0] = texture2D(texture, texCoordModel);
   
   //pack normal
   vec3 normal = texture2D(normalMap, texCoordModel).xyz;
   normal = normalize(normal * 2.0 - 1.0);
   normal = normalize(TBN * normal);
   gl_FragData[1] = vec4(normal, 1.0);

   //pass material information
   gl_FragData[2] = vec4(metallic, roughness, 1, 1);
}