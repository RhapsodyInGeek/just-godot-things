#******************************************************************
# FUNC DOOR
#
# Special FuncMove type that automatically opens when approached.
# Don't use with doors that need to open together.
#******************************************************************
@tool
class_name FuncDoor
extends FuncMove

var trigger_area: Area3D

func update_children() -> void:
	super.update_children()
	var m: MeshInstance3D = null
	
	for child in get_children():
		# clean up previous trigger area so we don't keep generating them
		if child is Area3D:
			if child.name == "trigger_area":
				child.queue_free()
		elif child is MeshInstance3D:
			m = child
	
	# Toggle doors don't get trigger areas
	if (flags & Func.FUNC_FLAG_TOGGLE):
		return
	
	# Auto generate a trigger volume to open the door
	trigger_area = null
	if m != null:
		trigger_area = Area3D.new()
		add_child(trigger_area)
		trigger_area.owner = owner
		trigger_area.name = "trigger_area"
		trigger_area.collision_layer = 0
		trigger_area.collision_mask = GameManager.TRIGGER_LAYER
		trigger_area.monitorable = false
		trigger_area.monitoring = true
		var c: CollisionShape3D = CollisionShape3D.new()
		var trigger_shape: BoxShape3D = BoxShape3D.new()
		trigger_shape.set_size(m.get_aabb().size + Vector3.ONE * 1.875) # magic padding number
		c.shape = trigger_shape
		c.name = "trigger_area_col"
		trigger_area.add_child(c)
		c.owner = owner

func use(activator: Node) -> void:
	if (flags & Func.FUNC_FLAG_PLAYER_ONLY) and activator is Actor:
		if !(activator.flags & Actor.FL_PLAYER):
			return
	super.use(activator)

func mv_reverse(ent: Node = null) -> void:
	var ents: Array[Node3D] = trigger_area.get_overlapping_bodies()
	for e in ents:
		if e is Actor:
			return
	super.mv_reverse(ent)

func state_physics(delta: float) -> void:
	super.state_physics(delta)
	if move_state == Func.MoveStates.MOVE:
		trigger_area.global_transform = trg_area_t

func _init() -> void:
	super._init()
	add_to_group("FUNC_DOOR")

func _ready():
	if !Engine.is_editor_hint():
		super._ready()
		if has_node("trigger_area"):
			trigger_area = get_node("trigger_area")
			trg_area_t = global_transform
			trigger_area.connect("body_entered", use)
			trigger_area.connect("body_exited", mv_reverse)
			if (flags & Func.FUNC_FLAG_START_OPEN):
				trigger_area.global_transform = trg_area_t
		wait = 0.0
