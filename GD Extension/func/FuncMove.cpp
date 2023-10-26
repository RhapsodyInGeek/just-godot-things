#include "GameDefs.h"

/***************************************************************************************************

FUNC MOVE

Moving brush. Primarily used for doors, but can also be used for platforms and even decorations.

***************************************************************************************************/
void FuncMove::_bind_methods() {
    // PROPERTIES
    // properties
    ClassDB::bind_method(D_METHOD("set_properties", "_properties"), &FuncMove::set_properties);
    ClassDB::bind_method(D_METHOD("get_properties"), &FuncMove::get_properties);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");
    // appearance_flags
    ClassDB::bind_method(D_METHOD("set_appearance_flags", "_appearance_flags"), &FuncMove::set_appearance_flags);
    ClassDB::bind_method(D_METHOD("get_appearance_flags"), &FuncMove::get_appearance_flags);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "appearance_flags", PROPERTY_HINT_FLAGS,
        APPEARANCE_FLAG_LIST), "set_appearance_flags", "get_appearance_flags");
    // flags
    ClassDB::bind_method(D_METHOD("set_flags", "_flags"), &FuncMove::set_flags);
    ClassDB::bind_method(D_METHOD("get_flags"), &FuncMove::get_flags);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "flags", PROPERTY_HINT_FLAGS,
        FUNC_FLAG_LIST), "set_flags", "get_flags");
    // targetname
    ClassDB::bind_method(D_METHOD("set_targetname", "targetname"), &FuncMove::set_targetname);
    ClassDB::bind_method(D_METHOD("get_targetname"), &FuncMove::get_targetname);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "targetname"), "set_targetname", "get_targetname");
    // lock flags
    ClassDB::bind_method(D_METHOD("set_lock_flags", "_lock_flags"), &FuncMove::set_lock_flags);
    ClassDB::bind_method(D_METHOD("get_lock_flags"), &FuncMove::get_lock_flags);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "lock_flags", PROPERTY_HINT_FLAGS,
        LOCK_FLAG_LIST), "set_lock_flags", "get_lock_flags");
    // grav type
    ClassDB::bind_method(D_METHOD("set_grav_type", "_grav_type"), &FuncMove::set_grav_type);
    ClassDB::bind_method(D_METHOD("get_grav_type"), &FuncMove::get_grav_type);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "grav_type", PROPERTY_HINT_ENUM, GRAV_TYPE_LIST), "set_grav_type", "get_grav_type");
    // hinge offset
    ClassDB::bind_method(D_METHOD("set_hinge_offset", "_hinge_offset"), &FuncMove::set_hinge_offset);
    ClassDB::bind_method(D_METHOD("get_hinge_offset"), &FuncMove::get_hinge_offset);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "hinge_offset"), "set_hinge_offset", "get_hinge_offset");
    // move pos
    ClassDB::bind_method(D_METHOD("set_move_pos", "_move_pos"), &FuncMove::set_move_pos);
    ClassDB::bind_method(D_METHOD("get_move_pos"), &FuncMove::get_move_pos);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "move_pos"), "set_move_pos", "get_move_pos");
    // move rot
    ClassDB::bind_method(D_METHOD("set_move_rot", "_move_rot"), &FuncMove::set_move_rot);
    ClassDB::bind_method(D_METHOD("get_move_rot"), &FuncMove::get_move_rot);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "move_rot"), "set_move_rot", "get_move_rot");
    // move scale
    ClassDB::bind_method(D_METHOD("set_move_scale", "_move_scale"), &FuncMove::set_move_scale);
    ClassDB::bind_method(D_METHOD("get_move_scale"), &FuncMove::get_move_scale);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "move_scale"), "set_move_scale", "get_move_scale");
    // speed
    ClassDB::bind_method(D_METHOD("set_speed", "_speed"), &FuncMove::set_speed);
    ClassDB::bind_method(D_METHOD("get_speed"), &FuncMove::get_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
    // wait
    ClassDB::bind_method(D_METHOD("set_wait", "_wait"), &FuncMove::set_wait);
    ClassDB::bind_method(D_METHOD("get_wait"), &FuncMove::get_wait);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "wait"), "set_wait", "get_wait");
    // loop
    ClassDB::bind_method(D_METHOD("set_loop", "_loop"), &FuncMove::set_loop);
    ClassDB::bind_method(D_METHOD("get_loop"), &FuncMove::get_loop);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "loop"), "set_loop", "get_loop");
    // crush damage
    ClassDB::bind_method(D_METHOD("set_crush_damage", "_crush_damage"), &FuncMove::set_crush_damage);
    ClassDB::bind_method(D_METHOD("get_crush_damage"), &FuncMove::get_crush_damage);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "crush_damage"), "set_crush_damage", "get_crush_damage");
    // locked
    ClassDB::bind_method(D_METHOD("set_locked", "_locked"), &FuncMove::set_locked);
    ClassDB::bind_method(D_METHOD("get_locked"), &FuncMove::get_locked);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "locked"), "set_locked", "get_locked");
    // locked message
    ClassDB::bind_method(D_METHOD("set_locked_message", "_locked_message"), &FuncMove::set_locked_message);
    ClassDB::bind_method(D_METHOD("get_locked_message"), &FuncMove::get_locked_message);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "locked_message"), "set_locked_message", "get_locked_message");
    // locked sound
    ClassDB::bind_method(D_METHOD("set_locked_sound", "_locked_sound"), &FuncMove::set_locked_sound);
    ClassDB::bind_method(D_METHOD("get_locked_sound"), &FuncMove::get_locked_sound);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "locked_sound"), "set_locked_sound", "get_locked_sound");
    // unlock sound
    ClassDB::bind_method(D_METHOD("set_unlock_sound", "_unlock_sound"), &FuncMove::set_unlock_sound);
    ClassDB::bind_method(D_METHOD("get_unlock_sound"), &FuncMove::get_unlock_sound);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "unlock_sound"), "set_unlock_sound", "get_unlock_sound");
    // move sound
    ClassDB::bind_method(D_METHOD("set_move_sound", "_move_sound"), &FuncMove::set_move_sound);
    ClassDB::bind_method(D_METHOD("get_move_sound"), &FuncMove::get_move_sound);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "move_sound"), "set_move_sound", "get_move_sound");

    // METHODS
    // Scene Tree Modification
    ClassDB::bind_method(D_METHOD("reparent_entity"), &FuncMove::reparent_entity);
    ClassDB::bind_method(D_METHOD("update_children"), &FuncMove::update_children);
    ClassDB::bind_method(D_METHOD("update_hinge"), &FuncMove::update_hinge);
    // Activation
    ClassDB::bind_method(D_METHOD("use", "activator"), &FuncMove::use);
    ClassDB::bind_method(D_METHOD("toggle"), &FuncMove::toggle);
    // Access
    ClassDB::bind_method(D_METHOD("lock"), &FuncMove::lock);
    ClassDB::bind_method(D_METHOD("unlock"), &FuncMove::unlock);
    // Movement
    ClassDB::bind_method(D_METHOD("mv_forward"), &FuncMove::mv_forward);
    ClassDB::bind_method(D_METHOD("mv_reverse", "ent"), &FuncMove::mv_reverse);
    ClassDB::bind_method(D_METHOD("mv_pause"), &FuncMove::mv_pause);
    ClassDB::bind_method(D_METHOD("stop"), &FuncMove::stop);
    ClassDB::bind_method(D_METHOD("stop_at_end"), &FuncMove::stop_at_end);
    ClassDB::bind_method(D_METHOD("stop_at_pos"), &FuncMove::stop_at_pos);
    ClassDB::bind_method(D_METHOD("loop_no"), &FuncMove::loop_no);
    ClassDB::bind_method(D_METHOD("loop_pong"), &FuncMove::loop_pong);
    ClassDB::bind_method(D_METHOD("loop_wrap"), &FuncMove::loop_wrap);
    ClassDB::bind_method(D_METHOD("loop_wrap_reverse"), &FuncMove::loop_wrap_reverse);
    ClassDB::bind_method(D_METHOD("get_rot_delta"), &FuncMove::get_rot_delta);
    // Data
    ClassDB::bind_method(D_METHOD("data_save"), &FuncMove::data_save);
    ClassDB::bind_method(D_METHOD("data_load", "data"), &FuncMove::data_load);
    // State Management
    ClassDB::bind_method(D_METHOD("state_physics", "delta"), &FuncMove::state_physics);
}

