# Non-collidable visual instance. Useful for little things like torches, small props, 
# things you don't want mucking up entity collision. Also useful for staircases when 
# paired with a clip brush ramp for collision, in case you don't want to deal with 
# stair step code, or your game doesn't allow for it.

@tool
extends Node3D
class_name FuncIllusionary

const INVERSE_SCALE: float = 1.0 / 32.0

@export var properties: Dictionary = {} :
	set(value):
		properties = value;
		
		if !Engine.is_editor_hint():
			return
		
		if properties.has("flags"):
			flags = properties["flags"] as int
		
		for child in get_children():
			if child.get_class() == "MeshInstance3D":
				var m: MeshInstance3D = child
				m.set_gi_mode(GeometryInstance3D.GI_MODE_STATIC);
				if properties.has("shadowcast"):
					m.set_cast_shadows_setting((properties["shadowcast"] as GeometryInstance3D.ShadowCastingSetting))
				else:
					m.set_cast_shadows_setting(GeometryInstance3D.SHADOW_CASTING_SETTING_ON)
				if m.mesh.get_class() == "ArrayMesh":
					(m.mesh as ArrayMesh).lightmap_unwrap(Transform3D(), INVERSE_SCALE)
				if (flags & FuncFlags.NO_CULL):
					m.ignore_occlusion_culling = true
		
		if properties.has("moveparent"):
			call_deferred("reparent_to_func_move")
	get:
		return properties

enum FuncFlags {
	NO_CULL = (1 << 0)
}
@export var flags: int = 0

func reparent_to_func_move()->void:
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
