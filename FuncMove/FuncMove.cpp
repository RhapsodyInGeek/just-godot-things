/*******************************************************************************
FUNC: MOVE
Moving brush. Primarily used for doors, but can also be used for platforms and
even decorations.
*******************************************************************************/
#include "FuncMove.h"

void FuncMove::_register_methods()
{
	// Properties
	register_property("properties", &FuncMove::set_properties, &FuncMove::get_properties, {});
	register_property("spawnflags", &FuncMove::set_spawnflags, &FuncMove::get_spawnflags, 0);
	register_property("grav_type", &FuncMove::set_grav_type, &FuncMove::get_grav_type, 0);
	register_property("move_pos", &FuncMove::set_move_pos, &FuncMove::get_move_pos, Vector3::ZERO);
	register_property("move_rot", &FuncMove::set_move_rot, &FuncMove::get_move_rot, Vector3::ZERO);
	register_property("move_scale", &FuncMove::set_move_scale, &FuncMove::get_move_scale, Vector3::ONE);
	register_property("speed", &FuncMove::set_speed, &FuncMove::get_speed, 1.0f);
	register_property("wait", &FuncMove::set_wait, &FuncMove::get_wait, -1.0f);
	register_property("loop", &FuncMove::set_loop, &FuncMove::get_loop, 0);
	register_property("locked", &FuncMove::set_locked, &FuncMove::get_locked, 0);
	register_property("crush_damage", &FuncMove::set_crush_damage, &FuncMove::get_crush_damage, 2);
	register_property("locked_message", &FuncMove::set_locked_message, &FuncMove::get_locked_message, String());
	register_property("move_sound", &FuncMove::set_move_sound, &FuncMove::get_move_sound, String());
	register_property("locked_sound", &FuncMove::set_locked_sound, &FuncMove::get_locked_sound, String());
	register_property("unlock_sound", &FuncMove::set_unlock_sound, &FuncMove::get_unlock_sound, String());
	register_method("reparent", &FuncMove::reparent);
	register_method("update_children", &FuncMove::update_children);
	register_method("trigger", &FuncMove::trigger);
	register_method("toggle", &FuncMove::toggle);
	register_method("lock", &FuncMove::lock);
	register_method("unlock", &FuncMove::unlock);
	register_method("play_motion", &FuncMove::play_motion);
	register_method("reverse_motion", &FuncMove::reverse_motion);
	register_method("pause_motion", &FuncMove::pause_motion);
	register_method("stop", &FuncMove::stop);
	register_method("stop_at_end", &FuncMove::stop_at_end);
	register_method("stop_at_pos", &FuncMove::stop_at_pos);
	register_method("loop_none", &FuncMove::loop_none);
	register_method("loop_pong", &FuncMove::loop_pong);
	register_method("loop_restart", &FuncMove::loop_restart);
	register_method("remove", &FuncMove::remove);
	register_method("get_rot_delta", &FuncMove::get_rot_delta);
	register_method("state_physics", &FuncMove::state_physics);
	register_method("data_save", &FuncMove::data_save);
	register_method("data_load", &FuncMove::data_load);
	register_method("_ready", &FuncMove::_ready);
	register_method("_physics_process", &FuncMove::_physics_process);
	register_signal<FuncMove>("triggered");
}

