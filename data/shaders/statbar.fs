varying vec2 v_uv;
uniform vec4 u_color;
uniform float u_mask;

void main()
{
	vec4 final_color;
	
	if( 0.0125 > 1-v_uv.y || 1-v_uv.y > u_mask - 0.0125 ){
		final_color = vec4(0.1, 0.1, 0.1, 1.0);
	}
	else if(0.05 > v_uv.x || v_uv.x > 0.95){
		final_color = vec4(0.1, 0.1, 0.1, 1.0);
	} 
	else {
		final_color = mix(u_color, vec4(0.7*u_color.rgb, 1.0), u_mask);
	}

	gl_FragColor = final_color;
}
