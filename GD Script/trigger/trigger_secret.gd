#******************************************************
# TRIGGER SECRET
#
# Trigger Secrets get tallied at the end of a map by an end of level entity
# that compares number of secrets to the number that have `secret_found == true`.
# Pretty simple stuff, but thought it was worth showing here anyway.
#******************************************************
@tool
class_name TriggerSecret
extends Trigger

var secret_found: bool = false

func is_secret_found() -> bool:
	return secret_found

func _use2():
	if !secret_found:
		trigger_state = TriggerStates.TRIGGERED
		secret_found = true
		GAME.snd_notif_secret()
		if message == "":
			message = "$secretfound"
		GAME.message(message)
		toggle_collision(false)

func data_save() -> Dictionary:
	var data: Dictionary = super.data_save()
	data["secret_found"] = secret_found
	return data

func data_load(data: Dictionary) -> void:
	super.data_load(data)
	secret_found = data["secret_found"]
	if secret_found:
		toggle_collision(false)

func _init() -> void:
	super._init()
	add_to_group("SECRET")