// PROPERTY SETGETS
void FuncMove::set_properties(Dictionary p)
{
	properties = p;
	if (!Engine::get_singleton()->is_editor_hint())
		return;
	// Parenting
	if (properties.has("movename"))
	{
		String movename = properties["movename"];
		if (movename != "")
			set_name("entity_func_move_" + movename);
	};
	if (properties.has("moveparent"))
		call_deferred("reparent");
	// Movement
	if (properties.has("translation"))
	{
		move_pos[1] = properties["translation"];
		move_pos[1] *= 0.03125f;
	};
	if (properties.has("rotation"))
	{
		Vector3 r = properties["rotation"];
		move_rot = Vector3(Math::deg2rad(r.x), Math::deg2rad(r.y), Math::deg2rad(r.z));
	};
	if (properties.has("scale"))
		move_scale = properties["scale"];
	if (properties.has("speed"))
		speed = properties["speed"];
	if (properties.has("wait"))
		wait = properties["wait"];
	// Flags
	if (properties.has("spawnflags"))
		spawnflags = properties["spawnflags"];
	if (properties.has("grav_type"))
		grav_type = properties["grav_type"];
	locked = GameManager::lock_set(spawnflags);
	if (properties.has("locked_message"))
		locked_message = properties["locked_message"];
	if (properties.has("loop"))
		loop = properties["loop"];
	if (properties.has("damage"))
		crush_damage = properties["damage"];
	// Audio
	if (properties.has("sound"))
		move_sound = properties["sound"];
	if (properties.has("locked_sound"))
		locked_sound = properties["locked_sound"];
	if (properties.has("unlock_sound"))
		unlock_sound = properties["unlock_sound"];
	// Children refresh
	call_deferred("update_children");
}

Dictionary FuncMove::get_properties() { return properties; }

void FuncMove::set_spawnflags(int f) { spawnflags = f; }
int FuncMove::get_spawnflags() { return spawnflags; }
void FuncMove::set_grav_type(int g) { grav_type = g; }
int FuncMove::get_grav_type() { return grav_type; }
void FuncMove::set_move_pos(Vector3 p) { move_pos[1] = p; }
Vector3 FuncMove::get_move_pos() { return move_pos[1]; }
void FuncMove::set_move_rot(Vector3 r) { move_rot = r; }
Vector3 FuncMove::get_move_rot() { return move_rot; }
void FuncMove::set_move_scale(Vector3 s) { move_scale = s; }
Vector3 FuncMove::get_move_scale() { return move_scale; }
void FuncMove::set_speed(float s) { speed = s; }
float FuncMove::get_speed() { return speed; }
void FuncMove::set_wait(float w) { wait = w; }
float FuncMove::get_wait() { return wait; }
void FuncMove::set_loop(int l) { loop = l; }
int FuncMove::get_loop() { return loop; }
void FuncMove::set_locked(int l) { locked = l; }
int FuncMove::get_locked() { return locked; }
void FuncMove::set_crush_damage(int d) { crush_damage = d; }
int FuncMove::get_crush_damage() { return crush_damage; }
void FuncMove::set_locked_message(String m) { locked_message = m; }
String FuncMove::get_locked_message() { return locked_message; }
void FuncMove::set_move_sound(String s) { move_sound = s; }
String FuncMove::get_move_sound() { return move_sound; }
void FuncMove::set_locked_sound(String s) { locked_sound = s; }
String FuncMove::get_locked_sound() { return locked_sound; }
void FuncMove::set_unlock_sound(String s) { unlock_sound = s; }
String FuncMove::get_unlock_sound() { return unlock_sound; }

void FuncMove::reparent()
{
	String moveparent = properties["moveparent"];
	moveparent = "entity_func_move_" + moveparent;
	if (get_parent()->get_name() == moveparent)
		return;
	if (is_inside_tree())
		if (get_parent()->has_node(NodePath(moveparent)))
		{
			Transform t = get_global_transform();
			Node* new_parent = get_parent()->get_node(NodePath(moveparent));
			get_parent()->remove_child(this);
			new_parent->add_child(this);
			set_global_transform(t);
			set_owner(new_parent->get_owner());
			for (int i = 0; i < get_child_count(); i++)
			{
				Node* n = get_child(i);
				n->set_owner(get_owner());
			};
		};
}

