#include "GameDefs.h"

/*******************************************************************************
FUNC: BUTTON
Visual trigger really.Intended to be used with func_move objects like doors or
elevators, but technically should be able to trigger anything.
*******************************************************************************/
void FuncButton::_bind_methods() {
	// PROPERTIES
	// lock flags
	ClassDB::bind_method(D_METHOD("set_lock_flags", "_lock_flags"), &FuncButton::set_lock_flags);
	ClassDB::bind_method(D_METHOD("get_lock_flags"), &FuncButton::get_lock_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "lock_flags", PROPERTY_HINT_FLAGS,
		LOCK_FLAG_LIST), "set_lock_flags", "get_lock_flags");
	// move pos
	ClassDB::bind_method(D_METHOD("set_move_pos", "_move_pos"), &FuncButton::set_move_pos);
	ClassDB::bind_method(D_METHOD("get_move_pos"), &FuncButton::get_move_pos);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "move_pos"), "set_move_pos", "get_move_pos");
	// move rot
	ClassDB::bind_method(D_METHOD("set_move_rot", "_move_rot"), &FuncButton::set_move_rot);
	ClassDB::bind_method(D_METHOD("get_move_rot"), &FuncButton::get_move_rot);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "move_rot"), "set_move_rot", "get_move_rot");
	// move scale
	ClassDB::bind_method(D_METHOD("set_move_scale", "_move_scale"), &FuncButton::set_move_scale);
	ClassDB::bind_method(D_METHOD("get_move_scale"), &FuncButton::get_move_scale);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "move_scale"), "set_move_scale", "get_move_scale");
	// hinge offset
	ClassDB::bind_method(D_METHOD("set_hinge_offset", "_hinge_offset"), &FuncButton::set_hinge_offset);
	ClassDB::bind_method(D_METHOD("get_hinge_offset"), &FuncButton::get_hinge_offset);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "hinge_offset"), "set_hinge_offset", "get_hinge_offset");
	// speed
	ClassDB::bind_method(D_METHOD("set_speed", "_speed"), &FuncButton::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &FuncButton::get_speed);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
	// locked
	ClassDB::bind_method(D_METHOD("set_locked", "_locked"), &FuncButton::set_locked);
	ClassDB::bind_method(D_METHOD("get_locked"), &FuncButton::get_locked);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "locked"), "set_locked", "get_locked");
	// locked message
	ClassDB::bind_method(D_METHOD("set_locked_message", "_locked_message"), &FuncButton::set_locked_message);
	ClassDB::bind_method(D_METHOD("get_locked_message"), &FuncButton::get_locked_message);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "locked_message"), "set_locked_message", "get_locked_message");
	// locked sound
	ClassDB::bind_method(D_METHOD("set_locked_sound", "_locked_sound"), &FuncButton::set_locked_sound);
	ClassDB::bind_method(D_METHOD("get_locked_sound"), &FuncButton::get_locked_sound);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "locked_sound"), "set_locked_sound", "get_locked_sound");
	// unlock sound
	ClassDB::bind_method(D_METHOD("set_unlock_sound", "_unlock_sound"), &FuncButton::set_unlock_sound);
	ClassDB::bind_method(D_METHOD("get_unlock_sound"), &FuncButton::get_unlock_sound);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "unlock_sound"), "set_unlock_sound", "get_unlock_sound");
	// press sound
	ClassDB::bind_method(D_METHOD("set_press_sound", "_press_sound"), &FuncButton::set_press_sound);
	ClassDB::bind_method(D_METHOD("get_press_sound"), &FuncButton::get_press_sound);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "press_sound"), "set_press_sound", "get_press_sound");

	// METHODS
	ClassDB::bind_method(D_METHOD("update_properties"), &FuncButton::update_properties);
	ClassDB::bind_method(D_METHOD("reparent_entity"), &FuncButton::reparent_entity);
	ClassDB::bind_method(D_METHOD("update_children"), &FuncButton::update_children);
	ClassDB::bind_method(D_METHOD("update_hinge"), &FuncButton::update_hinge);
	// Trigger
	ClassDB::bind_method(D_METHOD("use", "activator"), &FuncButton::use);
	ClassDB::bind_method(D_METHOD("reset_trigger"), &FuncButton::reset_trigger);
	// Access
	ClassDB::bind_method(D_METHOD("lock"), &FuncButton::lock);
	ClassDB::bind_method(D_METHOD("unlock"), &FuncButton::unlock);
	// Data
	ClassDB::bind_method(D_METHOD("data_save"), &FuncButton::data_save);
	ClassDB::bind_method(D_METHOD("data_load", "data"), &FuncButton::data_load);
	// Base Processing
	ClassDB::bind_method(D_METHOD("state_physics", "delta"), &FuncButton::state_physics);
}

