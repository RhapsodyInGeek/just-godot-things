#*******************************************************************************
# TRIGGER OCCLUDER
#
# Used in occlusion culling.
# Created in Trenchbroom by painting an Occluder Texture on one face and Skip textures on all others.
# The OccluderPolygon generation is probably one of my favorite solves for a Qodot entity,
# since it's a conversion of brush entity to 3D polygon to 2D polygon with the correct orientation.
# It also serves as proof to the concept that you really aren't limited by what you can implement
# in the TrenchBroom to Godot Pipeline.
#*******************************************************************************
@tool
class_name TriggerOccluder
extends OccluderInstance3D

@export var properties: Dictionary:
	set(value):
		properties = value
		if !Engine.is_editor_hint():
			return
		for m in get_children():
			if m is MeshInstance3D:
				var mdt: MeshDataTool = MeshDataTool.new()
				mdt.create_from_surface(m.mesh, 0)
				var n: Vector3 = mdt.get_face_normal(0)
				var axis: Vector3 = n.cross(Vector3.FORWARD)
				if axis.length() < 1.0:
					axis = n.cross(Vector3.UP)
				if axis.length() < 1.0:
					axis = n.cross(Vector3.RIGHT)
				var depth: float = 0.0
				# Save the mesh basis before the next part
				var mesh_basis: Basis = m.global_transform.basis
				# Orient the occluder to the face normal
				var b: Basis = transform.basis
				if n != b.y and -n != b.y:
					look_at(to_global(n), global_transform.basis.y)
				elif n != b.z and -n != b.z:
					look_at(to_global(n), global_transform.basis.z)
				elif n != b.x and -n != b.x:
					look_at(to_global(n), global_transform.basis.x)
				# Reorient 
				m.global_transform = Transform3D(mesh_basis, m.global_position)
				# Get the vertex points and convert them from Vector3 to Vector2
				var pts: PackedVector2Array = []
				for i in mdt.get_vertex_count():
					var v: Vector3 = mdt.get_vertex(i)
					v = to_local(v + global_position)
					depth = v.z
					pts.append(Vector2(v.x, v.y))
				var poly: PolygonOccluder3D = PolygonOccluder3D.new()
				poly.set_polygon(pts)
				set_occluder(poly)
				translate(Vector3(0.0, 0.0, depth))
			m.queue_free()
		
		if properties.has("appearance_flags"):
			appearance_flags = properties["appearance_flags"] as int
		if properties.has("moveparent"):
			call_deferred("reparent_entity")

@export var appearance_flags: int = 0

func reparent_entity() -> void:
	var moveparent: String = properties["moveparent"] as String
	moveparent = "entity_func_move_" + moveparent
	if get_parent().name == moveparent:
		return
	if is_inside_tree():
		if get_parent().has_node(NodePath(moveparent)):
			var t: Transform3D = global_transform
			var new_parent: Node = get_parent().get_node(NodePath(moveparent))
			get_parent().remove_child(self)
			new_parent.add_child(self)
			global_transform = t
			owner = new_parent.owner

func _ready() -> void:
	if !Engine.is_editor_hint():
		if GAME.appearance_check(appearance_flags):
			queue_free()
