
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;
uniform float u_mask;
uniform float u_minimap;
uniform vec3 u_camera_pos;

void main()
{
	vec4 fog_color = vec4(0.8, 0.8, 0.8, 0.8);
	vec2 uv = v_uv;

	vec4 final_color = u_color * texture2D( u_texture, uv ) + u_mask * vec4(0.5, 0.5, 0.0, 0.6);

	float dist = length(u_camera_pos - v_world_position);
	float fogMaxDist = 200.0;
	float fogMinDist = 20.0;
	float fog_factor = clamp(1.0 - (fogMaxDist - dist) / (fogMaxDist - fogMinDist),
		0.0, 1.0 );
	if(u_minimap == 1.0) fog_factor = 0.0;
	gl_FragColor = mix( final_color, fog_color, fog_factor );

}
