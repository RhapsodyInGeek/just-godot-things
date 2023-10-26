#********************************************************************************************
# FUNC MOVE
#
# Moving brush entity. Used for doors, platforms, rocket ships... pretty much any brush entity that moves. 
# This is a pretty hefty class, but Doors Are Complicated(TM).
#********************************************************************************************
@tool
class_name FuncMove
extends AnimatableBody3D

@export var properties: Dictionary:
	set(value):
		properties = value

		if !Engine.is_editor_hint():
			return
		
		# Parenting
		if properties.has("movename"):
			var movename: String = properties["movename"] as String
			if movename != "":
				name = "entity_func_move_" + movename
		if properties.has("moveparent"):
			call_deferred("reparent_entity")
		# Targeting
		if properties.has("targetname"):
			targetname = properties["targetname"] as String
		# Movement
		if properties.has("hinge_offset"):
			hinge_offset = GameManager.id_vec_to_godot_vec(properties["hinge_offset"])
		else:
			hinge_offset = Vector3.ZERO
		if properties.has("move_pos"):
			move_pos[1] = GameManager.id_vec_to_godot_vec(properties["move_pos"])
			move_pos[1] *= GameManager.INVERSE_SCALE
		if properties.has("move_rot"):
			if properties["move_rot"] is Vector3:
				var r: Vector3 = properties["move_rot"]
				for i in 3:
					move_rot[i] = deg_to_rad(r[i])
		if properties.has("move_scale"):
			move_scale = GameManager.id_vec_to_godot_vec(properties["move_scale"])
		if properties.has("speed"):
			speed = (properties["speed"] as float)
		if properties.has("wait"):
			wait = properties["wait"] as float
		# Flags
		if properties.has("appearance_flags"):
			appearance_flags = properties["appearance_flags"] as int
		if properties.has("flags"):
			flags = properties["flags"] as int
		if properties.has("lock_flags"):
			lock_flags = properties["lock_flags"] as int
		if properties.has("grav_type"):
			grav_type = properties["grav_type"] as int
		# Access
		locked = GameManager.set_lock(lock_flags)
		if properties.has("locked_message"):
			locked_message = String(properties["locked_message"]).replace("  ", " ")
		if properties.has("loop"):
			loop = properties["loop"] as int
		if properties.has("damage"):
			crush_damage = properties["damage"] as int
		# Sound
		if properties.has("sound"):
			move_sound = properties["sound"] as String
		if properties.has("locked_sound"):
			locked_sound = properties["locked_sound"] as String
		if properties.has("unlock_sound"):
			unlock_sound = properties["unlock_sound"] as String
		# Children refresh
		call_deferred("update_children")

@export_group("Flags")
@export var appearance_flags: int = 0
@export var flags: int = 0
@export var lock_flags: int = 0

@export_group("Movement")
@export var move_pos: Array[Vector3] = [Vector3.ZERO, Vector3.ZERO]
@export var move_rot: Vector3 = Vector3.ZERO
@export var move_scale: Vector3 = Vector3.ONE
@export var hinge_offset: Vector3 = Vector3.ZERO
@export var speed: float = 1.0
@export var wait: float = -1.0
@export var loop: int = 0

@export_group("Sound")
@export var move_sound: String = ""
@export var locked_sound: String = ""
@export var unlock_sound: String = ""

@export_group("Misc")
@export var targetname: String = ""
@export var locked: int = 0
@export var locked_message: String = ""
@export var grav_type: int = 0
@export var crush_damage: int = 2

var move_state: int = Func.MoveStates.READY
var wait_ct: float = 0.0
var move_progress: float = 0.0
var move_progress_target: float = 0.0
var crush_ct: float = 0.0
var move_paused: bool = false
var trg_area_t: Transform3D = Transform3D.IDENTITY
var block_area: Area3D # block area used for crush detection
var sfx: Array[AudioStreamPlayer3D] = [null, null]
var s_move: AudioStream = null
var s_locked: AudioStream = null
var s_unlock: AudioStream = null

#**************************************
# SCENE TREE MODIFICATION
#**************************************
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

func update_children() -> void:
	var volume: float = 1.0
	# Block area, for crush detection
	block_area = null
	if !has_node("block_area"):
		block_area = Area3D.new()
		add_child(block_area)
		block_area.owner = owner
		block_area.name = "block_area"
		block_area.collision_layer = 0
		block_area.monitorable = false
		var layers: int = GameManager.ACTOR_LAYER + GameManager.DEAD_LAYER
		block_area.collision_mask = layers
		block_area.monitoring = true
	
	for child in get_children():
		if block_area != null and child is CollisionShape3D:
			var c: CollisionShape3D = child.duplicate()
			block_area.add_child(c)
			c.owner = block_area.owner
			c.shape = c.shape.duplicate()
		elif child is MeshInstance3D:
			child.set_ignore_occlusion_culling(true)
			child.layers = GameManager.CULL_DYNAMIC
			if properties.has("cast_shadow"):
				child.cast_shadow = properties["cast_shadow"] as int
			volume = maxf(child.mesh.get_aabb().get_volume() * 0.5, 3.0)
	
	for i in 2:
		if has_node(NodePath("sfx" + String.num(i))) == false:
			sfx[i] = AudioStreamPlayer3D.new()
			sfx[i].bus = "SfxWorld"
			sfx[i].attenuation_filter_cutoff_hz = 10000
			sfx[i].unit_size = minf(volume, 10.6)
			sfx[i].max_distance = minf(volume * 3.0, 32.0)
			add_child(sfx[i])
			sfx[i].name = "sfx" + String.num(i)
			sfx[i].owner = owner
	
	call_deferred("update_hinge")

