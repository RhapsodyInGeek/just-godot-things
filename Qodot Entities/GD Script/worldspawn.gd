#************************************************************
# WORLDSPAWN
#
# In Quake, Worldspawn is the first entity created and is always the first entity in the map file.
# In Godot, Worldspawn doesn't do us as much good due to one giant mesh for your level not being optimal.
# Since we use FuncGeo for our map geometry instead but we always generate a Worldspawn entity due to
# the map format, we can reinvent what Worldspawn's role is.
#
# Since Worldspawn generally controls map settings, I figured WorldEnvironment seemed to be the best use
# of Worldspawn. In addition, we can use Worldspawn to also manage other map-wide aspects like lightmapping
# so that we don't have to manage a separate LightmapGI entity.
#
# Hopefully this Worldspawn class can serve as a good example of how you don't have to follow traditional
# design roles and how all Qodot sees is what you show it.
#************************************************************
@tool
class_name Worldspawn
extends WorldEnvironment

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

func apply_lightmap_properties() -> void:
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

func set_brightness(brightness: float) -> void:
	get_environment().set_adjustment_brightness(brightness)

func _ready() -> void:
	if !Engine.is_editor_hint():
		if properties.has("music"):
			MUSIC.music_play(properties["music"])
		if properties.has("gravity"):
			GAME.grav_strength = properties["gravity"] as float
		else:
			GAME.grav_strength = GAME.GRAV_STRENGTH
		GAME.connect("brightness_updated", Callable(self, "set_brightness"))
		set_brightness(GAME.brightness)
