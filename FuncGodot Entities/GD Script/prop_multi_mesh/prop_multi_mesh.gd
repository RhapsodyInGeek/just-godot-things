@tool
class_name PropMultiMesh
extends MultiMeshInstance3D

@export var populate_multi_mesh: bool :
	set(val):
		_populate_multi_mesh()

@export var target: String = ""

@export var targetname: String = ""

@export var target_mesh: String = ""

@export var mesh_res_path: String = ""

@export var amount: int = 128 :
	set(val):
		amount = max(val, 0)

@export var random_rotation: float = 0.0 :
	set(val):
		random_rotation = abs(val)

@export var random_tilt: float = 0.0 :
	set(val):
		random_tilt = abs(val)

@export var base_scale: float = 1.0 :
	set(val):
		base_scale = max(abs(val), 0.01)

@export var random_scale: float = 0.0 :
	set(val):
		random_scale = abs(val)

func _populate_multi_mesh() -> void:
	# Find surface target
	var mesh_targ: Array[MeshInstance3D] = [null, null]
	
	for i in 2:
		var targ_path: NodePath
		if i == 0:
			targ_path = "entity_" + target
		else:
			targ_path = "entity_" + target_mesh
		
		if get_parent().has_node(targ_path):
			var temp: Node = get_parent().get_node(targ_path)
			if temp is MeshInstance3D:
				mesh_targ[i] = temp as MeshInstance3D
			elif temp.get_child_count() > 0:
				for child in temp.get_children():
					if child is MeshInstance3D:
						mesh_targ[i] = child as MeshInstance3D
						break
	
	# No valid surface target! Find one within range
	if !mesh_targ[0] or !mesh_targ[0].mesh:
		var world_ents: Array[Node] = get_tree().get_nodes_in_group("WORLD")
		for ent in world_ents:
			for child in ent.get_children():
				if child is MeshInstance3D and child.mesh:
					if child.global_position.distance_to(global_position) < 8.0:
						mesh_targ[0] = child
						break
			if mesh_targ[0]:
				break
		if !mesh_targ[0]:
			printerr("No target found for PropMultiMesh " + name)
			return
	
	# Cache the mesh
	var mesh: Mesh = null
	if mesh_targ[1] != null:
		mesh = mesh_targ[1].mesh
	if !mesh and FileAccess.file_exists(mesh_res_path):
		mesh = load(mesh_res_path)
	if !mesh:
		printerr("No valid mesh set for PropMultiMesh " + name)
		return
	
	# Prepare target surface
	var geo_xform: Transform3D = global_transform.affine_inverse() * mesh_targ[0].global_transform
	var verts: PackedVector3Array = mesh_targ[0].mesh.get_faces()
	if verts.size() == 0 or verts.size() % 3 != 0:
		printerr("PropMultiMesh " + name + " target has invalid mesh")
		return
	
	var faces: Array[MeshFace] = []
	faces.resize(verts.size() / 3)
	var facecount: int = faces.size()
	for i in facecount:
		faces[i] = MeshFace.new(verts[i*3], verts[i*3+1], verts[i*3+2])
		for j in 3:
			faces[i].vertex[j] = geo_xform * faces[i].vertex[j]
	
	var area_accum: float = 0.0
	var triangle_area_map: Dictionary = {}
	for i in facecount:
		var area: float = faces[i].get_area()
		if area < 0.00001:
			continue
		triangle_area_map[area_accum] = i
		area_accum += area
	
	if triangle_area_map.is_empty() or area_accum == 0:
		printerr("PropMultiMesh " + name + " couldn't map area")
		return
	
	# Generate the MultiMesh resource
	multimesh = MultiMesh.new()
	multimesh.transform_format = MultiMesh.TRANSFORM_3D
	multimesh.instance_count = amount
	multimesh.mesh = mesh
	
	# Place instances
	for i in amount:
		var area_pos: float = randf_range(0, area_accum)
		var area_key: float = 0
		for key in triangle_area_map.keys():
			if abs(key - area_pos) < abs(area_key - area_pos):
				area_key = key
		var index: int = wrapi(triangle_area_map[area_key], 0, facecount)
		
		var face: MeshFace = faces[index]
		
		var pos: Vector3 = face.get_random_point_inside()
		var normal: Vector3 = face.get_plane().normal
		var op_axis: Vector3 = (face.vertex[0] - face.vertex[1]).normalized()
		
		var xform: Transform3D
		xform.origin = pos
		xform = xform.looking_at(pos + op_axis, normal)
		
		var rot: float = deg_to_rad(random_rotation)
		var tilt: float = deg_to_rad(random_tilt)
		
		var post_xform: Basis
		post_xform = post_xform.rotated(post_xform.y, randf_range(-rot, rot))
		post_xform = post_xform.rotated(post_xform.z, randf_range(-tilt, tilt))
		post_xform = post_xform.rotated(post_xform.x, randf_range(-tilt, tilt))
		xform.basis = post_xform * xform.basis
		xform.basis *= max(abs(base_scale + randf_range(-random_scale, random_scale)), 0.01)
		
		multimesh.set_instance_transform(i, xform)

func _func_godot_apply_properties(props: Dictionary) -> void:
	if props.has("target"):
		target = props["target"] as String
	if props.has("target_mesh"):
		target_mesh = props["target_mesh"] as String
	if props.has("targetname"):
		targetname = props["targetname"] as String
	if props.has("mesh_res_path"):
		mesh_res_path = props["mesh_res_path"] as String
	if props.has("amount"):
		amount = max((props["amount"] as int), 0)
	if props.has("random_rotation"):
		random_rotation = abs(props["random_rotation"] as float)
	if props.has("random_tilt"):
		random_tilt = props["random_tilt"] as float
	if props.has("base_scale"):
		base_scale = maxf(abs(props["base_scale"] as float), 0.01)
	if props.has("random_scale"):
		random_scale = abs(props["random_scale"] as float)
	
	_populate_multi_mesh()

func _init() -> void:
	add_to_group("prop_multi_mesh")