/*****************************************************
PROPERTIES
*****************************************************/
void FuncMove::set_properties(Dictionary p) {
    properties = p;
    if (!Engine::get_singleton()->is_editor_hint())
        return;

    // Parenting
    if (properties.has("movename")) {
        String movename = properties["movename"];
        if (movename != "")
            set_name("entity_func_move_" + movename);
    }
    if (properties.has("moveparent"))
        call_deferred("reparent_entity");
    // Targeting
    if (properties.has("targetname"))
        targetname = (String)properties["targetname"];
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
    if (properties.has("wait"))
        wait = (float)properties["wait"];
    // Flags
    if (properties.has("appearance_flags"))
        appearance_flags = (uint8_t)(int)properties["appearance_flags"];
    if (properties.has("flags"))
        flags = (uint32_t)(int)properties["flags"];
    if (properties.has("lock_flags"))
        lock_flags = (uint8_t)(int)properties["lock_flags"];
    if (properties.has("grav_type"))
        grav_type = uint8_t((int)properties["grav_type"]);
    // Access
    locked = GameManager::set_lock(lock_flags);
    if (properties.has("locked_message"))
        locked_message = String(properties["locked_message"]).replace("  ", " ");
    if (properties.has("loop"))
        loop = uint8_t((int)properties["loop"]);
    if (properties.has("damage"))
        crush_damage = int16_t((int)properties["damage"]);
    // Audio
    if (properties.has("sound"))
        move_sound = (String)properties["sound"];
    if (properties.has("locked_sound"))
        locked_sound = (String)properties["locked_sound"];
    if (properties.has("unlock_sound"))
        unlock_sound = (String)properties["unlock_sound"];
    // Children refresh
    call_deferred("update_children");
}

