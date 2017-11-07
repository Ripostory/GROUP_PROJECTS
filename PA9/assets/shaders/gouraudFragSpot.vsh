#version 330

uniform vec4 diffuse;
uniform vec4 ambient;
uniform vec4 specular;
uniform float shininess;

uniform vec4 l_position;
uniform vec4 l_direction;
uniform float l_cutOff;

in vec3 normal;
in vec3 eye;
in vec3 light_direction;

out vec4 color;

void main (void) {

		float intensity = 0.0;
		vec4 spec = vec4 (0.0);
		
		vec3 ld = normalize (light_direction);
		vec3 sd = normalize (vec3 (-l_direction));

		if (dot (sd, ld) > l_cutOff) {

			vec3 n = normalize (normal);
			intensity = max (dot (n, ld, 0.0));

			if (intensity > 0.0) {

				vec3 n_eye = normalize (eye);
				vec3 half = normalize (ld + eye);
				
				float intSpec = max (dot (half, n), 0.0);
				spec = specular * pow (intSpec, shininess);
			}
		}

		color = max (intensity * diffuse + spec, ambient);
}
