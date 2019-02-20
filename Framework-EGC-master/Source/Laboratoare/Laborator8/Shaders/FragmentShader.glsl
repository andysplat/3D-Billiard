#version 330

in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

layout(location = 0) out vec4 out_color;

void main()
{
	
	vec3 L = normalize( light_position - world_position );
	vec3 V = normalize( eye_position - world_position );
	vec3 H = normalize( L + V );
	
	// TODO: define ambient light component
	float ambient_light = 0;

	// TODO: compute diffuse light component
	float diffuse_light = material_kd * max (dot(world_normal, L), 0);

	// TODO: compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		specular_light = material_ks * pow(max(dot(world_normal, H), 0), material_shininess);
	}

	// TODO: compute light
	vec3 light;
	float cut_off = radians(30.0);
	float spot_light = dot(-L, light_direction);
	float spot_light_limit = cos(cut_off);
	float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
	float light_att_factor = pow(linear_att, 2);
	float aten_fac = 1 / pow(length(light_position - world_position), 2);
	if (spot_light > cos(cut_off))
	{
		light = object_color * (ambient_light + (light_att_factor + aten_fac) * (diffuse_light + specular_light));
	}
	else 
		light =  object_color * vec3(0);
	
	out_color = vec4(light, 1);
	// TODO: write pixel out color
}