Dictionary FuncMove::get_properties() { return properties; }
void FuncMove::set_appearance_flags(int f) { appearance_flags = (uint8_t)f; } int FuncMove::get_appearance_flags() { return (int)appearance_flags; }
void FuncMove::set_flags(int f) { flags = (uint32_t)f; } int FuncMove::get_flags() { return (int)flags; }
void FuncMove::set_targetname(String t) { targetname = t; } String FuncMove::get_targetname() { return targetname; }
void FuncMove::set_lock_flags(int f) { lock_flags = (uint8_t)f; } int FuncMove::get_lock_flags() { return (int)lock_flags; }
void FuncMove::set_grav_type(int g) { grav_type = (uint8_t)g; } int FuncMove::get_grav_type() { return (int)grav_type; }
// Movement
void FuncMove::set_move_pos(Vector3 p) { move_pos[1] = p; } Vector3 FuncMove::get_move_pos() { return move_pos[1]; }
void FuncMove::set_move_rot(Vector3 r) { move_rot = r; } Vector3 FuncMove::get_move_rot() { return move_rot; }
void FuncMove::set_move_scale(Vector3 s) { move_scale = s; } Vector3 FuncMove::get_move_scale() { return move_scale; }
void FuncMove::set_hinge_offset(Vector3 h) { hinge_offset = h; } Vector3 FuncMove::get_hinge_offset() { return hinge_offset; }
void FuncMove::set_speed(float s) { speed = s; } float FuncMove::get_speed() { return speed; }
void FuncMove::set_wait(float w) { wait = w; } float FuncMove::get_wait() { return wait; }
void FuncMove::set_loop(int l) { loop = (uint8_t)l; } int FuncMove::get_loop() { return (int)loop; }
void FuncMove::set_crush_damage(int d) { crush_damage = (int16_t)d; } int FuncMove::get_crush_damage() { return (int)crush_damage; }
void FuncMove::set_move_sound(String s) { move_sound = s; } String FuncMove::get_move_sound() { return move_sound; }
// Access
void FuncMove::set_locked(int l) { locked = (uint32_t)l; } int FuncMove::get_locked() { return (int)locked; }
void FuncMove::set_locked_message(String m) { locked_message = m; } String FuncMove::get_locked_message() { return locked_message; }
void FuncMove::set_locked_sound(String s) { locked_sound = s; } String FuncMove::get_locked_sound() { return locked_sound; }
void FuncMove::set_unlock_sound(String s) { unlock_sound = s; } String FuncMove::get_unlock_sound() { return unlock_sound; }

