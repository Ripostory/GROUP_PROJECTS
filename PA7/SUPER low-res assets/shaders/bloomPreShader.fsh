#version 330
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;

const float blurSizeH = 1.0 / 50.0;
const float blurSizeV = 1.0 / 30.0;

void main()
{
	vec4 sum = vec4(0.0);
    	for (int x = -2; x <= 2; x++)
        	for (int y = -2; y <= 2; y++)
            		sum += texture(
                		texFramebuffer,
                		vec2(Texcoord.x + x * blurSizeH, Texcoord.y + y * blurSizeV)
            		) / 25.0;
	outColor = pow(vec4(sum.xyz, 1.0f), vec4(2.0f));
}