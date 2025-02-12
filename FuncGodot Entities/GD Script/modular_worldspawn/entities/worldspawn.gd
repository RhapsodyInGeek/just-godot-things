@tool
extends WorldEnvironment

@export var mapname: String = ""
@export var music: String = ""

func _func_godot_apply_properties(props: Dictionary) -> void:
	mapname = props["mapname"] as String
	music = props["music"] as String
	
	var env: Environment = Environment.new()
	
	#region Background
	# Background
	if props.has["bg_mode"]:
		env.background_mode = Environment.BGMode
	else:
		env.background_mode = Environment.BG_COLOR
	if props.has("bg_color"):
		env.background_color = props["bg_color"] as Color
	else:
		env.background_color = Color.BLACK
	if props.has("bg_energy_multiplier"):
		env.background_energy_multiplier = props["bg_energy_multiplier"] as float
	else:
		env.background_energy_multiplier = 1.0
	if env.background_mode == Environment.BG_CANVAS:
		if props.has("bg_canvas_max_layer"):
			env.background_canvas_max_layer = props["bg_canvas_max_layer"] as int
		else:
			env.background_canvas_max_layer = 0
	if env.background_mode == Environment.BG_CAMERA_FEED:
		if props.has("bg_camera_feed_id"):
			env.background_camera_feed_id = props["bg_camera_feed_id"] as int
		else:
			env.background_camera_feed_id = 1
	#endregion
	
	#region Sky
	if env.background_mode == Environment.BG_SKY:
		var sky: Sky = Sky.new()
		if props.has("sky_material"):
			if ResourceLoader.exists(props["sky_material"] as String):
				var sky_mat: Resource = load(props["sky"] as String)
				if sky_mat is ShaderMaterial or sky_mat is PanoramaSkyMaterial or sky_mat is ProceduralSkyMaterial or sky_mat is PhysicalSkyMaterial:
					sky.sky_material = sky_mat
		if props.has("sky_radiance_size"):
			sky.radiance_size = props["sky_radiance_size"] as Sky.RadianceSize
		else:
			sky.radiance_size = Sky.RADIANCE_SIZE_256
		if props.has("sky_process_mode"):
			sky.process_mode = props["sky_process_mode"] as Sky.ProcessMode
		else:
			sky.process_mode = Sky.PROCESS_MODE_AUTOMATIC
		if props.has("sky_fov"):
			env.sky_custom_fov = props["sky_fov"] as float
		else:
			env.sky_custom_fov = 0.0
		if props.has("sky_rotation"):
			env.sky_rotation = props["sky_rotation"] as Vector3
		else:
			env.sky_rotation = Vector3.ZERO
	#endregion
	
	#region Ambient Light
	if props.has("ambient_light_source"):
		env.ambient_light_source = props["ambient_light_source"] as Environment.AmbientSource
	else:
		env.ambient_light_source = Environment.AMBIENT_SOURCE_COLOR
	if env.ambient_light_source != Environment.AMBIENT_SOURCE_DISABLED:
		if props.has("ambient_light_color"):
			env.ambient_light_color = props["ambient_light_color"] as Color
		else:
			env.ambient_light_color = Color.WHITE
		if env.background_mode != Environment.AMBIENT_SOURCE_COLOR:
			if props.has("ambient_light_sky_contribution"):
				env.ambient_light_sky_contribution = props["ambient_light_sky_contribution"] as float
			else:
				env.ambient_light_sky_contribution = 1.0
		if props.has("ambient_light_energy"):
			env.ambient_light_energy = props["ambient_light_energy"] as float
		else:
			env.ambient_light_energy = 1.0
	#endregion
	
	#region Reflected Light
	if props.has("reflected_light_source"):
		env.reflected_light_source = props["reflected_light_source"] as Environment.ReflectionSource
	else:
		env.reflected_light_source = Environment.REFLECTION_SOURCE_DISABLED
	#endregion
	
	#region Tonemap
	if props.has("tonemap_mode"):
		env.tonemap_mode = props["tonemap_mode"] as Environment.ToneMapper
	else:
		env.tonemap_mode = Environment.TONE_MAPPER_LINEAR
	if props.has("tonemap_exposure"):
		env.tonemap_exposure = props["tonemap_exposure"] as float
	else:
		env.tonemap_exposure = 1.0
	if env.tonemap_mode != Environment.TONE_MAPPER_LINEAR:
		if props.has("tonemap_white"):
			env.tonemap_white = props["tonemap_white"] as float
		else:
			env.tonemap_white = 1.0
	#endregion
	
	#region Screen Space Reflections
	if props.has("ssr_enabled"):
		env.ssr_enabled = props["ssr_enabled"] as bool
	else:
		env.ssr_enabled = false
	if env.ssr_enabled:
		if props.has("ssr_max_steps"):
			env.ssr_max_steps = props["ssr_max_steps"] as int
		else:
			env.ssr_max_steps = 64
		if props.has("ssr_fade_in"):
			env.ssr_fade_in = props["ssr_fade_in"] as float
		else:
			env.ssr_fade_in = 0.15
		if props.has("ssr_fade_out"):
			env.ssr_fade_out = props["ssr_fade_out"] as float
		else:
			env.ssr_fade_out = 2.0
		if props.has("ssr_depth_tolerance"):
			env.ssr_depth_tolerance = props["ssr_depth_tolerance"] as float
		else:
			env.ssr_depth_tolerance = 0.2
	#endregion
	
	#region Screen Space Ambient Occlusion
	if props.has("ssao_enabled"):
		env.ssao_enabled = props["ssao_enabled"] as bool
	else:
		env.ssao_enabled = false
	if env.ssao_enabled:
		if props.has("ssao_radius"):
			env.ssao_radius = props["ssao_radius"] as float
		else:
			env.ssao_radius = 1.0
		if props.has("ssao_intensity"):
			env.ssao_intensity = props["ssao_intensity"] as float
		else:
			env.ssao_intensity = 2.0
		if props.has("ssao_power"):
			env.ssao_power = props["ssao_power"] as float
		else:
			env.ssao_power = 1.5
		if props.has("ssao_detail"):
			env.ssao_detail = props["ssao_detail"] as float
		else:
			env.ssao_detail = 0.5
		if props.has("ssao_horizon"):
			env.ssao_horizon = props["ssao_horizon"] as float
		else:
			env.ssao_horizon = 0.06
		if props.has("ssao_sharpness"):
			env.ssao_sharpness = props["ssao_sharpness"] as float
		else:
			env.ssao_sharpness = 0.98
		if props.has("ssao_light_affect"):
			env.ssao_light_affect = props["ssao_light_affect"] as float
		else:
			env.ssao_light_affect = 0.0
		if props.has("ssao_ao_channel_affect"):
			env.ssao_ao_channel_affect = props["ssao_ao_channel_affect"] as float
		else:
			env.ssao_ao_channel_affect = 0.0
	#endregion
	
	#region Screen Space Indirect Lighting
	if props.has("ssil_enabled"):
		env.ssil_enabled = props["ssil_enabled"] as bool
	else:
		env.ssil_enabled = false
	if env.ssil_enabled:
		if props.has("ssil_radius"):
			env.ssil_radius = props["ssil_radius"] as float
		else:
			env.ssil_radius = 5.0
		if props.has("ssil_intensity"):
			env.ssil_intensity = props["ssil_intensity"] as float
		else:
			env.ssil_intensity = 1.0
		if props.has("ssil_sharpness"):
			env.ssil_sharpness = props["ssil_sharpness"] as float
		else:
			env.ssil_sharpness = 0.98
		if props.has("ssil_normal_rejection"):
			env.ssil_normal_rejection = props["ssil_normal_rejection"] as float
		else:
			env.ssil_normal_rejection = 1.0
	#endregion
	
	#region Signed Distance Field Global Illumination
	if props.has("sdfgi_enabled"):
		env.sdfgi_enabled = props["sdfgi_enabled"] as bool
	else:
		env.sdfgi_enabled = false
	if env.sdfgi_enabled:
		if props.has("sdfgi_use_occlusion"):
			env.sdfgi_use_occlusion = props["sdfgi_use_occlusion"] as bool
		else:
			env.sdfgi_use_occlusion = false
		if props.has("sdfgi_read_sky_light"):
			env.sdfgi_read_sky_light = props["sdfgi_read_sky_light"] as bool
		else:
			env.sdfgi_read_sky_light = true
		if props.has("sdfgi_bounce_feedback"):
			env.sdfgi_bounce_feedback = props["sdfgi_bounce_feedback"] as float
		else:
			env.sdfgi_bounce_feedback = 0.5
		if props.has("sdfgi_cascades"):
			env.sdfgi_cascades = props["sdfgi_cascades"] as int
		else:
			env.sdfgi_cascades = 4
		if props.has("sdfgi_min_cell_size"):
			env.sdfgi_min_cell_size = props["sdfgi_min_cell_size"] as float
		else:
			env.sdfgi_min_cell_size = 0.2
		if props.has("sdfgi_cascade0_distance"):
			env.sdfgi_cascade0_distance = props["sdfgi_cascade0_distance"] as float
		else:
			env.sdfgi_cascade0_distance = 12.8
		if props.has("sdfgi_max_distance"):
			env.sdfgi_max_distance = props["sdfgi_max_distance"] as float
		else:
			env.sdfgi_max_distance = 204.8
		if props.has("sdfgi_y_scale"):
			env.sdfgi_y_scale = props["sdfgi_y_scale"] as Environment.SDFGIYScale
		else:
			env.sdfgi_y_scale = Environment.SDFGI_Y_SCALE_75_PERCENT
		if props.has("sdfgi_energy"):
			env.sdfgi_energy = props["sdfgi_energy"] as float
		else:
			env.sdfgi_energy = 1.0
		if props.has("sdfgi_normal_bias"):
			env.sdfgi_normal_bias = props["sdfgi_normal_bias"] as float
		else:
			env.sdfgi_normal_bias = 1.1
		if props.has("sdfgi_probe_bias"):
			env.sdfgi_probe_bias = props["sdfgi_probe_bias"] as float
		else:
			env.sdfgi_probe_bias = 1.1
	#endregion
	
	#region Glow
	if props.has("glow_enabled"):
		env.glow_enabled = props["glow_enabled"] as bool
	else:
		env.glow_enabled = false
	if env.glow_enabled:
		for i in range(1, 7):
			var glow_level_key: String = "glow_level_" + str(i)
			var glow_level_prop: String = "glow_level/" + str(i)
			if props.has(glow_level_key):
				env.set(glow_level_prop, props[glow_level_key] as float)
			else:
				match i:
					3, 5:
						env.set(glow_level_prop, 1.0)
					_:
						env.set(glow_level_prop, 0.0)
		if props.has("glow_normalized"):
			env.glow_normalized = props["glow_normalized"] as bool
		else:
			env.glow_normalized = false
		if props.has("glow_intensity"):
			env.glow_intensity = props["glow_intensity"] as float
		else:
			env.glow_intensity = 0.8
		if props.has("glow_strength"):
			env.glow_strength = props["glow_strength"] as float
		else:
			env.glow_strength = 1.0
		if props.has("glow_bloom"):
			env.glow_bloom = props["glow_bloom"] as float
		else:
			env.glow_bloom = 0.0
		if props.has("glow_blend_mode"):
			env.glow_blend_mode = props["glow_blend_mode"] as Environment.GlowBlendMode
		else:
			env.glow_blend_mode = Environment.GLOW_BLEND_MODE_SOFTLIGHT
		if props.has("glow_hdr_threshold"):
			env.glow_hdr_threshold = props["glow_hdr_threshold"] as float
		else:
			env.glow_hdr_threshold = 1.0
		if props.has("glow_hdr_scale"):
			env.glow_hdr_scale = props["glow_hdr_scale"] as float
		else:
			env.glow_hdr_scale = 2.0
		if props.has("glow_hdr_luminance_cap"):
			env.glow_hdr_luminance_cap = props["glow_hdr_luminance_cap"] as float
		else:
			env.glow_hdr_luminance_cap = 12.0
		env.glow_map = null
		if props.has("glow_map_image_path"):
			if ResourceLoader.exists(props["glow_map_image_path"] as String):
				var map_img: Resource = load(props["glow_map_image_path"])
				if map_img is Texture:
					env.glow_map = map_img
			if env.glow_map != null:
				if props.has("glow_map_strength"):
					env.glow_map_strength = props["glow_map_strength"] as float
				else:
					env.glow_map_strength = 0.8
	#endregion
	
	#region Fog
	if props.has("fog_enabled"):
		env.fog_enabled = props["fog_enabled"] as bool
	else:
		env.fog_enabled = false
	if env.fog_enabled:
		if props.has("fog_mode"):
			env.fog_mode = props["fog_mode"] as Environment.FogMode
		else:
			env.fog_mode = Environment.FOG_MODE_EXPONENTIAL
		if props.has("fog_light_color"):
			env.fog_light_color = props["fog_light_color"] as Color
		else:
			env.fog_light_color = Color("848d9b")
		if props.has("fog_light_energy"):
			env.fog_light_energy = props["fog_light_energy"] as float
		else:
			env.fog_light_energy = 1.0
		if props.has("fog_sun_scatter"):
			env.fog_sun_scatter = props["fog_sun_scatter"] as float
		else:
			env.fog_sun_scatter = 0.0
		if props.has("fog_density"):
			env.fog_density = props["fog_density"] as float
		else:
			if env.fog_mode == Environment.FOG_MODE_EXPONENTIAL:
				env.fog_density = 0.01
			else:
				env.fog_density = 1.0
		if props.has("fog_sky_affect"):
			env.fog_sky_affect = props["fog_sky_affect"] as float
		else:
			env.fog_sky_affect = 1.0
		if props.has("fog_height"):
			env.fog_height = props["fog_height"] as float
		else:
			env.fog_height = 0.0
		if props.has("fog_height_density"):
			env.fog_height_density = props["fog_height_density"] as float
		else:
			env.fog_height_density = 0.0
		if env.fog_mode == Environment.FOG_MODE_DEPTH:
			if props.has("fog_depth_curve"):
				env.fog_depth_curve = props["fog_depth_curve"] as float
			else:
				env.fog_depth_curve = 1.0
			if props.has("fog_depth_begin"):
				env.fog_depth_begin = props["fog_depth_begin"] as float
			else:
				env.fog_depth_begin = 10.0
			if props.has("fog_depth_end"):
				env.fog_depth_end = props["fog_depth_end"] as float
			else:
				env.fog_depth_end = 100.0
	#endregion
	
	#region Volumetric Fog
	if props.has("volumetric_fog_enabled"):
		env.volumetric_fog_enabled = props["volumetric_fog_enabled"] as bool
	else:
		env.volumetric_fog_enabled = false
	if env.volumetric_fog_enabled:
		if props.has("volumetric_fog_density"):
			env.volumetric_fog_density = props["volumetric_fog_density"] as float
		else:
			env.volumetric_fog_density = 0.05
		if props.has("volumetric_fog_albedo"):
			env.volumetric_fog_albedo = props["volumetric_fog_albedo"] as Color
		else:
			env.volumetric_fog_albedo = Color.WHITE
		if props.has("volumetric_fog_emission"):
			env.volumetric_fog_emission = props["volumetric_fog_emission"] as Color
		else:
			env.volumetric_fog_emission = Color.BLACK
		if props.has("volumetric_fog_emission_energy"):
			env.volumetric_fog_emission_energy = props["volumetric_fog_emission_energy"] as float
		else:
			env.volumetric_fog_emission_energy = 1.0
		if props.has("volumetric_fog_gi_inject"):
			env.volumetric_fog_gi_inject = props["volumetric_fog_gi_inject"] as float
		else:
			env.volumetric_fog_gi_inject = 1.0
		if props.has("volumetric_fog_anisotropy"):
			env.volumetric_fog_anisotropy = props["volumetric_fog_anisotropy"] as float
		else:
			env.volumetric_fog_anisotropy = 0.2
		if props.has("volumetric_fog_length"):
			env.volumetric_fog_length = props["volumetric_fog_length"] as float
		else:
			env.volumetric_fog_length = 64.0
		if props.has("volumetric_fog_detail_spread"):
			env.volumetric_fog_detail_spread = props["volumetric_fog_detail_spread"] as float
		else:
			env.volumetric_fog_detail_spread = 2.0
		if props.has("volumetric_fog_ambient_inject"):
			env.volumetric_fog_ambient_inject = props["volumetric_fog_ambient_inject"] as float
		else:
			env.volumetric_fog_ambient_inject = 0.0
		if props.has("volumetric_fog_sky_affect"):
			env.volumetric_fog_sky_affect = props["volumetric_fog_sky_affect"] as float
		else:
			env.volumetric_fog_sky_affect = 1.0
		if props.has("volumetric_fog_temporal_reprojection_enabled"):
			env.volumetric_fog_temporal_reprojection_enabled = props["volumetric_fog_temporal_reprojection_enabled"] as bool
		else:
			env.volumetric_fog_temporal_reprojection_enabled = true
		if env.volumetric_fog_temporal_reprojection_enabled:
			if props.has("volumetric_fog_temporal_reprojection_amount"):
				env.volumetric_fog_temporal_reprojection_amount = props["volumetric_fog_temporal_reprojection_amount"] as float
			else:
				env.volumetric_fog_temporal_reprojection_amount = 0.9
	#endregion
	
	#region Adjustments
	if props.has("adjustment_enabled"):
		env.adjustment_enabled = props["adjustment_enabled"] as bool
	else:
		env.adjustment_enabled = false
	if env.adjustment_enabled:
		if props.has("adjustment_brightness"):
			env.adjustment_brightness = props["adjustment_brightness"] as float
		else:
			env.adjustment_brightness = 1.0
		if props.has("adjustment_contrast"):
			env.adjustment_contrast = props["adjustment_contrast"] as float
		else:
			env.adjustment_contast = 1.0
		if props.has("adjustment_saturation"):
			env.adjustment_saturation = props["adjustment_saturation"] as float
		else:
			env.adjustment_saturation = 1.0
		if props.has("adjustment_color_correction_path"):
			if ResourceLoader.exists(props["adjustment_color_correction_path"] as String):
				var cc: Resource = load(props["adjustment_color_correction_path"] as String)
				if cc is Texture2D or cc is Texture3D:
					env.adjustment_color_correction = cc
	#endregion
	
	environment = env