/*****************************************************
SCENE TREE MODIFICATION
*****************************************************/
void FuncMove::reparent_entity() {
    String moveparent = (String)properties["moveparent"];
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
            for (int i = 0; i < get_child_count(); i++) {
                Node* n = get_child(i);
                n->set_owner(get_owner());
            }
        }
}

void FuncMove::update_children() {
    float volume = 1.0f;
    // Block area, for crush detection
    block_area = nullptr;
    if (has_node("block_area") == false) {
        block_area = memnew(Area3D);
        add_child(block_area);
        block_area->set_owner(get_owner());
        block_area->set_name("block_area");
        block_area->set_collision_layer(0);
        block_area->set_monitorable(false);
        int layers = ACTOR_LAYER + DEAD_LAYER;
        block_area->set_collision_mask(layers);
        block_area->set_monitoring(true);
    }
    for (int i = 0; i < get_child_count(); i++) {
        if (block_area != nullptr && get_child(i)->get_class() == "CollisionShape3D") {
            CollisionShape3D* c = cast_to<CollisionShape3D>(get_child(i)->duplicate());
            block_area->add_child(c);
            c->set_owner(block_area->get_owner());
            c->set_shape(c->get_shape()->duplicate());
        }
        else if (get_child(i)->get_class() == "MeshInstance3D") {
            MeshInstance3D* m = cast_to<MeshInstance3D>(get_child(i));
            m->set_ignore_occlusion_culling(true);
            m->set_layer_mask(CULL_DYNAMIC);
            if (properties.has("cast_shadow"))
                m->set_cast_shadows_setting(GeometryInstance3D::ShadowCastingSetting((int)properties["cast_shadow"]));
            volume = fmaxf(m->get_mesh()->get_aabb().get_volume() * 0.5f, 3.0f);
        }
    }
    for (int i = 0; i < 2; i++) {
        if (has_node(NodePath("sfx" + String::num(i))) == false) {
            sfx[i] = memnew(AudioStreamPlayer3D);
            sfx[i]->set_bus("SfxWorld");
            sfx[i]->set_attenuation_filter_cutoff_hz(10000);
            sfx[i]->set_unit_size(fminf(volume, 10.6f));
            sfx[i]->set_max_distance(fminf(volume * 3.0f, 32.0f));
            add_child(sfx[i]);
            sfx[i]->set_name("sfx" + String::num(i));
            sfx[i]->set_owner(get_owner());
        }
    }
    call_deferred("update_hinge");
}

void FuncMove::update_hinge() {
    if (properties.has("hinge_target") && properties["hinge_target"] != "") {
        String h = properties["hinge_target"];
        if (is_inside_tree()) {
            Node* n = get_node_or_null(NodePath(h));
            if (n != nullptr && n->get_class() == "InfoNull")
                hinge_offset = cast_to<InfoNull>(n)->get_global_position() - get_global_position();
        }
        properties.erase("hinge_target");
    }
    if (hinge_offset != VECTOR3_ZERO) {
        for (int i = 0; i < get_child_count(); i++) {
            Node* n = get_child(i);
            if (n->has_method("set_position") && n->get("position").get_type() == Variant::VECTOR3)
                n->set("position", cast_to<Node3D>(n)->get_position() + hinge_offset);
        }
        hinge_offset = VECTOR3_ZERO;
        if (properties.has("hinge_offset"))
            properties.erase("hinge_offset");
    }
}