void FuncMove::update_children()
{
	MeshInstance* m;
	float volume = 1.0f;
	// Block area, for crush detection
	block_area = nullptr;
	if (has_node("block_area") == false)
	{
		block_area = Area::_new();
		add_child(block_area);
		block_area->set_owner(get_owner());
		block_area->set_name("block_area");
		block_area->set_collision_layer(0);
		block_area->set_monitorable(false);
		int layers = GameManager::ACTOR_LAYER + GameManager::DEAD_LAYER;
		block_area->set_collision_mask(layers);
		block_area->set_monitoring(true);
	};
	for (int i = 0; i < get_child_count(); i++)
	{
		if (block_area != nullptr && get_child(i)->get_class() == "CollisionShape")
		{
			cast_to<CollisionShape>(get_child(i))->get_shape()->set_margin(0.01f);
			CollisionShape* c = cast_to<CollisionShape>(get_child(i)->duplicate());
			block_area->add_child(c);
			c->set_owner(block_area->get_owner());
			c->set_shape(c->get_shape()->duplicate());
		}
		else if (get_child(i)->get_class() == "MeshInstance")
		{
			m = cast_to<MeshInstance>(get_child(i));
			m->set_portal_mode(CullInstance::PORTAL_MODE_GLOBAL);
			m->set_flag(GeometryInstance::FLAG_USE_BAKED_LIGHT, true);
			if (properties.has("shadowcast"))
				m->set_cast_shadows_setting(properties["shadowcast"]);
			else
				m->set_cast_shadows_setting(GeometryInstance::SHADOW_CASTING_SETTING_ON);
			if (m->get_mesh()->get_class() == "ArrayMesh")
				Ref<ArrayMesh>(m->get_mesh())->lightmap_unwrap(Transform::IDENTITY, 0.3125f);
			volume = fmaxf(m->get_mesh()->get_aabb().get_area() * 0.5f, 3.0f);
		};
	};
	for (int i = 0; i < 2; i++)
	{
		if (has_node(NodePath("sfx" + String::num(i))) == false)
		{
			sfx[i] = AudioStreamPlayer3D::_new();
			sfx[i]->set_bus("SfxWorld");
			sfx[i]->set_attenuation_filter_cutoff_hz(10000);
			sfx[i]->set_out_of_range_mode(AudioStreamPlayer3D::OUT_OF_RANGE_MIX);
			sfx[i]->set_unit_size(fminf(volume, 10.6f));
			sfx[i]->set_max_distance(fminf(volume * 3.0f, 32.0f));
			add_child(sfx[i]);
			sfx[i]->set_name("sfx" + String::num(i));
			sfx[i]->set_owner(get_owner());
		};
	};
}

Ref<AudioStream> FuncMove::update_sound(String path)
{
	String snd_path = "sounds/" + path;
	Ref<AudioStream> s;
	if (snd_path.rfind(".wav") > -1 || snd_path.rfind(".ogg") > -1)
	{
		Ref<File> file = Ref<File>(File::_new());
		if (file->file_exists("user://" + snd_path)) // For modding
			s = ResourceLoader::get_singleton()->load("user://" + snd_path);
		else if (file->file_exists("res://" + snd_path) || file->file_exists("res://" + snd_path + ".import"))
			s = ResourceLoader::get_singleton()->load("res://" + snd_path);
	};
	return s;
}

// MOVE METHODS
void FuncMove::trigger(Node* caller)
{
	if (locked > 0)
	{
		Node* n = caller;
		if (n->is_in_group("TRIGGER_USE"))
			n = n->call("get_last_entity");
		if (n != nullptr)
		{
			if (GAME->lock_check(n, this))
			{
				SND->play3d(sfx[1], s_locked);
				return;
			};
			SND->play3d(sfx[1], s_unlock);
			locked = 0;
		};
	};
	if (locked == 0 && move_progress_target < 1.0f)
		play_motion();
}

void FuncMove::toggle()
{
	if (move_progress_target > 0.0f)
		reverse_motion();
	else
		play_motion();
}

void FuncMove::lock()
{
	locked = 1;
	SND->play3d(sfx[1], s_locked);
}

