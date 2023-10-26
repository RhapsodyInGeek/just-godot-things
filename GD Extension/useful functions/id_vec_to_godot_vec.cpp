// HEADER
static Vector3 id_vec_to_godot_vec(Variant vec);

// BODY
Vector3 GameManager::id_vec_to_godot_vec(Variant vec) {
	Vector3 org = VECTOR3_ZERO;
	if (vec.get_type() == Variant::VECTOR3)
		org = vec;
	else if (vec.get_type() == Variant::STRING) {
		PackedFloat64Array arr = String(vec).split_floats(" ");
		for (int i = 0; i < Math::max((int)arr.size(), 3); i++)
			org[i] = arr[i];
	}
	return Vector3(org.y, org.z, org.x);
}