/*****************************************************
ACTIVATION
*****************************************************/
void FuncMove::use(Node* activator) {
    if (locked > 0) {
        if (activator->is_in_group("TRIGGER"))
            activator = cast_to<Trigger>(activator)->last_activator;
        if (activator != nullptr) {
            if (GAME->check_lock(activator, this)) {
                sfx[1]->set_stream(s_locked);
                sfx[1]->play();
                return;
            }
            sfx[1]->set_stream(s_unlock);
            sfx[1]->play();
            locked = 0;
        }
    }
    if (!locked && move_progress_target < 1.0f)
        mv_forward();
}

void FuncMove::toggle() {
    if (move_progress_target > 0.0f)
        mv_reverse();
    else
        mv_forward();
}

/*****************************************************
ACCESS
*****************************************************/
void FuncMove::lock() {
    locked = 1;
    sfx[1]->set_stream(s_locked);
    sfx[1]->play();
}

void FuncMove::unlock() {
    locked = 0;
    sfx[1]->set_stream(s_unlock);
    sfx[1]->play();
}

/*****************************************************
MOVEMENT
*****************************************************/
void FuncMove::mv_forward() {
    if (move_progress_target < 1.0f) {
        move_state = ST_MOVE;
        move_progress_target = 1.0f;
        play_move_sound();
    }
}

void FuncMove::mv_reverse(Node* ent) {
    if (ent != nullptr && (flags & FUNC_FLAG_PLAYER_ONLY) && !ent->is_in_group("ACTOR"))
        if (!(cast_to<Actor>(ent)->flags & Actor::FL_PLAYER))
            return;
    if (move_progress_target > 0.0f) {
        move_state = ST_MOVE;
        move_progress_target = 0.0f;
        play_move_sound();
    }
}

void FuncMove::mv_pause(bool set_pause) { move_paused = set_pause; }

void FuncMove::stop(float move_targ) {
    loop = LoopTypes::LOOP_NO;
    move_progress_target = move_targ;
}

void FuncMove::stop_at_end() { stop(1.0f); }
void FuncMove::stop_at_pos() { stop(move_progress_target); }
void FuncMove::loop_no() { loop = LoopTypes::LOOP_NO; }
void FuncMove::loop_pong() { loop = LoopTypes::LOOP_PINGPONG; play_move_sound(); move_state = ST_MOVE; }
void FuncMove::loop_wrap() { loop = LoopTypes::LOOP_WRAP; move_progress_target = 1.0f; play_move_sound(); move_state = ST_MOVE; }
void FuncMove::loop_wrap_reverse() { loop = LoopTypes::LOOP_WRAP; move_progress_target = -1.0f; play_move_sound(); move_state = ST_MOVE; }

void FuncMove::play_move_sound() {
    if (!s_move.is_null()) {
        sfx[0]->set_stream(s_move);
        sfx[0]->play();
        sfx[0]->seek(0.0f);
    }
}

void FuncMove::stop_move_sound() {
    if (sfx[0]->is_playing() && sfx[0]->get_stream()->get_class() == "AudioStreamOGGVorbis")
        sfx[0]->stop();
}

Vector3 FuncMove::get_rot_delta() {
    Vector3 r = VECTOR3_ZERO;
    if (move_rot.length() != 0.0f && move_progress != move_progress_target) {
        r = move_rot * speed * GAME->dt_phys;
        if (move_progress > move_progress_target)
            r *= -1.0f;
    }
    return r;
}

