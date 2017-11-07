#version 330

uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 normal;
uniform vec4 l_pos;

in vec4 v_position;
in vec3 v_normal;

out vec3 a_normal;
out vec4 eye; 
out vec3 light_dir;

void main () {

		vec4 pos = MV * v_position;

		v_normal = normalize (normal * v_normal);
		light_dir = vec3 (l_pos - pos);
		eye = vec3 (-pos);
	
		gl_Position = MVP * v_position;
}
