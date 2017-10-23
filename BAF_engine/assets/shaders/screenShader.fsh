#version 330
in vec2 Texcoord;
out vec4 frag_color;
uniform sampler2D texFramebuffer;

void main()
{
    frag_color = texture(texFramebuffer, Texcoord);
}