/*****************************************************
DATA
*****************************************************/
Dictionary FuncMove::data_save() {
    Dictionary data;
    data["origin"] = get_position();
    data["rotation"] = get_rotation();
    data["scale"] = get_scale();
    data["move_state"] = move_state;
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

void FuncMove::data_load(Dictionary data) {
    set_position(GameManager::str_to_vec3(data["origin"]));
    set_rotation(GameManager::str_to_vec3(data["rotation"]));
    set_scale(GameManager::str_to_vec3(data["scale"]));
    move_state = uint8_t((int)data["move_state"]);
    move_progress_target = data["move_progress_target"];
    move_progress = data["move_progress"];
    move_paused = data["move_paused"];
    locked = data["locked"];
    wait_ct = data["wait_ct"];
    if ((bool)data["sfx0_is_playing"] == true) {
        play_move_sound();
        sfx[0]->seek(data["sfx0_position"]);
    }
    else
        stop_move_sound();
}

/*****************************************************
STATE MANAGEMENT
*****************************************************/
void FuncMove::state_physics(float delta) {
    // Crush stuff
    if (move_state == ST_MOVE && crush_damage > 0 && move_progress != move_progress_target) {
        if (crush_ct > 0.0f)
            crush_ct -= delta;
        Vector3 mdir;
        if (move_progress_target == 1)
            mdir = move_pos[0].direction_to(move_pos[1]);
        else
            mdir = move_pos[1].direction_to(move_pos[0]);
        mdir *= speed * delta;
        block_area->set_position(mdir);
        TypedArray<Node3D> ents = block_area->get_overlapping_bodies();
        if (ents.size() > 0) {
            for (int i = 0; i < ents.size(); i++)
                if (cast_to<Node>(ents[i])->is_in_group("ACTOR")) {
                    Actor* e = cast_to<Actor>(ents[i]);
                    Transform3D t = e->get_global_transform();
                    t.origin += mdir;
                    Array act_c = e->space_state->intersect_shape(e->make_shape_query(t, 0.0f, MAP_LAYER, Array::make(this, e)));
                    if (act_c.size() > 0) {
                        if (crush_ct <= 0.0f) {
                            e->call("damage", this, this, crush_damage);
                            crush_ct = 0.1f;
                        }
                        if (e->health > 0) {
                            if (wait >= 0.0f)
                                move_progress_target = (move_progress_target != 0.0f) ? 0.0f : 1.0f;
                            else
                                mv_pause(true);
                        }
                    }
                }
        }
        else
            mv_pause(false);
    }
    // Continue movement
    if (!move_paused) {
        if (move_state == ST_MOVE) {
            // Back and forth loop
            if (loop == LoopTypes::LOOP_PINGPONG) {
                if (move_progress >= 1.0f)
                    move_progress_target = 0.0f;
                else if (move_progress <= 0.0f)
                    move_progress_target = 1.0f;
            }
            // Instant restart loop
            else if (loop == LoopTypes::LOOP_WRAP) {
                if (move_progress >= 1.0f && move_progress_target != 0.0f)
                    move_progress = 0.0f;
                else if (move_progress <= 0.0f && move_progress_target != 1.0f)
                    move_progress_target = 1.0f;
            }

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
            else {
                stop_move_sound();
                if (wait > 0.0f)
                    if (move_progress >= 1.0f || loop != LoopTypes::LOOP_NO) {
                        move_state = ST_WAIT;
                        wait_ct = wait;
                        return;
                    }
                if (loop == LoopTypes::LOOP_NO)
                    move_state = ST_READY;
            }
        }
        // We can hold the door for a bit
        else if (move_state == ST_WAIT && !move_paused) {
            if (wait_ct > 0.0f)
                wait_ct -= delta;
            else {
                if (loop == LoopTypes::LOOP_NO) {
                    move_progress_target = (move_progress_target != 0.0f) ? 0.0f : 1.0f;
                    play_move_sound();
                }
                move_state = ST_MOVE;
            }
        }
    }
}

/*****************************************************
BASE PROCESSING
*****************************************************/
FuncMove::FuncMove() {
    add_to_group("WORLD");
    add_to_group("FUNC_MOVE");
    add_to_group("SAV");
    set_sync_to_physics(false);
}

FuncMove::~FuncMove() {}

void FuncMove::_ready() {
    if (!Engine::get_singleton()->is_editor_hint()) {
        GAME = get_node<GameManager>(GAME_PATH);
        if (GAME->appearance_check(appearance_flags)) {
            queue_free();
            return;
        }
        // Child pointers
        block_area = get_node<Area3D>("block_area");
        for (int i = 0; i < 2; i++)
            sfx[i] = get_node<AudioStreamPlayer3D>(NodePath("sfx" + String::num(i)));
        s_move = GameManager::update_sound(move_sound);
        s_locked = GameManager::update_sound(locked_sound);
        s_unlock = GameManager::update_sound(unlock_sound);
        // Movement setup
        move_pos[0] = get_position();
        move_pos[1] += move_pos[0];
        if (speed > 0.0f)
            speed = 1.0f / speed;
        GAME->set_targetname(this, targetname);
        if (flags & FUNC_FLAG_START_OPEN) {
            move_progress_target = 1.0f;
            move_progress = 1.0f;
            set_position(move_pos[1]);
            set_rotation(move_rot);
            set_scale(move_scale);
        }
        if (loop != LoopTypes::LOOP_NO) {
            move_state = ST_MOVE;
            play_move_sound();
        }
    }
}

void FuncMove::_physics_process(float delta) {
    if (!Engine::get_singleton()->is_editor_hint())
        call("state_physics", GAME->dt_phys);
}



/******************************************************************************************

FUNC DOOR

Automatically opens when approached. Don't use with doors that need to open together.

******************************************************************************************/
void FuncDoor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("update_children"), &FuncDoor::update_children);
    ClassDB::bind_method(D_METHOD("use", "activator"), &FuncDoor::use);
    ClassDB::bind_method(D_METHOD("mv_reverse", "ent"), &FuncDoor::mv_reverse);
    ClassDB::bind_method(D_METHOD("state_physics", "delta"), &FuncDoor::state_physics);
}

