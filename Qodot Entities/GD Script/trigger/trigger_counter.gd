#*******************************************************************************
# TRIGGER COUNTER
#
# Whenever this trigger is triggered by other entities, it increments a counter.
# When that counter reaches the specified amount, it triggers its target.
# Can optionally repeat the counter.
#*******************************************************************************
@tool
class_name TriggerCounter
extends Node

enum {
	TRIGGER_FLAG_PLAYER_ONLY = (1 << 0),
	TRIGGER_FLAG_ON_START = (1 << 1),
	TRIGGER_FLAG_NO_TOUCH = (1 << 2),
	TRIGGER_FLAG_CONTINUOUS = (1 << 3)
}

enum TriggerStates { 
	READY, 
	DELAYED, 
	TRIGGERED, 
	REMOVED 
}

@export var properties: Dictionary:
	set(value):
		properties = value
		if !Engine.is_editor_hint():
			return
		# Trigger
		if properties.has("appearance_flags"):
			appearance_flags = properties["appearance_flags"] as int
		if properties.has("flags"):
			flags = properties["flags"] as int
		if properties.has("targetname"):
			targetname = properties["targetname"] as String
		if properties.has("target"):
			target = properties["target"] as String
		if properties.has("targetfunc"):
			targetfunc = properties["targetfunc"] as String
		if properties.has("message"):
			message = String(properties["message"]).replace("  ", " ")
		if properties.has("delay"):
			delay = properties["delay"] as float
		# Counter
		if properties.has("count"):
			count = properties["count"] as int
		if properties.has("repeat"):
			repeat = properties["repeat"] as int
		if properties.has("count_message"):
			count_message = String(properties["count_message"]).replace("  ", " ")
		if properties.has("count_up"):
			count_up = properties["count_up"] as bool

# Trigger
@export var appearance_flags: int = 0
@export var flags: int = TRIGGER_FLAG_PLAYER_ONLY
@export var trigger_state: int = TriggerStates.READY
@export var targetname: String = ""
@export var target: String = ""
@export var targetfunc: String = ""
@export var message: String = ""
@export var delay: float = 0.0
var timeout: float = 0.0
var last_activator: Node = null
var trigger_callable: Callable
# Counter
@export var count: int = 1
var count_ct: int = 0
@export var repeat: int = 0
@export var count_message: String = ""
@export var count_up: bool = false

func use(activator: Node) -> void:
	if trigger_state == TriggerStates.READY and count_ct > -1:
		count_ct += 1
		if count_ct >= count:
			last_activator = activator
			if delay > 0.0:
				trigger_state = TriggerStates.DELAYED
				timeout = delay
			else:
				_use2()
		elif count_message != "":
			GAME.snd_msg_hint()
			var curr_cnt: Array = [];
			if count_up:
				curr_cnt.append(String.num(count_ct))
			else:
				curr_cnt.append(String.num(count - count_ct))
			GAME.message(count_message, [String.num(count - count_ct)])

func _use2() -> void:
	if count_ct >= count:
		trigger_state = TriggerStates.TRIGGERED
		if target.find("ACTIVATOR") != -1 and last_activator != null: # target keyword, allows us to specifically trigger the one that activated us
			last_activator.add_to_group("ACTIVATOR")
		GAME.use_targets(self, target)
		last_activator.call_deferred("remove_from_group", "ACTIVATOR")

		if repeat > 0:
			repeat -= 1
		if repeat != 0:
			count_ct = 0
			trigger_state = TriggerStates.READY
		else:
			count_ct = -1

func get_last_activator() -> Node:
	return last_activator

func data_save() -> Dictionary:
	var data: Dictionary = {}
	data["trigger_state"] = trigger_state
	data["timeout"] = timeout
	data["last_activator"] = last_activator.get_path()
	data["count_ct"] = count_ct
	data["repeat"] = repeat
	return data

func data_load(data: Dictionary) -> void:
	trigger_state = data["trigger_state"] as int
	timeout = data["timeout"] as float
	data["last_activator"] = get_node_or_null(NodePath(data["last_activator"]))
	count_ct = data["count_ct"] as int
	repeat = data["repeat"] as int

func _init() -> void:
	add_to_group("TRIGGER")
	add_to_group("SAV")

func _ready() -> void:
	if !Engine.is_editor_hint():
		if GAME.appearance_check(appearance_flags):
			queue_free()
			return
		call("apply_flags")
		if targetname != "":
			GAME.set_targetname(self, targetname)
		if (flags & TRIGGER_FLAG_ON_START):
			call_deferred("_use2")

func _process(_delta: float) -> void:
	if trigger_state == TriggerStates.DELAYED:
		if timeout > 0.0:
			timeout -= GAME.dt_idle
		else:
			_use2()
