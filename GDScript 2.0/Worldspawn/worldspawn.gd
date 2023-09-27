# An alternative to the built-in worldspawn feature of Qodot. Instead of building map geometry,
# this Worldspawn allows the mapper to customize WorldEnvironment and Lightmap settings from
# TrenchBroom on a map by map basis.

# Unfortunately they removed the ability to bake lightmaps via script in Godot 4, so you'll still 
# have to select the LightmapGI node and bake manually. For this reason, the Lightmap node is automatically
# moved outside of and above the QodotMap node for easier accessibility after build.

# The _ready() function is commented out due to requirements for a GameManager autoload. I left it in
# partly to show how you might use the Worldspawn entity to manage other game settings on a global level.

@tool
extends WorldEnvironment
class_name Worldspawn

const LIGHT_LAYER_MASK: int = 31

@export var properties: Dictionary = {} :
	set(value):
		properties = value;
		if !Engine.is_editor_hint():
			return

		# WORLD ENVIRONMENT
		var env: Environment = Environment.new()
		# Base settings
		env.set_fog_enabled(false);
		env.set_tonemapper(Environment.TONE_MAPPER_FILMIC)
		env.set_glow_enabled(false)
		# Background
		env.set_background(Environment.BG_COLOR);
		if properties.has("color_bg"):
			env.set_bg_color(properties["color_bg"]);
		else:
			env.set_bg_color(Color());
		# Ambient light
		env.set_ambient_source(Environment.AMBIENT_SOURCE_COLOR);
		if properties.has("color_ambient"):
			env.set_ambient_light_color(properties["color_ambient"]);
		else:
			env.set_ambient_light_color(Color.hex(0xFFFFFFFF));
		if properties.has("ambient_light"):
			env.set_ambient_light_energy(properties["ambient_light"]);
		else:
			env.set_ambient_light_energy(0.0);
		env.set_ambient_light_sky_contribution(0.0)
		# Brightness setup
		env.set_adjustment_enabled(true)
		env.set_adjustment_brightness(1.0)
		set_environment(env)
		
		call_deferred("apply_lightmap_properties")
	get:
		return properties

func apply_lightmap_properties()->void:
	# Find existing lightmap, else build a new one
	var lit: LightmapGI
	if get_owner().has_node("lightmap"):
		lit = get_owner().get_node("lightmap")
	else:
		lit = LightmapGI.new()
		lit.set_name("lightmap")
		get_owner().add_child(lit)
		lit.set_owner(get_owner())
	lit.get_parent().call_deferred("move_child", lit, 0);
	lit.set_layer_mask(LIGHT_LAYER_MASK)
	# Bake Quality
	if properties.has("lit_quality"):
		lit.set_bake_quality(properties["lit_quality"] as LightmapGI.BakeQuality)
	else:
		lit.set_bake_quality(LightmapGI.BakeQuality.BAKE_QUALITY_MEDIUM);
	# Bounces
	if properties.has("lit_bounces"):
		lit.set_bounces(properties["lit_bounces"] as int)
	else:
		lit.set_bounces(3);
	# Lightmapper Probes Subdivision
	if properties.has("lit_probes_subdiv"):
		lit.set_generate_probes(properties["lit_probes_subdiv"] as LightmapGI.GenerateProbes)
	else:
		lit.set_generate_probes(LightmapGI.GenerateProbes.GENERATE_PROBES_SUBDIV_8);
	# Use Denoiser
	if properties.has("lit_denoiser"):
		lit.set_use_denoiser(properties["lit_denoiser"] as bool);
	else:
		lit.set_use_denoiser(true);

func set_brightness(brightness: float)->void:
	get_environment().set_adjustment_brightness(brightness)

#func _ready()->void:
#	if !Engine.is_editor_hint():
#		if properties.has("music"):
#			MUSIC.music_play(properties["music"])
#		if properties.has("gravity"):
#			GAME.grav_strength = properties["gravity"] as float
#		else:
#			GAME.grav_strength = GAME.GRAV_STRENGTH
#		GAME.connect("brightness_updated", Callable(self, "set_brightness"))
#		set_brightness(GAME.brightness)
