// Quake-style Water Distortion
//
// Original author: Jason Allen Doucette
// http://xona.com/jason/
// https://www.shadertoy.com/view/MsKXzD
// Original upload to Shader Toy: May 15, 2016
//
// Modified for Godot November 2021 by Tim McCabe for Blue Key Games
// @RhapsodyInGeek, @BlueKeyGames
shader_type spatial;
render_mode depth_draw_always;
render_mode cull_disabled;

uniform sampler2D tex_alb: filter_nearest_mipmap, source_color;
uniform sampler2D tex_glow: filter_nearest_mipmap, hint_default_black, source_color;
uniform float transparency = 0.8;
uniform float speed = 1.0;
// the amount of shearing (shifting of a single column or row)
// 1.0 = entire screen height offset (to both sides, meaning it's 2.0 in total)
uniform vec2 magnitude = vec2(0.066);
// cycle multiplier for a given screen height
// 2*PI = you see a complete sine wave from top..bottom
uniform vec2 sine_cycles = vec2(6.28);

void fragment(){
	vec2 new_uv = UV;
	// the value for the sine has 2 inputs:
	// 1. the time, so that it animates.
	// 2. the y-row, so that ALL scanlines do not distort equally.
	float time = TIME*speed;
	float x_angle = time + new_uv.y * sine_cycles.y;
	float y_angle = time + new_uv.x * sine_cycles.x;
	
	vec2 distort_offset = vec2(sin(x_angle), sin(y_angle)) * magnitude;
	// shear the coordinates
	new_uv += distort_offset;
	
	vec4 frag = texture(tex_alb, new_uv);
	ALBEDO = frag.rgb;
	ALPHA = frag.a * transparency;
	SPECULAR = 0.0;
	ROUGHNESS = 1.0;
	SSS_TRANSMITTANCE_COLOR = vec4(1.0);
	EMISSION = texture(tex_glow, new_uv).rgb;
}