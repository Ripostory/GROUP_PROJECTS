#version 330

//take in position and normal
in vec4 color;
in vec2 texCoordMod;

uniform sampler2D texture;
uniform sampler2D normalMap;

out vec4 frag_color;

void main(void)
{	
	
	frag_color = color;
}

