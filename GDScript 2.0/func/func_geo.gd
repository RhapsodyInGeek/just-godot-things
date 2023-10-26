#*******************************************************************************
# FUNC GEO
#
# Geometry mesh. Used to split up the map geometry for better culling. Best when combined with Skip textures
# to cull the outside faces of geometry to reduce overdraw and provide better editor visibility.
# The FGD resource defaults to Convex Collision, but Concave works quite well in conjunction with smart use of Skip.
#*******************************************************************************
@tool
class_name FuncGeo
extends StaticBody3D

@export var properties: Dictionary = {} :
	set(value):
		properties = value;
		
		if !Engine.is_editor_hint():
			return
		
		if properties.has("appearance_flags"):
			appearance_flags = properties["appearance_flags"] as int
		if properties.has("flags"):
			flags = properties["flags"] as int
		if properties.has("grav_type"):
			grav_type = properties["grav_type"] as int		
		for child in get_children():
			if child is MeshInstance3D:
				if (flags & Func.FUNC_FLAG_NO_CULL):
					child.ignore_occlusion_culling = true
				if properties.has("cast_shadow"):
					child.cast_shadow = properties["cast_shadow"] as int

@export var appearance_flags: int = 0
@export var flags: int = 0
@export var grav_type: int = 0

func _init() -> void:
	add_to_group("WORLD")
	add_to_group("FUNC_GEO")

func _ready() -> void:
	if !Engine.is_editor_hint():
		if GAME.appearance_check(appearance_flags):
			queue_free()
