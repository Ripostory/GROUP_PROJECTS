#version 330

uniform vec4 m_diffuse;
uniform vec4 m_ambient;
uniform vec4 m_specular;
uniform float shininess;

uniform vec3 light_direction;

in vec3 normal;
in vec4 eye;
in vec3 light_dir;

out vec4 frag_color;

void main(void) {


		vec4 spec = vec4 (0.0, 0.0, 0.0, 0.0);

		vec3 n = normalize (normal);
		vec3 l = normalize (light_dir);
		vec3 e = normalize (vec3 (eye));

		float intensity = max (dot (n, light_direction), 0.0);
		
		if (intensity > 0.0) {

				vec3 half = normalize (light_direction + e);
				float intSpec = max (dot (h, n), 0.0);
				spec = specular * pow (intSpec, shininess);
				
		}

		frag_color = max (intensity * diffuse + spec, ambient);
}