void FuncMove::unlock()
{
	locked = 0;
	SND->play3d(sfx[1], s_unlock);
}

void FuncMove::play_motion()
{
	if (move_progress_target < 1.0f)
	{
		door_state = ST_MOVE;
		move_progress_target = 1.0f;
		play_move_sound();
	};
}

void FuncMove::reverse_motion(Node* ent)
{
	if (ent != nullptr && (spawnflags & GameManager::FL_PLAYER_ONLY) && !ent->is_in_group("PLAYER"))
		return;
	if (move_progress_target > 0.0f)
	{
		door_state = ST_MOVE;
		move_progress_target = 0.0f;
		play_move_sound();
	};
}

void FuncMove::pause_motion(bool set_pause) { move_paused = set_pause; }

void FuncMove::stop(float mt)
{
	loop = 0;
	move_progress_target = mt;
}

void FuncMove::stop_at_end() { stop(1.0f); }
void FuncMove::stop_at_pos() { stop(move_progress_target); }
void FuncMove::loop_none() { loop = 0; }
void FuncMove::loop_pong() { loop = 1; play_move_sound(); door_state = ST_MOVE; }
void FuncMove::loop_restart() {	loop = 2; move_progress_target = 1.0f; play_move_sound(); door_state = ST_MOVE; }

void FuncMove::play_move_sound()
{
	if (!s_move.is_null())
	{
		SND->play3d(sfx[0], s_move);
		sfx[0]->seek(0.0f);
	}
}

void FuncMove::stop_move_sound()
{
	if (sfx[0]->is_playing() && sfx[0]->get_stream()->get_class() == "AudioStreamOGGVorbis")
		sfx[0]->stop();
}

void FuncMove::remove() { queue_free(); }

Vector3 FuncMove::get_rot_delta()
{
	Vector3 r = Vector3::ZERO;
	if (move_rot.length() != 0.0f && move_progress != move_progress_target)
	{
		r = move_rot * speed * get_physics_process_delta_time();
		if (move_progress > move_progress_target)
			r *= -1.0f;
	};
	return r;
}

void FuncMove::state_physics(float delta)
{
	// Crush stuff
	if (door_state == ST_MOVE && crush_damage > 0.0f && move_progress != move_progress_target)
	{
		if (crush_ct > 0.0f)
			crush_ct -= delta;
		Vector3 mdir;
		if (move_progress_target == 1)
			mdir = move_pos[0].direction_to(move_pos[1]);
		else
			mdir = move_pos[1].direction_to(move_pos[0]);
		mdir *= speed * delta;
		block_area->set_translation(mdir);
		Array ents = block_area->get_overlapping_bodies();
		if (ents.size() > 0)
		{
			for (int i = 0; i < ents.size(); i++)
				if (cast_to<Node>(ents[i])->is_in_group("ACTOR"))
				{
					Actor* e = cast_to<Actor>(ents[i]);
					Transform t = e->get_global_transform();
					t.origin += mdir;
					Array act_c = e->col_shape_check_body(t, 0.0f, GameManager::MAP_LAYER, Array::make(this, e));
					if (act_c.size() > 0 && crush_ct <= 0.0f)
					{
						e->call("damage", crush_damage, this, get_path());
						if ((int)e->call("get_health") > 0)
						{
							if (wait >= 0.0f)
								move_progress_target = (move_progress_target != 0.0f) ? 0.0f : 1.0f;
							else
								pause_motion(true);
						};
						crush_ct = 0.1f;
					};
				};
		}
		else
			pause_motion(false);
	};
	// Continue movement
	if (!move_paused)
	{
		if (door_state == ST_MOVE)
		{
			// Back and forth loop
			if (loop == 1)
			{
				if (move_progress >= 1.0f)
					move_progress_target = 0.0f;
				else if (move_progress <= 0.0f)
					move_progress_target = 1.0f;
			}
			// Instant restart loop
			else if (loop == 2 && move_progress >= 1.0f)
				move_progress = 0.0f;
			// Movement
			if (move_progress != move_progress_target)
			{
				if (move_progress < move_progress_target)
					move_progress = fminf(move_progress + speed * delta, move_progress_target);
				else if (move_progress > move_progress_target)
					move_progress = fmaxf(move_progress - speed * delta, move_progress_target);
				if (move_pos[0] != move_pos[1])
					set_translation(move_pos[0].linear_interpolate(move_pos[1], move_progress));
				if (move_rot != Vector3::ZERO)
					set_rotation(Vector3::ZERO.linear_interpolate(move_rot, move_progress));
				if (move_scale != Vector3::ONE)
					set_scale(Vector3::ONE.linear_interpolate(move_scale, move_progress));
			}
			else
			{
				stop_move_sound();
				if (wait > 0.0f)
					if (move_progress >= 1.0f || loop > 0)
					{
						door_state = ST_WAIT;
						wait_ct = wait;
						return;
					};
				if (loop == 0)
					door_state = ST_READY;
			};
		}
		// We can hold the door for a bit
		else if (door_state == ST_WAIT && !move_paused)
		{
			if (wait_ct > 0.0f)
				wait_ct -= delta;
			else
			{
				if (loop == 0)
				{
					move_progress_target = (move_progress_target != 0.0f) ? 0.0f : 1.0f;
					play_move_sound();
				};
				door_state = ST_MOVE;
			};
		};
	};
}

