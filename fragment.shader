#version 330 core

out vec4 result_color;

uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 object_color;
uniform vec3 view_position;
uniform bool colour;
uniform bool flag;
uniform bool lights;
uniform bool normalcol;
uniform bool greyscale;
uniform bool red;
uniform bool blue;
uniform bool green;
uniform sampler2D shadowMap;

in vec3 fragment_position; //interpolated
in vec3 normal;
in vec4 FragPosLightSpace;

in vec3 col;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 norm, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

void main()
{
	vec3 norm = normalize(normal);

	//ambient
	float ambient_strength = 0.25f;
	vec3 ambient = ambient_strength * light_color;

	//diffuse
	vec3 light_direction = normalize(light_position - fragment_position);
	float diffuse_strength = 0.75f;  //use max so that it doesn't go negative
	vec3 diffuse = diffuse_strength * max(dot(normalize(norm), light_direction), 0.0) * light_color ;

	//Specular
	vec3 view_direction = normalize(view_position - fragment_position);
	float specular_strength = 1.0f;
	vec3 halfway_dir = normalize(light_direction + view_direction);
	vec3 specular = specular_strength * pow(max(dot(norm, halfway_dir), 0.0), 128) * light_color;	
	vec3 color = (specular + diffuse + ambient) * object_color;
	
	if (red == true) {
		if (color.x != ((specular + diffuse + ambient) * object_color).x) {
			color.x = ((specular + diffuse + ambient) * object_color).x;
		}
		else 
			color.x = 0;
	}

	if (green == true) {
		if (color.y != ((specular + diffuse + ambient) * object_color).y) {
			color.y = ((specular + diffuse + ambient) * object_color).y;
		}
		else
			color.y = 0;
	}

	if (blue == true) {
		if (color.z != ((specular + diffuse + ambient) * object_color).z) {
			color.z = ((specular + diffuse + ambient) * object_color).z;
		}
		else
			color.z = 0;
	}

	if (colour == true) {
		if (color != (specular + diffuse + ambient) * object_color) {
			color = (specular + diffuse + ambient) * object_color;
		}
	}

	//Gouraud
	if (flag == true) {
		if (normalcol == true) {
			if (greyscale == true) {
				float grey = dot(col, vec3(0.2989, 0.5870, 0.1140));
				result_color = vec4(grey, grey, grey, 1.0f);
			}
			else
				result_color = vec4(col, 1.0f);
		}
		else {
			if (greyscale == true) {
				float grey = dot(col, vec3(0.2989, 0.5870, 0.1140));
				result_color = vec4(grey, grey, grey, 1.0f);
			}
			else 
				result_color = vec4(col, 1.0f);
		}
	}

	//Phong
	else {
		//Without Lighting
		if (lights == true) {
			color = ambient * object_color;
			if (red == true) {
				if (color.x != (ambient * object_color).x) {
					color.x = (ambient * object_color).x;
				}
				else
					color.x = 0;
			}

			if (green == true) {
				if (color.y != (ambient * object_color).y) {
					color.y = (ambient * object_color).y;
				}
				else
					color.y = 0;
			}

			if (blue == true) {
				if (color.z != (ambient * object_color).z) {
					color.z = (ambient * object_color).z;
				}
				else
					color.z = 0;
			}

			if (normalcol == true) {
				color = norm;
				if (greyscale == true) {
					float grey = dot(color, vec3(0.2989, 0.5870, 0.1140));
					result_color = vec4(grey, grey, grey, 1.0f);
				}
				else
					result_color = vec4(color, 1.0f);
			}
			else {
				if (greyscale == true) {
					float grey = dot(color, vec3(0.2989, 0.5870, 0.1140));
					result_color = vec4(grey, grey, grey, 1.0f);
				}
				else {
					
					result_color = vec4(color, 1.0f);
				}
			}
		}
		//With Lighting
		else {
			if (normalcol == true) {
				color = (specular + diffuse + ambient) * norm;
				if (greyscale == true) {
					float grey = dot(color, vec3(0.2989, 0.5870, 0.1140));
					result_color = vec4(grey, grey, grey, 1.0f);
				}
				else
					result_color = vec4(color, 1.0f);
			}
			else {
				if (greyscale == true) {
					float grey = dot(color, vec3(0.2989, 0.5870, 0.1140));
					result_color = vec4(grey, grey, grey, 1.0f);
				}
				else {
					result_color = vec4(color, 1.0f);
				}
			}
		}
	}

	// Apply Shadow Mapping
	if (lights == false) { // lights == false means lighting is ENABLED
		float shadow = ShadowCalculation(FragPosLightSpace, normalize(normal), normalize(light_position - fragment_position));
		vec3 ambient_base = 0.25f * light_color;
		vec3 current_ambient;
		
		if (normalcol == true) {
			current_ambient = ambient_base * normalize(normal);
		} else {
			current_ambient = ambient_base * object_color;
			// Preserve the channel toggling logic for the ambient component
			if (red == true) current_ambient.x = 0.0;
			if (green == true) current_ambient.y = 0.0;
			if (blue == true) current_ambient.z = 0.0;
			if (colour == true) current_ambient = ambient_base * object_color;
		}

		if (greyscale == true) {
			float grey_amb = dot(current_ambient, vec3(0.2989, 0.5870, 0.1140));
			current_ambient = vec3(grey_amb);
		}

		// Darken the non-ambient parts based on the shadow value
		result_color.rgb = current_ambient + (result_color.rgb - current_ambient) * (1.0 - shadow);
	}
} 