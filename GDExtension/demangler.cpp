/*
This GDExtension function helps reorient Point Entities imported from Trenchbroom via Qodot that utilize the "mangle" key value pair.
This assumes your entities are intended to be -Z forward as per Godot's position system.
Light and Info_Intermission entities have special orientations in Trenchbroom. If you utilize those key prefixes
in your entity classnames, you'll want to specify the other mangle type.
*/
static Vector3 demangler(Dictionary properties, int mangle_type = 0);

Vector3 demangler(Dictionary properties, uint8_t mangle_type) {
	if (properties.has("mangle")) {
		Vector3 mangle = VECTOR3_ZERO;
		if (properties["mangle"].get_type() == Variant::VECTOR3)
			mangle = properties["mangle"];
		else if (properties["mangle"].get_type() == Variant::STRING) {
			String v = properties["mangle"];
			Array arr = v.split(" ");
			v = arr[0];
			mangle.x = v.to_float();
			v = arr[1];
			mangle.y = v.to_float();
			v = arr[2];
			mangle.z = v.to_float();
		}
		if (mangle_type == 0) // actors, items, etc...
			mangle = Vector3(mangle.x, mangle.y + 180.0f, -mangle.z);
		else if (mangle_type == 1) // lights
			mangle = Vector3(mangle.y, mangle.x + 180.0f, -mangle.z);
		else if (mangle_type == 2) // info_intermission
			mangle = Vector3(-mangle.x, mangle.y + 180.0f, -mangle.z);
		return mangle;
	}
	else if (properties.has("angle"))
		return Vector3(0.0f, (float)properties["angle"] + 180.0f, 0.0f);
	return Vector3(0.0f, 180.0f, 0.0f);
}
