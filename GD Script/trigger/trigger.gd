#*******************************************************************************
# TRIGGER VOLUME
#
# Entering this volume will call GAME.use_targets on the target groups.
# Can optionally be activated by non-player Actors.
#*******************************************************************************
@tool
class_name Trigger
extends Area3D

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
		if Engine.is_editor_hint():
			call("update_properties")

@export var appearance_flags: int = 0
@export var flags: int = TRIGGER_FLAG_PLAYER_ONLY
@export var trigger_state: int = TriggerStates.READY
@export var health: int = 0
@export var health_max: int = 0
@export var bleed_type: int = 0
@export var targetname: String = ""
@export var target: String = ""
@export var targetfunc: String = ""
@export var message: String = ""
@export var delay: float = 0.0
@export var wait: float = 0.0
var timeout: float = 0.0
var last_activator: Node = null
var trigger_callable: Callable

func update_properties() -> void:
	if properties.has("moveparent"):
		call_deferred("reparent_entity")
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
	if properties.has("wait"):
		wait = properties["wait"] as float
	var layers: int = GameManager.TRIGGER_LAYER
	if properties.has("health") and int(properties["health"]) > 0:
		health_max = properties["health"] as int
		health = health_max
		layers += GameManager.ACTOR_LAYER
	if properties.has("health_max"):
		health_max = properties["health_max"] as int
	if properties.has("bleed_type"):
		bleed_type = properties["bleed_type"] as int
	collision_mask = layers
	collision_layer = layers
	call("apply_flags")

func apply_flags() -> void:
	if (flags & TRIGGER_FLAG_CONTINUOUS):
		if wait < 0.0:
			wait = 0.0
		if health_max > 0:
			flags &= ~TRIGGER_FLAG_CONTINUOUS

func reparent_entity() -> void:
	var moveparent: String = properties["moveparent"] as String
	moveparent = "entity_func_move_" + moveparent
	if get_parent().name == moveparent:
		return
	if is_inside_tree():
		if get_parent().has_node(NodePath(moveparent)):
			var t: Transform3D = global_transform
			var new_parent: Node = get_parent().get_node(NodePath(moveparent))
			get_parent().remove_child(self)
			new_parent.add_child(self)
			global_transform = t
			owner = new_parent.owner
			for child in get_children():
				child.owner = owner

func damage(_inflictor: Node, attacker: Node, amount: int) -> void:
	if health_max > 0:
		if (flags & TRIGGER_FLAG_PLAYER_ONLY) and !attacker.is_in_group("PLAYER"):
			return
		health -= amount
		if health < 1:
			toggle_collision(false)
			call("use", attacker)

func bleed(hit_xform: Transform3D, _amount: int) -> void:
	var fx: Node3D = GAME.get_bleed(bleed_type)
	add_child(fx)
	fx.transform = Transform3D(hit_xform.basis, fx.to_local(hit_xform.origin))

func toggle_collision(toggle: bool) -> void:
	for child in get_children():
		if child is CollisionShape3D:
			child.set_deferred("disabled", !toggle)

func _on_ent_entered(ent: Node) -> void:
	print(ent.name)
	if trigger_state == TriggerStates.READY:
		if ent is Actor and ent.health > 0:
			if ent.is_in_group("PLAYER") or !(flags & TRIGGER_FLAG_PLAYER_ONLY):
				call("use", ent)

func use(activator: Node) -> void:
	if trigger_state == TriggerStates.READY:
		toggle_collision(false)
		last_activator = activator
		if delay > 0.0:
			trigger_state = TriggerStates.DELAYED
			timeout = delay
		else:
			call("_use2")

func _use2() -> void:
	timeout = wait
	trigger_state = TriggerStates.TRIGGERED
	toggle_collision(false)
	if target.find("ACTIVATOR") != -1 and last_activator != null: # target keyword, allows us to specifically trigger the one that activated us
		last_activator.add_to_group("ACTIVATOR")
	GAME.use_targets(self, target)
	last_activator.call_deferred("remove_from_group", "ACTIVATOR")

func reset_trigger() -> void:
	if !(flags & TRIGGER_FLAG_NO_TOUCH) or health_max > 0:
		toggle_collision(true)
	health = health_max
	trigger_state = TriggerStates.READY

func get_last_activator() -> Node:
	return last_activator

func data_save() -> Dictionary:
	var data: Dictionary = {}
	data["trigger_state"] = trigger_state
	if trigger_state == TriggerStates.TRIGGERED:
		toggle_collision(false)
	data["timeout"] = timeout
	data["health"] = health
	data["last_activator"] = last_activator.get_path()
	return data

func data_load(data: Dictionary) -> void:
	trigger_state = data["trigger_state"] as int
	timeout = data["timeout"] as float
	health = data["health"] as int
	data["last_activator"] = get_node_or_null(NodePath(data["last_activator"]))

func _init() -> void:
	add_to_group("TRIGGER")
	add_to_group("SAV")
	monitoring = true
	monitorable = false
	trigger_callable = Callable(self, "_on_ent_entered")
	connect("body_entered", trigger_callable)

func _ready() -> void:
	if !Engine.is_editor_hint():
		if GAME.appearance_check(appearance_flags):
			queue_free()
			return
		call("apply_flags")
		if (flags & TRIGGER_FLAG_CONTINUOUS) or health > 0:
			if is_connected("body_entered", trigger_callable):
				disconnect("body_entered", trigger_callable)
		elif (flags & TRIGGER_FLAG_NO_TOUCH):
			if is_connected("body_entered", trigger_callable):
				disconnect("body_entered", trigger_callable)
			monitoring = false
			monitorable = false
		GAME.set_targetname(self, targetname)
		if (flags & TRIGGER_FLAG_ON_START):
			call_deferred("_use2")

func state_idle(delta: float) -> void:
	if trigger_state == TriggerStates.TRIGGERED and wait >= 0.0:
		if timeout > 0.0:
			timeout -= delta
		else:
			call_deferred("reset_trigger")
	elif trigger_state == TriggerStates.DELAYED:
		if timeout > 0.0:
			timeout -= delta
		else:
			call("_use2")

func state_physics(delta: float) -> void:
	if (flags & TRIGGER_FLAG_CONTINUOUS) or (GAME.time < 0.1 and is_monitoring()):
		if timeout > 0.0:
			timeout -= delta
		else:
			var ents: Array = get_overlapping_bodies()
			for e in ents:
				call("_on_ent_entered", e)

func _process(_delta: float) -> void:
	if Engine.is_editor_hint():
		return
	if GAME.gamestate != GameManager.GameStates.LEVEL:
		return
	call("state_idle", GAME.dt_idle)

func _physics_process(_delta: float) -> void:
	if Engine.is_editor_hint():
		return
	if GAME.gamestate != GameManager.GameStates.LEVEL:
		return
	call("state_physics", GAME.dt_phys)