// SAVE DATA
Dictionary FuncMove::data_save()
{
	Dictionary data;
	data["origin"] = get_translation();
	data["rotation"] = get_rotation();
	data["scale"] = get_scale();
	data["door_state"] = door_state;
	data["move_progress_target"] = move_progress_target;
	data["move_progress"] = move_progress;
	data["move_paused"] = move_paused;
	data["locked"] = locked;
	data["wait_ct"] = wait_ct;
	data["sfx0_is_playing"] = sfx[0]->is_playing();
	if (sfx[0]->is_playing())
		data["sfx0_position"] = sfx[0]->get_playback_position();
	return data;
}

void FuncMove::data_load(Dictionary data)
{
	set_translation(GameManager::str_to_vec3(data["origin"]));
	set_rotation(GameManager::str_to_vec3(data["rotation"]));
	set_scale(GameManager::str_to_vec3(data["scale"]));
	door_state = data["door_state"];
	move_progress_target = data["move_progress_target"];
	move_progress = data["move_progress"];
	move_paused = data["move_paused"];
	locked = data["locked"];
	wait_ct = data["wait_ct"];
	if ((bool)data["sfx0_is_playing"] == true)
	{
		play_move_sound();
		sfx[0]->seek(data["sfx0_position"]);
	}
	else
		stop_move_sound();
}

// BASE PROCESSING
void FuncMove::_init()
{
	add_to_group("WORLD");
	add_to_group("FUNC_MOVE");
	add_to_group("SAV");
	set_collision_layer(GameManager::MAP_LAYER);
	set_collision_mask(0);
}

void FuncMove::_ready()
{
	if (!Engine::get_singleton()->is_editor_hint())
	{
		GAME = cast_to<GameManager>(get_node("/root/GameManager"));
		GAME->remove_check(this, spawnflags);
		SND = cast_to<SoundManager>(get_node("/root/SoundManager"));
		// Child pointers
		block_area = cast_to<Area>(get_node("block_area"));
		for (int i = 0; i < 2; i++)
			sfx[i] = cast_to<AudioStreamPlayer3D>(get_node(NodePath("sfx" + String::num(i))));
		s_move = update_sound(move_sound);
		s_locked = update_sound(locked_sound);
		s_unlock = update_sound(unlock_sound);
		// Movement setup
		move_pos[0] = get_translation();
		move_pos[1] += move_pos[0];
		if (speed > 0.0f)
			speed = 1.0f / speed;
		if (properties.has("targetname"))
			GAME->set_node_targetname(this, properties["targetname"]);
		if (spawnflags & GameManager::FL_START_OPEN)
		{
			move_progress_target = 1.0f;
			move_progress = 1.0f;
			set_translation(move_pos[1]);
			set_rotation(move_rot);
			set_scale(move_scale);
		};
		if (loop > 0)
		{
			door_state = ST_MOVE;
			play_move_sound();
		};
	};
}

