#version 330 core
  
layout (location = 0) in vec3 position;
layout(location = 1) in vec3 normals;

uniform mat4 vm;
uniform mat4 pm;
uniform mat4 mm;

uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 view_position;

out vec3 fragment_position;
out vec3 normal;
out vec3 col;

void main()
{
	normal = mat3(transpose(inverse(mm))) * normals;
	fragment_position = vec3(mm * vec4(position, 1.0f));
	gl_Position = pm * vm * mm * vec4(position, 1.0);

	// ambient
	float ambient_strength = 0.25f;
	vec3 ambient = ambient_strength * light_color;

	// diffuse
	vec3 norm = normalize(normal);
	vec3 light_direction = normalize(light_position - fragment_position);
	float diffuse_strength = 0.75f; 
	vec3 diffuse = diffuse_strength * max(dot(norm, light_direction), 0.0) * light_color;

	// Specular
	vec3 view_direction = normalize(view_position - fragment_position);
	float specular_strength = 1.0f;
	vec3 halfway_dir = normalize(light_direction + view_direction);
	vec3 specular = specular_strength * pow(max(dot(norm, halfway_dir), 0.0), 128) * light_color;	

	// Gouraud result (to be multiplied by object_color in fragment shader)
	col = ambient + diffuse + specular;
}