func update_hinge() -> void:
	if properties.has("hinge_target") and properties["hinge_target"] != "":
		var h: String = properties["hinge_target"] as String
		if is_inside_tree():
			var n: Node = get_node_or_null(NodePath(h))
			if n != null and n is InfoNull:
				hinge_offset = n.global_position - global_position
		properties.erase("hinge_target")
	
	if hinge_offset != Vector3.ZERO:
		for child in get_children():
			if "position" in child and child.position is Vector3:
				child.position += hinge_offset
		hinge_offset = Vector3.ZERO
		if properties.has("hinge_offset"):
			properties.erase("hinge_offset")


#**************************************
# ACTIVATION
#**************************************
func use(activator: Node) -> void:
	if locked > 0:
		if activator is Trigger:
			activator = activator.last_activator
		if activator != null:
			if GAME.check_lock(activator, self):
				sfx[1].stream = s_locked
				sfx[1].play()
				return
			sfx[1].stream = s_unlock
			sfx[1].play()
			locked = 0
	if !locked and move_progress_target < 1.0:
		mv_forward()

func toggle() -> void:
	if move_progress_target > 0.0:
		mv_reverse()
	else:
		mv_forward()


#**************************************
# ACCESS
#**************************************
func lock() -> void:
	locked = 1
	sfx[1].stream = s_locked
	sfx[1].play()

func unlock() -> void:
	locked = 0
	sfx[1].stream = s_unlock
	sfx[1].play()


#**************************************
# MOVEMENT
#**************************************
func mv_forward() -> void:
	if move_progress_target < 1.0:
		move_state = Func.MoveStates.MOVE
		move_progress_target = 1.0
		play_move_sound()

func mv_reverse(ent: Node = null) -> void:
	if ent != null and (flags & Func.FUNC_FLAG_PLAYER_ONLY) and !ent.is_in_group("PLAYER"):
		return
	if move_progress_target > 0.0:
		move_state = Func.MoveStates.MOVE
		move_progress_target = 0.0
		play_move_sound()

func mv_pause(set_pause: bool = true) -> void:
	move_paused = set_pause

func stop(move_targ: float = 0.0) -> void:
	loop = 0
	move_progress_target = move_targ

func stop_at_end() -> void:
	stop(1.0)

func stop_at_pos() -> void:
	stop(move_progress_target)

func loop_no() -> void:
	loop = GameManager.LoopTypes.LOOP_NO

func loop_pong() -> void:
	loop = GameManager.LoopTypes.LOOP_PINGPONG
	play_move_sound()
	move_state = Func.MoveStates.MOVE

func loop_wrap() -> void:
	loop = GameManager.LoopTypes.LOOP_WRAP
	move_progress_target = 1.0
	play_move_sound()
	move_state = Func.MoveStates.MOVE

func loop_wrap_reverse() -> void:
	loop = GameManager.LoopTypes.LOOP_WRAP
	move_progress_target = -1.0
	play_move_sound()
	move_state = Func.MoveStates.MOVE

func play_move_sound() -> void:
	if not s_move == null:
		sfx[0].stream = s_move
		sfx[0].play()
		sfx[0].seek(0.0)

func stop_move_sound() -> void:
	if sfx[0].playing and sfx[0].stream is AudioStreamOggVorbis:
		sfx[0].stop()

func get_rot_delta() -> Vector3:
	var r: Vector3 = Vector3.ZERO
	if move_rot.length() != 0.0 and move_progress != move_progress_target:
		r = move_rot * speed * GAME.dt_phys
		if move_progress > move_progress_target:
			r *= -1.0
	return r


#**************************************
# DATA
#**************************************
func data_save() -> Dictionary:
	var data: Dictionary = {}
	data["origin"] = position
	data["rotation"] = rotation
	data["scale"] = scale
	data["move_state"] = move_state
	data["move_progress_target"] = move_progress_target
	data["move_progress"] = move_progress
	data["move_paused"] = move_paused
	data["locked"] = locked
	data["wait_ct"] = wait_ct
	data["sfx0_is_playing"] = sfx[0].playing
	if sfx[0].playing:
		data["sfx0_position"] = sfx[0].get_playback_position()
	return data

