#version 330
in vec2 Texcoord;
out vec4 frag_color;
uniform sampler2D texFramebuffer;

void main()
{
    //ignore background
    if (texture(texFramebuffer, Texcoord) == vec4(0,0,0,0))
        discard;

	vec3 finalColor = texture(texFramebuffer, Texcoord).rgb;

    //gamma correction
    finalColor = finalColor / (finalColor + vec3(1.0));
    finalColor = pow(finalColor, vec3(1.0/2.2));

    frag_color = vec4(finalColor, 1.0f) * 1.5;
}
