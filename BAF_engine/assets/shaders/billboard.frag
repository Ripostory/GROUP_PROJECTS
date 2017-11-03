#version 330
in vec2 texCoord;

out vec4 frag_color;

uniform sampler2D image;

void main(void)
{
   frag_color = texture2D(image, texCoord);
}