void FuncButton::update_properties() {
	Trigger::update_properties();
	if (!Engine::get_singleton()->is_editor_hint())
		return;
	// Movement
	if (properties.has("hinge_offset"))
		hinge_offset = GameManager::id_vec_to_godot_vec(properties["hinge_offset"]);
	else
		hinge_offset = VECTOR3_ZERO;
	if (properties.has("move_pos"))
		move_pos[1] = GameManager::id_vec_to_godot_vec(properties["move_pos"]) * INVERSE_SCALE;
	if (properties.has("move_rot")) {
		if (properties["move_rot"].get_type() == Variant::VECTOR3) {
			Vector3 r = properties["move_rot"];
			for (int i = 0; i < 3; i++)
				move_rot[i] = Math::deg_to_rad(r[i]);
		}
	}
	if (properties.has("move_scale"))
		move_scale = GameManager::id_vec_to_godot_vec(properties["move_scale"]);
	if (properties.has("speed"))
		speed = (float)properties["speed"] * INVERSE_SCALE;
	// Access
	if (properties.has("lock_flags"))
		lock_flags = (uint8_t)(int)properties["lock_flags"];
	locked = GameManager::set_lock(lock_flags, true);
	if (properties.has("locked_message"))
		locked_message = String(properties["locked_message"]).replace("  ", " ");
	// Audio
	if (properties.has("sound"))
		press_sound = (String)properties["sound"];
	if (properties.has("locked_sound"))
		locked_sound = (String)properties["locked_sound"];
	if (properties.has("unlock_sound"))
		unlock_sound = (String)properties["unlock_sound"];

	call_deferred("update_children");
}

// Movement
void FuncButton::set_move_pos(Vector3 p) { move_pos[1] = p; }
Vector3 FuncButton::get_move_pos() { return move_pos[1]; }
void FuncButton::set_move_rot(Vector3 r) { move_rot = r; }
Vector3 FuncButton::get_move_rot() { return move_rot; }
void FuncButton::set_move_scale(Vector3 s) { move_scale = s; }
Vector3 FuncButton::get_move_scale() { return move_scale; }
void FuncButton::set_hinge_offset(Vector3 h) { hinge_offset = h; }
Vector3 FuncButton::get_hinge_offset() { return hinge_offset; }
void FuncButton::set_speed(float s) { speed = s; }
float FuncButton::get_speed() { return speed; }
// Access
void FuncButton::set_lock_flags(int f) { lock_flags = (uint8_t)f; }
int FuncButton::get_lock_flags() { return (int)lock_flags; }
void FuncButton::set_locked(int l) { locked = l; }
int FuncButton::get_locked() { return locked; }
void FuncButton::set_locked_message(String m) { locked_message = m; }
String FuncButton::get_locked_message() { return locked_message; }
// Audio
void FuncButton::set_press_sound(String s) { press_sound = s; }
String FuncButton::get_press_sound() { return press_sound; }
void FuncButton::set_locked_sound(String s) { locked_sound = s; }
String FuncButton::get_locked_sound() { return locked_sound; }
void FuncButton::set_unlock_sound(String s) { unlock_sound = s; }
String FuncButton::get_unlock_sound() { return unlock_sound; }

