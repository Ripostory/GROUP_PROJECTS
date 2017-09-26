#version 330

smooth in vec3 color;
in vec2 texCoordModel;

out vec4 frag_color;
      
uniform sampler2D gSampler;

void main(void)
{
   frag_color = texture2D(gSampler, vec2(texCoordModel.x, -texCoordModel.y));
}