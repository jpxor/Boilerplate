### VERTEX ###

#version 430 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float fog_density;
uniform float fog_visibility;
uniform vec3 light_positions[4];

layout(location=0) in vec4 position;
layout(location=1) in vec2 texcoord;
layout(location=2) in vec3 normal;

out float fog_fraction;
out vec2 frag_texcoord;
out vec4 frag_normal;
out vec3 toward_lights[4];
out vec3 toward_camera;

void main(){
    vec4 world_position = model * position;
    vec4 view_position = view * world_position;
    gl_Position = projection * view_position;

    frag_texcoord = texcoord;
    frag_normal = model * vec4(normal,0);

    vec4 camera_position = inverse(view) * vec4(0, 0, 0, 1);
    toward_camera = (camera_position - world_position).xyz;
    
    float distance = length(view_position.xyz);
    float fraction = clamp(fog_visibility / distance, 0.0, 1.0);
    float gradient = clamp(fog_density, 0.01, 2.0);
    fog_fraction =  pow(fraction, 1.0/gradient);

    for(int i = 0; i < 4; ++i){
		toward_lights[i] = light_positions[i] - world_position.xyz;
	}
}

### FRAGMENT ###

#version 430 core

uniform vec3 ambient_light;
uniform vec3 diffuse_color;
uniform vec3 specular_color;
uniform float specular_exponent;
uniform float transparency;

const float specular_reflection = 1.0;

uniform vec3 fog_colour;
uniform vec3 light_colors[4];
uniform vec3 light_attenuation[4];

in float fog_fraction;
in vec2 frag_texcoord;
in vec4 frag_normal;
in vec3 toward_lights[4];
in vec3 toward_camera;

out vec4 frag_colour;

float saturate(float val){
	return max(0,min(1,val));
}

void main(){
    vec3 unit_normal = normalize(frag_normal).xyz;
	vec3 unit_camera = normalize(toward_camera);

    vec3 sum_light = ambient_light;
	vec3 sum_specular = vec3(0,0,0);

    for(int i=0; i<4; ++i){
		float distToLight = length(toward_lights[i]);
		float attenuationFactor = (light_attenuation[i].x) 
								+ (light_attenuation[i].y * distToLight) 
								+ (light_attenuation[i].z * distToLight * distToLight);
		vec3 unit_light = normalize(toward_lights[i]);
		float lightIntensity = saturate(dot(unit_normal, unit_light))/attenuationFactor;
		
		vec3 reflectedDir = reflect( -unit_light, unit_normal );
		
		float specularFactor = max(dot(reflectedDir, unit_camera), 0);
		float dampedFactor = pow( specularFactor, specular_exponent );
		
		vec3 light = lightIntensity * light_colors[i];
		vec3 specular = light * specular_color * specular_reflection * dampedFactor;
		
		sum_light += light;
		sum_specular += specular;
	}

    vec4 albedo = vec4(diffuse_color, 1.0-transparency);
	frag_colour = vec4(sum_light, 1.0) * albedo + vec4(sum_specular, 1.0);
	frag_colour = mix( vec4(fog_colour,1), frag_colour, fog_fraction );
	frag_colour = vec4(pow(frag_colour.rgb, vec3(1.0/2.2)), frag_colour.a);
}
