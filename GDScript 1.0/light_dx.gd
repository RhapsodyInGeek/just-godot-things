#*******************************************************************************
# LIGHT DX
# Light node governed by Trenchbroom key / value pairs.
#*******************************************************************************
extends Light
tool

#*******************************************************************************
# CLASS VARIABLES
#*******************************************************************************
var style: String = ""
var style_ct: float = 0.0
var energy_1: float = 1.0
var energy_2: float = 0.0
var color_1: Color = Color.white
var color_2: Color = Color.black
# Qodot
export(Dictionary) var properties setget set_properties

#*******************************************************************************
# CLASS FUNCTIONS
#*******************************************************************************
func set_properties(new_properties : Dictionary) -> void:
	if(properties != new_properties):
		properties = new_properties
		update_properties()

func update_properties():
	# Make sure we light both the world visual layer and the view weapon layer
	layers = 3
	light_cull_mask = 3
	set_bake_mode(Light.BAKE_ALL)
	if get_class() == "SpotLight":
		# Spotlight direction
		if "mangle" in properties:
			var mangle:= Vector3()
			if typeof(properties["mangle"]) == TYPE_STRING:
				var m: PoolStringArray = properties["mangle"].split(" ")
				mangle = Vector3(float(m[0]),float(m[1]),float(m[2]))
			elif typeof(properties["mangle"]) == TYPE_VECTOR3:
				mangle = properties["mangle"]
			rotation_degrees = Vector3(mangle.y, mangle.x + 180, mangle.z)
		else:
			rotation_degrees.y = 180
		# Spotlight cone angle
		if "angle" in properties:
			set_param(Light.PARAM_SPOT_ANGLE, properties["angle"])
	# Default energy is 1.0
	if "light" in properties:
		light_negative = properties["light"] < 0
		energy_1 = abs(properties["light"])
		set_param(Light.PARAM_ENERGY, energy_1)
		set_param(Light.PARAM_INDIRECT_ENERGY, energy_1)
	# Used for styles - we can interpolate between 2 energy levels, most useful for controlling subtle pulses or flickers
	if "light2" in properties:
		energy_2 = properties["light2"]
	# Range is 'wait'/32, since 32 Trenchbroom units = 1 meter in Godot
	if "wait" in properties:
		set_param(Light.PARAM_RANGE, properties["wait"] * 0.03125)
	else:
		set_param(Light.PARAM_RANGE, 9.375)
	# Light falloff
	if "delay" in properties:
		set_param(Light.PARAM_ATTENUATION, properties['delay'])
	# Specular? In THIS retro FPS? Bwahahahahahaha
	set_param(Light.PARAM_SPECULAR, 0.0)
	# Shadows can be expensive - use sparingly
	#set_shadow(false)
	if "shadows" in properties:
		set_shadow(bool(properties["shadows"]))
		if properties["shadows"] > 1:
			set_bake_mode(Light.BAKE_INDIRECT)
	# Mood lighting
	if "_color" in properties:
		color_1 = properties["_color"]
		color_2 = color_1
		set_color(color_1)
	# Used for styles - we can interpolate between 2 colors
	if "_color2" in properties:
		color_2 = properties["_color2"]
	# Animation style - please see the style functions below
	if "style" in properties:
		set_style(properties["style"])
	# Lightmap setup
	if style.length() > 1 or "targetname" in properties:
		set_bake_mode(Light.BAKE_DISABLED)
	if "start_off" in properties:
		visible = !properties.start_off
		if properties.start_off:
			set_bake_mode(Light.BAKE_DISABLED)
	if "moveparent" in properties:
		set_bake_mode(Light.BAKE_DISABLED)
		call_deferred("reparent")

func reparent() -> void:
	var moveparent: String = "entity_func_move_" + properties["moveparent"]
	if get_parent().name == moveparent:
		return
	if is_inside_tree():
		if get_parent().has_node(moveparent):
			var pos: Vector3 = global_transform.origin
			var new_parent = get_parent().get_node(moveparent)
			get_parent().remove_child(self)
			new_parent.add_child(self)
			global_transform.origin = pos
			self.owner = new_parent.owner
			self.portal_mode = CullInstance.PORTAL_MODE_GLOBAL

func trigger(_caller_node) -> void:
	visible = !visible

# LIGHT STYLES -----------------------------------------------------------------
func set_style(style_id:int=0) -> void:
	# Flicker 1
	if style_id == 1: style = "mmnmmommommnonmmonqnmmo"
	# Slow Strong Pulse
	elif style_id == 2: style = "abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcba"
	# Candle 1
	elif style_id == 3: style = "mmmmmaaaaammmmmaaaaaabcdefgabcdefg"
	# Fast Strobe
	elif style_id == 4: style = "mamamamamama"
	# Gentle Pulse
	elif style_id == 5: style = "jklmnopqrstuvwxyzyxwvutsrqponmlkj"
	# Flicker 2
	elif style_id == 6: style = "nmonqnmomnmomomno"
	# Candle 2
	elif style_id == 7: style = "mmmaaaabcdefgmmmmaaaammmaamm"
	# Candle 3
	elif style_id == 8: style = "mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa"
	# Slow Strobe
	elif style_id == 9: style = "aaaaaaaazzzzzzzz"
	# Flourescent Flicker
	elif style_id == 10: style = "mmamammmmammamamaaamammma"
	# Slow Pulse, No Fade To Black
	elif style_id == 11: style = "abcdefghijklmnopqrrqponmlkjihgfedcba"
	# 2nd
	elif style_id == 63: style = "a"
	# Stay at energy_1, color_1
	else: style = "m"

func style_proc(delta:float) -> float:
	style_ct += delta
	var i: int = int(style_ct*10) % style.length()
	var t: float = float("abcdefghijklmnopqrstuvwxyz".find(style[i])) / 12.0
	return t

#*******************************************************************************
# BASE PROCESSING
#*******************************************************************************
func _ready():
	if !Engine.is_editor_hint():
		style_ct = 0.0
		#if "targetname" in properties:
		#	GAME.set_node_targetnames(self,properties["targetname"])

func _process(delta):
	if style.length() > 1:
		if style_ct > 10800:
			style_ct = 0
		var t: float = style_proc(delta)
		set_param(Light.PARAM_ENERGY, lerp(energy_2,energy_1,t)*float(visible))
		set_color(lerp(color_2,color_1,t))
