#*******************************************************************************
# TRIGGER: USE
#
# A player can press the use key to trigger this.
#*******************************************************************************
@tool
class_name TriggerUse
extends Trigger

func reset_trigger() -> void:
	toggle_collision(true)
	trigger_state = TriggerStates.READY

func _init() -> void:
	super._init()
	add_to_group("TRIGGER_USE")
	if is_connected("body_entered", trigger_callable):
		disconnect("body_entered", trigger_callable)
	collision_layer = GameManager.USE_LAYER
	monitoring = false
	monitorable = true
