// Quake-style Button Material
// Used with FuncButton entities to display appropriate activation states.
shader_type spatial;
render_mode depth_draw_always;

uniform sampler2D on_alb: filter_nearest_mipmap, source_color;
uniform sampler2D on_glow: filter_nearest_mipmap, hint_default_black, source_color;
uniform sampler2D off_alb: filter_nearest_mipmap, source_color;
uniform sampler2D off_glow: filter_nearest_mipmap, hint_default_black, source_color;
uniform bool button_pressed = false;

void fragment(){
	vec4 alb;
	vec4 glow;
	if (button_pressed == true){
		alb = texture(off_alb,UV);
		glow = texture(off_glow,UV);
	}
	else {
		alb = texture(on_alb,UV);
		glow = texture(on_glow,UV);
	}
	ALBEDO = alb.rgb;
	SPECULAR = 0.0;
	ROUGHNESS = 1.0;
	ALPHA = alb.a;
	EMISSION = glow.rgb;
}