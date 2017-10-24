#version 330

in vec2 texCoordModel;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D texture;
uniform sampler2D normalMap;

void main(void)
{
   //albedo > normal > worldPos
   gl_FragData[0] = texture2D(texture, texCoordModel);
   
   //pack normal and Metallic into normal map
   vec3 finalNormal = normalize(normal);
   gl_FragData[1] = vec4(finalNormal, 1.0f);
   
   //pack worldPos and Roughness into worldPos map
   gl_FragData[2] = vec4(fragPos.xyz, 1.0f);
}