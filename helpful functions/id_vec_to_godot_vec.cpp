/*
This GDExtension function converts Quake 1 / TrenchBroom oriented vectors into Godot oriented vectors.
Useful for custom positioning or movement functions for Qodot entities designed in TrenchBroom.
*/

static Vector3 id_vec_to_godot_vec(Variant vec);

Vector3 id_vec_to_godot_vec(Variant vec) {
	Vector3 org = VECTOR3_ZERO;
	if (vec.get_type() == Variant::VECTOR3)
		org = vec;
	else if (vec.get_type() == Variant::STRING) {
		Array arr = String(vec).split(" ");
		String v = arr[0];
		org.x = v.to_float();
		v = arr[1];
		org.y = v.to_float();
		v = arr[2];
		org.z = v.to_float();
	}
	return Vector3(org.y, org.z, org.x);
}
