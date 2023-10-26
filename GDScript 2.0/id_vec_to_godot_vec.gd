# This GDScript 2 function converts Quake 1 / TrenchBroom oriented vectors into Godot oriented vectors.
# Useful for custom positioning or movement functions for Qodot entities designed in TrenchBroom.

static func id_vec_to_godot_vec(vec: Variant)->Vector3:
	var org: Vector3 = Vector3.ZERO
	if vec is Vector3:
		org = vec
	elif vec is String:
		var arr: PackedFloat64Array = (vec as String).split_floats(" ")
		for i in max(arr.size(), 3):
			org[i] = arr[i]
	return Vector3(org.y, org.z, org.x)
