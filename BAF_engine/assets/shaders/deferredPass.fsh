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
   gl_FragData[1] = vec4(normal.xyz, 1.0f);
   gl_FragData[2] = vec4(fragPos.xyz, 1.0f);
}