void FuncButton::reparent_entity() {
	String moveparent = properties["moveparent"];
	moveparent = "entity_func_move_" + moveparent;
	if (get_parent()->get_name() == moveparent)
		return;
	if (is_inside_tree())
		if (get_parent()->has_node(NodePath(moveparent))) {
			Transform3D t = get_global_transform();
			Node* new_parent = get_parent()->get_node<Node>(NodePath(moveparent));
			get_parent()->remove_child(this);
			new_parent->add_child(this);
			set_global_transform(t);
			set_owner(new_parent->get_owner());
			for (int i = 0; i < get_child_count(); i++)	{
				Node* n = get_child(i);
				n->set_owner(get_owner());
			}
		}
}

void FuncButton::update_children() {
	if (has_node("sfx"))
		return;
	float volume = 1.0f;
	for (int i = 0; i < get_child_count(); i++)	{
		if (get_child(i)->get_class() == "MeshInstance3D") {
			MeshInstance3D* m = cast_to<MeshInstance3D>(get_child(i));
			m->set_ignore_occlusion_culling(true);
			m->set_layer_mask(CULL_DYNAMIC);
			m->set_gi_mode(GeometryInstance3D::GI_MODE_STATIC);
			if (properties.has("shadowcast"))
				m->set_cast_shadows_setting(GeometryInstance3D::ShadowCastingSetting((int)properties["shadowcast"]));
			else
				m->set_cast_shadows_setting(GeometryInstance3D::SHADOW_CASTING_SETTING_ON);
			if (m->get_mesh()->get_class() == "ArrayMesh")
				Ref<ArrayMesh>(m->get_mesh())->lightmap_unwrap(Transform3D(), INVERSE_SCALE);
			volume = fmaxf(m->get_mesh()->get_aabb().get_volume() * 0.5f, 3.0f);
			break;
		}
	}
	if (has_node(NodePath("sfx")) == false) {
		sfx = memnew(AudioStreamPlayer3D);
		sfx->set_bus("SfxWorld");
		sfx->set_attenuation_filter_cutoff_hz(10000);
		sfx->set_unit_size(fminf(volume, 10.6f));
		sfx->set_max_distance(fminf(volume * 3.0f, 32.0f));
		add_child(sfx);
		sfx->set_name("sfx");
		sfx->set_owner(get_owner());
	}
}

void FuncButton::update_hinge() {
	if (properties.has("hinge_target") && properties["hinge_target"] != "") {
		String h = properties["hinge_target"];
		if (is_inside_tree()) {
			Node* n = get_node_or_null(NodePath(h));
			if (n != nullptr && n->get_class() == "InfoNull")
				hinge_offset = cast_to<InfoNull>(n)->get_global_position() - get_global_position();
		}
		properties.erase("hinge");
	}
	if (hinge_offset != VECTOR3_ZERO) {
		for (int i = 0; i < get_child_count(); i++) {
			Node* n = get_child(i);
			if (n->has_method("set_position"))
				n->set("position", cast_to<Node3D>(n)->get_position() + hinge_offset);
		}
		hinge_offset = VECTOR3_ZERO;
		if (properties.has("hinge_offset"))
			properties.erase("hinge_offset");
	}
}

void FuncButton::use(Node* activator) {
	if (trigger_state == ST_READY && activator->is_in_group("PLAYER"))	{
		if (GAME->check_lock(activator, this)) {
			sfx->set_stream(s_locked);
			sfx->play();
			return;
		}
		locked = 0;
		move_progress_target = 1.0f;
		sfx->set_stream(s_pressed);
		sfx->play();
		toggle_collision(false);
		last_activator = activator;
		if (delay > 0) {
			trigger_state = ST_DELAYED;
			timeout = delay;
		}
		else
			call("_use2");
	}
}