void FuncMove::_physics_process(float delta)
{
	if (!Engine::get_singleton()->is_editor_hint())
	{
		call("state_physics", delta);
	};
}

/*******************************************************************************
FUNC_DOOR
Automatically opens when approached.
Don't use with doors that need to open together.
*******************************************************************************/
void FuncDoor::_register_methods()
{
	register_method("update_children", &FuncDoor::update_children);
	register_method("trigger", &FuncDoor::trigger);
	register_method("reverse_motion", &FuncDoor::reverse_motion);
	register_method("state_physics", &FuncDoor::state_physics);
	register_method("_ready", &FuncDoor::_ready);
}

void FuncDoor::update_children()
{
	FuncMove::update_children();
	// Toggle doors don't get trigger_areas
	if (spawnflags & GameManager::FL_TOGGLE)
	{
		if (has_node("trigger_area"))
			get_node("trigger_area")->queue_free();
		return;
	};
	MeshInstance* m;
	for (int i = 0; i < get_child_count(); i++)
		if (get_child(i)->get_class() == "MeshInstance")
		{
			m = cast_to<MeshInstance>(get_child(i));
			break;
		};
	// Auto generate a trigger volume to open the door
	trigger_area = nullptr;
	if (m != nullptr)
	{
		trigger_area = Area::_new();
		add_child(trigger_area);
		trigger_area->set_owner(get_owner());
		trigger_area->set("name", "trigger_area");
		trigger_area->set_collision_layer(0);
		trigger_area->set_collision_mask(GameManager::TRIGGER_LAYER);
		trigger_area->set_monitorable(false);
		trigger_area->set_monitoring(true);
		CollisionShape* c = CollisionShape::_new();
		Ref<BoxShape> trigger_shape = Ref<BoxShape>(BoxShape::_new());
		AABB aabb = m->get_aabb();
		trigger_shape->set_extents(m->get_aabb().get_size() * 0.5f + Vector3::ONE * 1.875f);
		c->set_shape(trigger_shape);
		trigger_area->add_child(c);
		c->set_owner(get_owner());
	};
}

void FuncDoor::trigger(Node* caller)
{
	if (spawnflags & GameManager::FL_PLAYER_ONLY && !caller->is_in_group("PLAYER"))
		return;
	FuncMove::trigger(caller);
}

void FuncDoor::reverse_motion(Node* ent)
{
	Array ents = trigger_area->get_overlapping_bodies();
	for (int i = 0; i < ents.size(); i++)
		if (cast_to<Node>(ents[i])->is_in_group("ACTOR"))
			return;
	FuncMove::reverse_motion(ent);
}

void FuncDoor::state_physics(float delta)
{
	FuncMove::state_physics(delta);
	if (door_state == ST_MOVE)
		trigger_area->set_global_transform(trg_area_t);
}

void FuncDoor::_init()
{
	FuncMove::_init();
	add_to_group("FUNC_DOOR");
}

void FuncDoor::_ready()
{
	if (!Engine::get_singleton()->is_editor_hint())
	{
		trigger_area = cast_to<Area>(get_node("trigger_area"));
		trg_area_t = get_global_transform();
		trigger_area->connect("body_entered", this, "trigger");
		trigger_area->connect("body_exited", this, "reverse_motion");
		wait = 0.0f;
		if (spawnflags & GameManager::FL_START_OPEN)
			trigger_area->set_global_transform(trg_area_t);
	};
}
