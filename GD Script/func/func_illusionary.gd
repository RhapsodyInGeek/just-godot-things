#***************************************************************************************
# FUNC ILLUSIONARY
#
# Non-collidable visual instance. Useful for little things like torches, small props, 
# things you don't want mucking up entity collision. Also useful for staircases when 
# paired with a clip brush ramp for collision, in case you don't want to deal with 
# stair step code, or your game doesn't allow for it.
#***************************************************************************************
@tool
class_name FuncIllusionary
extends Node3D

@export var properties: Dictionary = {} :
	set(value):
		properties = value;
			
		if !Engine.is_editor_hint():
			return
		
		if properties.has("appearance_flags"):
			appearance_flags = properties["appearance_flags"] as int
		if properties.has("flags"):
			flags = properties["flags"] as int
		for child in get_children():
			if child is MeshInstance3D:
				if (flags & Func.FUNC_FLAG_NO_CULL):
					child.ignore_occlusion_culling = true
				if properties.has("cast_shadow"):
					child.cast_shadow = properties["cast_shadow"] as int
		if properties.has("moveparent"):
			call_deferred("reparent_entity")

@export var appearance_flags: int = 0
@export var flags: int = 0

func reparent_entity() -> void:
	var moveparent: String = properties["moveparent"]
	moveparent = "entity_func_move_" + moveparent
	if get_parent().get_name() == moveparent:
		return
	if is_inside_tree():
		if get_parent().has_node(NodePath(moveparent)):
			var t: Transform3D = global_transform
			var new_parent: Node = get_parent().get_node(NodePath(moveparent));
			get_parent().remove_child(self);
			new_parent.add_child(self);
			set_global_transform(t);
			set_owner(new_parent.owner);
			for child in get_children():
				child.owner = owner
				if child.get_class() == "MeshInstance3D":
					(child as MeshInstance3D).set_ignore_occlusion_culling(true)

func _init() -> void:
	add_to_group("WORLD")
	add_to_group("FUNC_ILLUSIONARY")

func _ready() -> void:
	if !Engine.is_editor_hint():
		if GAME.appearance_check(appearance_flags):
			queue_free()
