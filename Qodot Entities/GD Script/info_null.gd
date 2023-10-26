#*******************************************************************************
# INFO NULL
#
# Used for targeting and other reference stuff.
#*******************************************************************************
class_name InfoNull
extends Node3D

@export var properties: Dictionary = {} :
	set(value):
		properties = value
		if !Engine.is_editor_hint():
			return
		rotation_degrees = GameManager.demangler(properties)
		# Used for entity modification on map build, since targetnames don't get applied until runtime
		if properties.has("name") and properties["name"] != "":
			name = properties["name"] as String
		# Prefer targetname for in-game stuff
		if properties.has("targetname"):
			targetname = properties["targetname"] as String
	get:
		return properties

@export var targetname: String = ""

func _ready() -> void:
	if !Engine.is_editor_hint():
		GAME.set_targetname(self, targetname)
