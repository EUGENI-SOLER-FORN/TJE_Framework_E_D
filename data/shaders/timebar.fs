varying vec2 v_uv;
uniform vec4 u_color;
uniform float u_mask;

void main()
{
	vec4 final_color;
	
	if( 0.05 > v_uv.y || v_uv.y > 0.95){
		final_color = vec4(0.1, 0.1, 0.1, 1.0);
	}
	else if(0.0125 > v_uv.x || v_uv.x > 0.9875){
		final_color = vec4(0.1, 0.1, 0.1, 1.0);
	} 
	else {
		final_color = mix(vec4(1.0, 0.83, 0.0,1.0), vec4(0.27, 0.0, 0.313, 1.0), 2*abs(v_uv.x-0.5));
	}

	gl_FragColor = final_color;
}
