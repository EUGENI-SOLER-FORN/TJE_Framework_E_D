varying vec2 v_uv;
uniform vec4 u_color;
uniform float u_mask;

void main()
{
	vec4 final_color;
	
	if(0.025 > v_uv.y || v_uv.y > u_mask){
		final_color = vec3(0.1, 0.1, 0.1);
	}
	else if(0.025 > v_uv.x || v_uv.x > 0.975){
		final_color = vec3(0.1, 0.1, 0.1);
	} 
	else {
		final_color = mix(u_color, vec4(0.7*u_color.xyz, 1.0), u_mask);
	}

	gl_FragColor =vec4(final_color * u_color.rgb, 1.0);
}
