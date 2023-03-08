#*******************************************************************************
# TRIGGER ROOM v1.0
# Used in culling. Created in Trenchbroom using a single convex brush.
#*******************************************************************************
extends Room
tool

export(Dictionary) var properties setget set_properties

func set_properties(new_properties : Dictionary) -> void:
	if(properties != new_properties):
		properties = new_properties
		update_properties()

func update_properties() -> void:
	if "targetname" in properties:
		if properties["targetname"] != "":
			name = "Room_" + properties["targetname"]
		else:
			name = "Room_" + str(int(name)) + "*"
	for c in get_children():
		if c is MeshInstance:
			c.name = name + "-bound"
	visible = false