void FuncButton::reset_trigger() {
	toggle_collision(true);
	set_monitorable(true);
	health = health_max;
	move_progress_target = 0.0f;
	trigger_state = ST_READY;
}

void FuncButton::lock() {
	locked = 1;
	sfx->set_stream(s_locked);
	sfx->play();
}

void FuncButton::unlock() {
	locked = 0;
	sfx->set_stream(s_unlock);
	sfx->play();
}

Dictionary FuncButton::data_save() {
	Dictionary data = Trigger::data_save();
	data["move_progress_target"] = move_progress_target;
	data["move_progress"] = move_progress;
	data["locked"] = locked;
	data["origin"] = get_position();
	data["rotation"] = get_rotation();
	data["scale"] = get_scale();
	return data;
}

void FuncButton::data_load(Dictionary data) {
	Trigger::data_load(data);
	move_progress_target = data["move_progress_target"];
	move_progress = data["move_progress"];
	locked = data["locked"];
	set_position(GameManager::str_to_vec3(data["origin"]));
	set_rotation(GameManager::str_to_vec3(data["rotation"]));
	set_scale(GameManager::str_to_vec3(data["scale"]));
}

void FuncButton::state_physics(float delta) {
	if (!Engine::get_singleton()->is_editor_hint()) {
		if (!button_mat.is_null())
			button_mat->set_shader_parameter("button_pressed", (bool)move_progress_target);
		// Movement
		if (move_progress != move_progress_target) {
			if (move_progress < move_progress_target)
				move_progress = fminf(move_progress + speed * delta, move_progress_target);
			else if (move_progress > move_progress_target)
				move_progress = fmaxf(move_progress - speed * delta, move_progress_target);
			if (move_pos[0] != move_pos[1])
				set_position(move_pos[0].lerp(move_pos[1], move_progress));
			if (move_rot != VECTOR3_ZERO)
				set_rotation(VECTOR3_ZERO.lerp(move_rot, move_progress));
			if (move_scale != VECTOR3_ONE)
				set_scale(VECTOR3_ONE.lerp(move_scale, move_progress));
		}
	}
}

FuncButton::FuncButton() {
	add_to_group("TRIGGER_USE");
	set_monitorable(true);
}

FuncButton::~FuncButton() {}

void FuncButton::_ready() {
	Trigger::_ready();
	if (is_queued_for_deletion())
		return;
	if (!Engine::get_singleton()->is_editor_hint())	{
		if (flags & TRIGGER_FLAG_NO_TOUCH) {
			if (is_connected("body_entered", Callable(this, "_on_ent_entered")))
				disconnect("body_entered", Callable(this, "_on_ent_entered"));
			set_monitoring(false);
		}
		sfx = get_node<AudioStreamPlayer3D>("sfx");
		s_pressed = GameManager::update_sound(press_sound);
		s_locked = GameManager::update_sound(locked_sound);
		s_unlock = GameManager::update_sound(unlock_sound);
		move_pos[0] = get_position();
		move_pos[1] += move_pos[0];
		if (speed > 0.0f)
			speed = 1.0f / speed;
		if (flags & TRIGGER_FLAG_ON_START) {
			move_progress_target = 1.0f;
			move_progress = 1.0f;
			set_position(move_pos[1]);
			set_rotation(move_rot);
			set_scale(move_scale);
		}
		// Get the button material, to display on:off states via shader
		for (int i = 0; i < get_child_count(); i++)
			if (get_child(i)->get_class() == "MeshInstance3D") {
				Ref<Mesh> m = cast_to<MeshInstance3D>(get_child(i))->get_mesh();
				for (int s = 0; s < m->get_surface_count(); s++)
					if (m->surface_get_material(s)->get_class() == "ShaderMaterial") {
						button_mat = m->surface_get_material(s)->duplicate();
						m->surface_set_material(s, button_mat);
						return;
					}
			}
	}
}