# This GDScript 2 function converts Quake 1 / TrenchBroom oriented vectors into Godot oriented vectors.
# Useful for custom positioning or movement functions for Qodot entities designed in TrenchBroom.

static func id_vec_to_godot_vec(vec: Variant)->Vector3:
	var org: Vector3 = Vector3.ZERO
	if vec.get_type() == TYPE_VECTOR3:
		org = vec
	elif vec.get_type() == TYPE_STRING:
		var arr: Array[String] = (vec as String).split(" ")
		for i in max(arr.size(), 3):
			org[i] = arr[i].to_float()
	return Vector3(org.y, org.z, org.x)
