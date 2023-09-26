# Geo mesh. Used to split up the map geometry for better culling.
# Best when combined with Skip textures to cull the outside faces of
# geometry to reduce overdraw and provide better editor visibility.

@tool
extends StaticBody3D
class_name FuncGeo

const INVERSE_SCALE: float = 1.0 / 32.0

@export var properties: Dictionary = {} :
	set(value):
		properties = value;
		
		if !Engine.is_editor_hint():
			return
		
		if properties.has("flags"):
			flags = properties["flags"] as int
		
		for child in get_children():
			if child.get_class() == "CollisionShape3D":
				(child as CollisionShape3D).shape.margin = 0.01
				
			if child.get_class() == "MeshInstance3D":
				var m: MeshInstance3D = child
				m.set_gi_mode(GeometryInstance3D.GI_MODE_STATIC);
				if properties.has("shadowcast"):
					m.set_cast_shadows_setting((properties["shadowcast"] as GeometryInstance3D.ShadowCastingSetting))
				else:
					m.set_cast_shadows_setting(GeometryInstance3D.SHADOW_CASTING_SETTING_ON)
				if m.mesh.get_class() == "ArrayMesh":
					(m.mesh as ArrayMesh).lightmap_unwrap(Transform3D(), INVERSE_SCALE)
				if (flags & FuncFlags.NO_CULL):
					m.ignore_occlusion_culling = true
	get:
		return properties

enum FuncFlags {
	NO_CULL = (1 << 0)
}
@export var flags: int = 0
