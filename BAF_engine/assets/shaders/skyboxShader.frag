#version 330

in vec3 viewVector;

out vec4 frag_color;

uniform samplerCube skybox;

void main(void)
{
   frag_color = textureCube(skybox, viewVector);
}