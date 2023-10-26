# This GDScript 2 function helps reorient Point Entities imported from Trenchbroom via Qodot that utilize the "mangle" key value pair.
# This assumes your entities are intended to be -Z forward as per Godot's position system.
# Light and Info_Intermission entities have special orientations in Trenchbroom. If you utilize those key prefixes
# in your entity classnames, you'll want to specify the other mangle type.

static func demangler(properties: Dictionary) -> Vector3:
	if properties.has("mangle") or properties.has("angles"):
		var mangle: Vector3 = Vector3.ZERO
		var key: String = "mangle" if properties.has("mangle") else "angles"
		if properties[key] is Vector3:
			mangle = properties[key]
		elif properties["mangle"] is String:
			var arr: PackedFloat64Array = (properties[key] as String).split_floats(" ")
			for i in maxi(arr.size(), 3):
				mangle[i] = arr[i]
		var classname: String = properties["classname"] as String
		if classname.begins_with("light"):
			return Vector3(mangle.y, mangle.x + 180.0, -mangle.z)
		if classname == "info_intermission":
			return Vector3(-mangle.x, mangle.y + 180.0, -mangle.z)
		return Vector3(mangle.x, mangle.y + 180.0, -mangle.z) # actors, items, etc...
	elif properties.has("angle"):
		return Vector3(0.0, (properties["angle"] as float) + 180.0, 0.0)
	return Vector3(0.0, 180.0, 0.0)