func data_load(data: Dictionary) -> void:
	position = GameManager.str_to_vec3(data["origin"])
	rotation = GameManager.str_to_vec3(data["rotation"])
	scale = GameManager.str_to_vec3(data["scale"])
	move_state = data["move_state"] as int
	move_progress_target = data["move_progress_target"] as float
	move_progress = data["move_progress"] as float
	move_paused = data["move_paused"] as bool
	locked = data["locked"] as int
	wait_ct = data["wait_ct"] as float
	if (data["sfx0_is_playing"] as bool) == true:
		play_move_sound()
		sfx[0].seek(data["sfx0_position"])
	else:
		stop_move_sound()


#**************************************
# STATE MANAGEMENT
#**************************************
func state_physics(delta: float) -> void:
	# Crush stuff
	if move_state == Func.MoveStates.MOVE and crush_damage > 0 and move_progress != move_progress_target:
		if crush_ct > 0.0:
			crush_ct -= delta
		var mdir: Vector3
		if move_progress_target == 1:
			mdir = move_pos[0].direction_to(move_pos[1])
		else:
			mdir = move_pos[1].direction_to(move_pos[0])
		mdir *= speed * delta
		block_area.position = mdir
		var ents: Array[Node3D] = block_area.get_overlapping_bodies()
		if ents.size() > 0:
			for ent in ents:
				if ent is Actor:
					var t: Transform3D = ent.global_transform
					t.origin += mdir
					var act_c: Array = ent.space_state.intersect_shape(ent.make_shape_query(t, 0.0, GameManager.MAP_LAYER, [self, ent]))
					if act_c.size() > 0:
						if crush_ct <= 0.0:
							ent.call("damage", self, self, crush_damage)
							crush_ct = 0.1
						if ent.health > 0:
							if wait >= 0.0:
								move_progress_target = 0.0 if move_progress_target != 0.0 else 1.0
							else:
								mv_pause(true)
		mv_pause(false)
	
	# Continue movement
	if !move_paused:
		if move_state == Func.MoveStates.MOVE:
			# Back and forth loop
			if loop == GameManager.LoopTypes.LOOP_PINGPONG:
				if move_progress >= 1.0:
					move_progress_target = 0.0
				elif move_progress <= 0.0:
					move_progress_target = 1.0
			
			# Instant restart loop
			elif loop == GameManager.LoopTypes.LOOP_WRAP:
				if move_progress >= 1.0 and move_progress_target != 0.0:
					move_progress_target = 0.0
				elif move_progress <= 0.0 and move_progress_target != 1.0:
					move_progress_target = 1.0
			
			# Movement
			if move_progress != move_progress_target:
				if move_progress < move_progress_target:
					move_progress = minf(move_progress + speed * delta, move_progress_target)
				elif move_progress > move_progress_target:
					move_progress = maxf(move_progress - speed * delta, move_progress_target)
				if move_pos[0] != move_pos[1]:
					position = move_pos[0].lerp(move_pos[1], move_progress)
				if move_rot != Vector3.ZERO:
					rotation = Vector3.ZERO.lerp(move_rot, move_progress)
				if move_scale != Vector3.ONE:
					scale = Vector3.ONE.lerp(move_scale, move_progress)
			else:
				stop_move_sound()
				if wait > 0.0:
					if move_progress >= 1.0 or loop != GameManager.LoopTypes.LOOP_NO:
						move_state = Func.MoveStates.WAIT
						wait_ct = wait
						return
				if loop == GameManager.LoopTypes.LOOP_NO:
					move_state = Func.MoveStates.READY
		# We can hold the door for a bit
		elif move_state == Func.MoveStates.WAIT and !move_paused:
			if wait_ct > 0.0:
				wait_ct -= delta
			else:
				if loop == GameManager.LoopTypes.LOOP_NO:
					move_progress_target = 0.0 if move_progress != 0.0 else 1.0
					play_move_sound()
				move_state = Func.MoveStates.MOVE


#**************************************
# BASE PROCESSING
#**************************************
func _init() -> void:
	add_to_group("WORLD")
	add_to_group("FUNCC_MOVE")
	add_to_group("SAV")
	sync_to_physics = false

func _ready() -> void:
	if !Engine.is_editor_hint():
		if GAME.appearance_check(appearance_flags):
			queue_free()
			return
		
		# Child pointers
		block_area = get_node("block_area")
		for i in 2:
			sfx[i] = get_node(NodePath("sfx" + String.num(i))) as AudioStreamPlayer3D
		s_move = GameManager.update_sound(move_sound)
		s_locked = GameManager.update_sound(locked_sound)
		s_unlock = GameManager.update_sound(unlock_sound)

		# Movement setup
		move_pos[0] = position
		move_pos[1] += move_pos[0]
		print(speed)
		if speed > 0.0:
			speed = 1.0 / speed
		GAME.set_targetname(self, targetname)
		if (flags & Func.FUNC_FLAG_START_OPEN):
			move_progress_target = 1.0
			move_progress = 1.0
			position = move_pos[1]
			rotation = move_rot
			scale = move_scale
		if loop != GameManager.LoopTypes.LOOP_NO:
			move_state = Func.MoveStates.MOVE
			play_move_sound()

func _physics_process(_delta: float) -> void:
	if !Engine.is_editor_hint():
		call("state_physics", GAME.dt_phys)
