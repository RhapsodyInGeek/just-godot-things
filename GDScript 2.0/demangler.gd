# This GDScript 2 function helps reorient Point Entities imported from Trenchbroom via Qodot that utilize the "mangle" key value pair.
# This assumes your entities are intended to be -Z forward as per Godot's position system.
# Light and Info_Intermission entities have special orientations in Trenchbroom. If you utilize those key prefixes
# in your entity classnames, you'll want to specify the other mangle type.

static func demangler(properties: Dictionary, mangle_type: int = 0)->Vector3:
	if properties.has("mangle"):
		var mangle: Vector3 = Vector3.ZERO
		if properties["mangle"] is Vector3:
			mangle = properties["mangle"]
		elif properties["mangle"] is String:
			var arr: Array[String] = (properties["mangle"] as String).split(" ")
			for i in maxi(arr.size(), 3):
				mangle[i] = arr[i].to_float()
		match mangle_type:
			0: mangle = Vector3(mangle.x, mangle.y + 180.0, -mangle.z) # actors, items, etc...
			1: mangle = Vector3(mangle.y, mangle.x + 180.0, -mangle.z) # lights
			2: mangle = Vector3(-mangle.x, mangle.y + 180.0, -mangle.z) # info_intermission
		return mangle
	elif properties.has("angle"):
		return Vector3(0.0, (properties["angle"] as float) + 180.0, 0.0)
	return Vector3(0.0, 180.0, 0.0)
