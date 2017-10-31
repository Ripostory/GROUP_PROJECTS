#version 330
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;
uniform sampler2D bloom1;
uniform sampler2D bloom2;
uniform sampler2D bloom3;

void main()
{
	vec4 bloomColor = (texture(bloom1, Texcoord)
		+ texture(bloom2, Texcoord)
		+ texture(bloom3, Texcoord))/3;

	bloomColor = clamp(bloomColor, 0, 1) * 1.8;

	outColor = 1 - (1- bloomColor) * (1 - clamp(texture(texFramebuffer, Texcoord),0 ,1));
}