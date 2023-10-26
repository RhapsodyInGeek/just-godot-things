#*******************************************************************************
# TRIGGER HURT
#
# Any Actors in this volume get beat up.
#*******************************************************************************
@tool
class_name TriggerHurt
extends Trigger

@export var damg: int = 10
@export var can_gib: bool = true
var ent_timers: Array[TriggerEntityTimer] = []

func update_properties() -> void:
	super.update_properties()
	health_max = 0
	health = 0
	if properties.has("damage"):
		damg = properties["damage"] as int
	if !delay:
		delay = 1.0
	if properties.has("can_gib"):
		can_gib = properties["can_gib"] as bool

func apply_flags() -> void:
	pass

func _on_ent_entered(ent: Node) -> void:
	if ent is Actor:
		if (flags & TRIGGER_FLAG_PLAYER_ONLY) and !(ent.flags & Actor.FL_PLAYER):
			return
		var new_ent: TriggerEntityTimer = TriggerEntityTimer.new()
		new_ent.ent = ent as Actor
		new_ent.delay = 0.0
		ent_timers.append(new_ent)

func _on_ent_exited(ent: Node) -> void:
	for e in ent_timers:
		if e.ent == ent:
			ent_timers.erase(e)
			return

func use(_activator: Node) -> void:
	var ents: Array = get_overlapping_bodies()
	for ent in ents:
		if ent is Actor:
			if (flags & TRIGGER_FLAG_PLAYER_ONLY) and !(ent.flags & Actor.FL_PLAYER):
				continue
			ent.call("damage", self, self, damg)
			ent.call("knockback", GameManager.rand_vec3(), damg * 0.1)

func _init() -> void:
	super._init()
	add_to_group("TRIGGER_HURT")
	monitorable = false
	connect("body_exited", _on_ent_exited)

func state_physics(delta: float) -> void:
	super.state_physics(delta)
	if ent_timers.size() > 0:
		for e in ent_timers:
			e.delay -= delta
			if e.delay < 0.0:
				if e.ent != null:
					if e.ent.health > 0 or can_gib:
						e.ent.call("damage", self, self, damg)
						e.ent.call("knockback", GameManager.rand_vec3(), damg * 0.1)
						e.delay = delay
					else:
						ent_timers.erase(e)
