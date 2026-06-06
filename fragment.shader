#version 330 core

out vec4 result_color;

uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 object_color;
uniform vec3 view_position;

in vec3 fragment_position; 
in vec3 normal;

void main()
{
	vec3 norm = normalize(normal);
	vec3 light_direction = normalize(light_position - fragment_position);

	// ambient
	float ambient_strength = 0.25f;
	vec3 ambient = ambient_strength * light_color;

	// diffuse
	float diffuse_strength = 0.75f;
	vec3 diffuse = diffuse_strength * max(dot(norm, light_direction), 0.0) * light_color;

	// Specular
	vec3 view_direction = normalize(view_position - fragment_position);
	float specular_strength = 1.0f;
	vec3 halfway_dir = normalize(light_direction + view_direction);
	vec3 specular = specular_strength * pow(max(dot(norm, halfway_dir), 0.0), 128) * light_color;	
	
	vec3 lighting_result = ambient + diffuse + specular;
	result_color = vec4(lighting_result * object_color, 1.0f);
}