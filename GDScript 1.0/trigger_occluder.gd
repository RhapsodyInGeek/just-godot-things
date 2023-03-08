#*******************************************************************************
# TRIGGER PORTAL v1.0
# Used in Room culling. Created in Trenchbroom by painting a Portal Texture on
# one face and Skip textures on all others.
#*******************************************************************************
extends Occluder
tool

#*******************************************************************************
# CLASS VARIABLES
#*******************************************************************************
export(Dictionary) var properties setget set_properties

#*******************************************************************************
# CLASS FUNCTIONS
#*******************************************************************************
func set_properties(new_properties : Dictionary) -> void:
	if(properties != new_properties):
		properties = new_properties
		update_properties()

func update_properties() -> void:
	for c in get_children():
		if c is MeshInstance:
			var mdt:= MeshDataTool.new()
			var _err = mdt.create_from_surface(c.mesh,0)
			var n: Vector3 = mdt.get_face_normal(0)
			var axis: Vector3 = n.cross(Vector3.FORWARD)
			if axis.length() < 1:
				axis = n.cross(Vector3.UP)
			if axis.length() < 1:
				axis = n.cross(Vector3.RIGHT)
			var depth: float = 0.0
			# Copy the mesh rotation
			var mesh_basis: Basis = c.global_transform.basis
#			# Orient the portal to the face normal
			if n != transform.basis.y and -n != transform.basis.y:
				look_at(to_global(n), global_transform.basis.y)
			elif n != transform.basis.z and -n != transform.basis.z:
				look_at(to_global(n), global_transform.basis.z)
			elif n != transform.basis.x and -n != transform.basis.x:
				look_at(to_global(n), global_transform.basis.x)
			# Reorient the mesh
			c.global_transform.basis = mesh_basis
			# Get the vertex points and convert them from Vector3 to Vector2
			var pts: PoolVector2Array = []
			for i in mdt.get_vertex_count():
				var v: Vector3 = mdt.get_vertex(i)
				v = to_local(v + global_transform.origin)
				depth = v.z
				pts.append(Vector2(v.x,v.y))
			shape = OccluderShapePolygon.new()
			shape.polygon_points = pts
			if "two_way" in properties:
				if properties["two_way"] < 1:
					shape.two_way = false
				else:
					shape.two_way = true
			translate(Vector3(0,0,depth))
		c.queue_free()
	if "moveparent" in properties:
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
			for c in get_children():
				c.owner = owner
