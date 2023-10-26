// HEADER
static Vector3 demangler(Dictionary properties);

// BODY
Vector3 GameManager::demangler(Dictionary properties) {
	if (properties.has("mangle") || properties.has("angles")) {
		Vector3 mangle = VECTOR3_ZERO;
		String key = (properties.has("mangle")) ? "mangle" : "angles";
		if (properties[key].get_type() == Variant::VECTOR3)
			mangle = properties["mangle"];
		else if (properties[key].get_type() == Variant::STRING) {
			PackedFloat64Array arr = String(properties[key]).split_floats(" ");
			for (int i = 0; i < Math::max((int)arr.size(), 3); i++)
				mangle[i] = arr[i];
		}
		String classname = (String)properties["classname"];
		if (classname.begins_with("light"))
			return Vector3(mangle.y, mangle.x + 180.0f, -mangle.z);
		else if (classname == "info_intermission")
			return Vector3(-mangle.x, mangle.y + 180.0f, -mangle.z);
		else // actors, items, etc...
			return Vector3(mangle.x, mangle.y + 180.0f, -mangle.z);
	}
	else if (properties.has("angle"))
		return Vector3(0.0f, (float)properties["angle"] + 180.0f, 0.0f);
	return Vector3(0.0f, 180.0f, 0.0f);
}