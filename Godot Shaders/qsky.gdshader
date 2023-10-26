// Quake-style Sky
// December 26, 2021
// By Tim McCabe for Blue Key Games
// @RhapsodyInGeek, @BlueKeyGames
shader_type spatial;
render_mode shadows_disabled;
render_mode depth_draw_always;
render_mode unshaded;
// Takes a texture, cuts it in half and maps it to a sphere infinitely far away
// Left half of the texture is the back / base, right half is the front, usually with transparency
// We can specify different scroll speeds to give a parallax effect

uniform sampler2D alb: filter_nearest_mipmap, hint_default_black, source_color; // Make sure the texture is a multiple of 2x1; Quake used 256x128
uniform vec2 scroll_speed_b = vec2(0.01);
uniform vec2 scroll_speed_f = vec2(0.05);
uniform float sky_height = 4.0; // Sky height is a little misleading; higher numbers flatten the sky more
uniform float sky_scale = 3.0; // Texture scale; higher numbers = more repeats

void fragment(){
	// Initial sphere map
	vec3 n = VERTEX * mat3(INV_VIEW_MATRIX[0].xyz, INV_VIEW_MATRIX[1].xyz, INV_VIEW_MATRIX[2].xyz);
	n.y *= sky_height;
	n = normalize(n) * sky_scale;
	// Back albedo
	vec2 s = scroll_speed_b * TIME;
	vec2 u = vec2(n.x + 0.5 + s.x, n.z + s.y);
	vec4 alb_b = texture(alb, vec2(mod(u.x, 0.5), u.y));
	// Front albedo
	s = scroll_speed_f * TIME;
	u = vec2(n.x + 0.5 + s.x, n.z + s.y);
	vec4 alb_f = texture(alb, vec2(mod(u.x, 0.5) + 0.5,u.y));
	// Mixed albedo
	vec3 frag = mix(alb_b.rgb, alb_f.rgb, 1.0*alb_f.a);
	ALBEDO = frag.rgb;
}