void FuncDoor::update_children() {
    FuncMove::update_children();
    MeshInstance3D* m;
    for (int i = 0; i < get_child_count(); i++)
        // Clean up previous trigger area so we don't keep generating them
        if (get_child(i)->get_class() == "Area3D") {
            if (get_child(i)->get_name().find("trigger_area") != -1)
                get_child(i)->queue_free();
        }
        else if (get_child(i)->get_class() == "MeshInstance3D")
            m = cast_to<MeshInstance3D>(get_child(i));

    // Toggle doors don't get trigger_areas
    if (flags & FUNC_FLAG_TOGGLE)
        return;

    // Auto generate a trigger volume to open the door
    trigger_area = nullptr;
    if (m != nullptr) {
        trigger_area = memnew(Area3D);
        add_child(trigger_area);
        trigger_area->set_owner(get_owner());
        trigger_area->set_name("trigger_area");
        trigger_area->set_collision_layer(0);
        trigger_area->set_collision_mask(TRIGGER_LAYER);
        trigger_area->set_monitorable(false);
        trigger_area->set_monitoring(true);
        CollisionShape3D* c = memnew(CollisionShape3D);
        Ref<BoxShape3D> trigger_shape;
        trigger_shape.instantiate();
        trigger_shape->set_size(m->get_aabb().get_size() + VECTOR3_ONE * 1.875f);
        c->set_shape(trigger_shape);
        c->set_name("trigger_area_col");
        trigger_area->add_child(c);
        c->set_owner(get_owner());
    }
}

void FuncDoor::use(Node* activator) {
    if (flags & FUNC_FLAG_PLAYER_ONLY && activator->is_in_group("ACTOR"))
        if (!(cast_to<Actor>(activator)->flags & Actor::FL_PLAYER))
            return;
    FuncMove::use(activator);
}

void FuncDoor::mv_reverse(Node* ent) {
    TypedArray<Node3D> ents = trigger_area->get_overlapping_bodies();
    for (int i = 0; i < ents.size(); i++)
        if (cast_to<Node>(ents[i])->is_in_group("ACTOR"))
            return;
    FuncMove::mv_reverse(ent);
}

void FuncDoor::state_physics(float delta) {
    FuncMove::state_physics(delta);
    if (move_state == ST_MOVE)
        trigger_area->set_global_transform(trg_area_t);
}

FuncDoor::FuncDoor() {
    add_to_group("FUNC_DOOR");
}

FuncDoor::~FuncDoor() {}

void FuncDoor::_ready() {
    if (!Engine::get_singleton()->is_editor_hint()) {
        FuncMove::_ready();
        if (has_node("trigger_area")) {
            trigger_area = get_node<Area3D>("trigger_area");
            trg_area_t = get_global_transform();
            trigger_area->connect("body_entered", Callable(this, "use"));
            trigger_area->connect("body_exited", Callable(this, "mv_reverse"));
            if (flags & FUNC_FLAG_START_OPEN)
                trigger_area->set_global_transform(trg_area_t);
        }
        wait = 0.0f;
    }
}