#include "GameDefs.h"

/************************************************
TRIGGER BASE CLASS
************************************************/
void Trigger::_bind_methods() {
    // PROPERTIES
    // properties
    ClassDB::bind_method(D_METHOD("set_properties", "properties"), &Trigger::set_properties);
    ClassDB::bind_method(D_METHOD("get_properties"), &Trigger::get_properties);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");
    ClassDB::bind_method(D_METHOD("update_properties"), &Trigger::update_properties);
    // appearance_flags
    ClassDB::bind_method(D_METHOD("set_appearance_flags", "appearance_flags"), &Trigger::set_appearance_flags);
    ClassDB::bind_method(D_METHOD("get_appearance_flags"), &Trigger::get_appearance_flags);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "appearance_flags", PROPERTY_HINT_FLAGS,
        APPEARANCE_FLAG_LIST), "set_appearance_flags", "get_appearance_flags");
    // flags
    ClassDB::bind_method(D_METHOD("set_flags", "flags"), &Trigger::set_flags);
    ClassDB::bind_method(D_METHOD("get_flags"), &Trigger::get_flags);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "flags", PROPERTY_HINT_FLAGS,
        TRIGGER_FLAG_LIST), "set_flags", "get_flags");
    ClassDB::bind_method(D_METHOD("apply_flags"), &Trigger::apply_flags);
    // targetname
    ClassDB::bind_method(D_METHOD("set_targetname", "targetname"), &Trigger::set_targetname);
    ClassDB::bind_method(D_METHOD("get_targetname"), &Trigger::get_targetname);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "targetname"), "set_targetname", "get_targetname");
    // target
    ClassDB::bind_method(D_METHOD("set_target", "target"), &Trigger::set_target);
    ClassDB::bind_method(D_METHOD("get_target"), &Trigger::get_target);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "target"), "set_target", "get_target");
    // targetfunc
    ClassDB::bind_method(D_METHOD("set_targetfunc", "targetfunc"), &Trigger::set_targetfunc);
    ClassDB::bind_method(D_METHOD("get_targetfunc"), &Trigger::get_targetfunc);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "targetfunc"), "set_targetfunc", "get_targetfunc");
    // message
    ClassDB::bind_method(D_METHOD("set_message", "message"), &Trigger::set_message);
    ClassDB::bind_method(D_METHOD("get_message"), &Trigger::get_message);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "message", PROPERTY_HINT_MULTILINE_TEXT), "set_message", "get_message");
    // delay
    ClassDB::bind_method(D_METHOD("set_delay", "delay"), &Trigger::set_delay);
    ClassDB::bind_method(D_METHOD("get_delay"), &Trigger::get_delay);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "delay"), "set_delay", "get_delay");
    // wait
    ClassDB::bind_method(D_METHOD("set_wait", "delay"), &Trigger::set_wait);
    ClassDB::bind_method(D_METHOD("get_wait"), &Trigger::get_wait);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "wait"), "set_wait", "get_wait");
    // health
    ClassDB::bind_method(D_METHOD("set_health", "health"), &Trigger::set_health);
    ClassDB::bind_method(D_METHOD("get_health"), &Trigger::get_health);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "health"), "set_health", "get_health");
    // health max
    ClassDB::bind_method(D_METHOD("set_health_max", "health_max"), &Trigger::set_health_max);
    ClassDB::bind_method(D_METHOD("get_health_max"), &Trigger::get_health_max);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "health_max"), "set_health_max", "get_health_max");
    // bleed type
    ClassDB::bind_method(D_METHOD("set_bleed_type", "bleed_type"), &Trigger::set_bleed_type);
    ClassDB::bind_method(D_METHOD("get_bleed_type"), &Trigger::get_bleed_type);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "bleed_type", PROPERTY_HINT_ENUM, 
        BLEED_TYPE_LIST), "set_bleed_type", "get_bleed_type");
    
    // METHODS
    ClassDB::bind_method(D_METHOD("reparent_entity"), &Trigger::reparent_entity);
    ClassDB::bind_method(D_METHOD("damage", "inflictor", "attacker", "amount"), &Trigger::damage);
    ClassDB::bind_method(D_METHOD("bleed", "hit_xform", "amount"), &Trigger::bleed);
    ClassDB::bind_method(D_METHOD("toggle_collision", "_toggled"), &Trigger::toggle_collision);
    ClassDB::bind_method(D_METHOD("_on_ent_entered", "ent"), &Trigger::_on_ent_entered);
    ClassDB::bind_method(D_METHOD("use", "activator"), &Trigger::use);
    ClassDB::bind_method(D_METHOD("_use2"), &Trigger::_use2);
    ClassDB::bind_method(D_METHOD("reset_trigger"), &Trigger::reset_trigger);
    ClassDB::bind_method(D_METHOD("get_last_activator"), &Trigger::get_last_activator);
    ClassDB::bind_method(D_METHOD("data_save"), &Trigger::data_save);
    ClassDB::bind_method(D_METHOD("data_load", "data"), &Trigger::data_load);
    ClassDB::bind_method(D_METHOD("state_idle", "delta"), &Trigger::state_idle);
    ClassDB::bind_method(D_METHOD("state_physics", "delta"), &Trigger::state_physics);
}

void Trigger::set_properties(Dictionary p) {
    properties = p;
    if (Engine::get_singleton()->is_editor_hint())
        call("update_properties");
}

Dictionary Trigger::get_properties() { return properties; }

void Trigger::update_properties() {
    if (properties.has("moveparent"))
        call_deferred("reparent_entity");
    if (properties.has("appearance_flags"))
        appearance_flags = (uint8_t)(int)properties["appearance_flags"];
    if (properties.has("flags"))
        flags = (uint32_t)(int)properties["flags"];
    if (properties.has("target"))
        target = (String)properties["target"];
    if (properties.has("targetfunc"))
        targetfunc = (String)properties["targetfunc"];
    if (properties.has("message"))
        message = String(properties["message"]).replace("  ", " ");
    if (properties.has("delay"))
        delay = (float)properties["delay"];
    if (properties.has("wait"))
        wait = (float)properties["wait"];
    int layers = TRIGGER_LAYER;
    if (properties.has("health") && (int)properties["health"] > 0) {
        health_max = (int16_t)(int)properties["health"];
        health = health_max;
        layers += ACTOR_LAYER;
    }
    if (properties.has("health_max"))
        health_max = (int16_t)(int)properties["health_max"];
    if (properties.has("bleed_type"))
        bleed_type = (uint8_t)(int)properties["bleed_type"];
    set_collision_mask(layers);
    set_collision_layer(layers);
    call("apply_flags");
}

void Trigger::set_appearance_flags(int f) { appearance_flags = (uint8_t)f; }
int Trigger::get_appearance_flags() { return (int)appearance_flags; }
void Trigger::set_flags(int f) { flags = (uint32_t)f; }
int Trigger::get_flags() { return (int)flags; }

void Trigger::apply_flags() {
    if (flags & TRIGGER_FLAG_CONTINUOUS) {
        if (wait < 0.0f)
            wait = 0.0f;
        if (health_max > 0)
            flags &= ~TRIGGER_FLAG_CONTINUOUS;
    }
}

void Trigger::set_targetname(String t) { targetname = t; } String Trigger::get_targetname() { return targetname; }
void Trigger::set_target(String t) { target = t; } String Trigger::get_target() { return target; }
void Trigger::set_targetfunc(String f) { targetfunc = f; } String Trigger::get_targetfunc() { return targetfunc; }
void Trigger::set_message(String m) { message = m; } String Trigger::get_message() { return message; }
void Trigger::set_delay(float t) { delay = t; } float Trigger::get_delay() { return delay; }
void Trigger::set_wait(float t) { wait = t; } float Trigger::get_wait() { return wait; }
void Trigger::set_health(int h) { health = (int16_t)h; } int Trigger::get_health() { return (int)health; }
void Trigger::set_health_max(int h) { health_max = (int16_t)h; } int Trigger::get_health_max() { return (int)health_max; }
void Trigger::set_bleed_type(int b) { bleed_type = (uint8_t)b; } int Trigger::get_bleed_type() { return (int)bleed_type; }

void Trigger::reparent_entity() {
    String moveparent = properties["moveparent"];
    moveparent = "entity_func_move_" + moveparent;
    if (get_parent()->get_name() == moveparent)
        return;
    if (is_inside_tree())
        if (get_parent()->has_node(NodePath(moveparent)))
        {
            Transform3D t = get_global_transform();
            Node* new_parent = get_parent()->get_node<Node>(NodePath(moveparent));
            get_parent()->remove_child(this);
            new_parent->add_child(this);
            set_global_transform(t);
            set_owner(new_parent->get_owner());
            for (int i = 0; i < get_child_count(); i++)
                get_child(i)->set_owner(get_owner());
        }
}

void Trigger::damage(Node* inflictor, Node* attacker, int amount) {
    if (health_max > 0) {
        if ((flags & TRIGGER_FLAG_PLAYER_ONLY) && !attacker->is_in_group("PLAYER"))
            return;
        health -= amount;
        if (health < 1) {
            toggle_collision(false);
            call("use", attacker);
        }
    }
}

void Trigger::bleed(Transform3D hit_xform, int amount) {
    Node3D* fx = GAME->get_bleed(bleed_type);
    add_child(fx);
    fx->set_transform(Transform3D(hit_xform.basis, fx->to_local(hit_xform.origin)));
}

void Trigger::toggle_collision(bool toggle) {
    for (int i = 0; i < get_child_count(); i++)
        if (get_child(i)->get_class() == "CollisionShape3D")
            get_child(i)->set_deferred("disabled", !toggle);
}

void Trigger::_on_ent_entered(Node* ent) {
    if (trigger_state == ST_READY)
        if (ent->is_in_group("ACTOR") && (int)ent->call("get_health") > 0)
            if (ent->is_in_group("PLAYER") || !(flags & TRIGGER_FLAG_PLAYER_ONLY))
                call("use", ent);
}

void Trigger::use(Node* activator) {
    if (trigger_state == ST_READY) {
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

void Trigger::_use2() {
    timeout = wait;
    trigger_state = ST_TRIGGERED;
    toggle_collision(false);
    if (target.find("ACTIVATOR") != -1 && last_activator != nullptr) // target keyword, allows us to specifically trigger the one that activated us
        last_activator->add_to_group("ACTIVATOR");
    GAME->use_targets(this, target);
    last_activator->call_deferred("remove_from_group", "ACTIVATOR");
}

void Trigger::reset_trigger() {
    if (!(flags & TRIGGER_FLAG_NO_TOUCH) || health_max > 0)
        toggle_collision(true);
    health = health_max;
    trigger_state = ST_READY;
}

Node* Trigger::get_last_activator() {
    return last_activator;
}

Dictionary Trigger::data_save() {
    Dictionary data = Dictionary();
    data["trigger_state"] = trigger_state;
    if (trigger_state == ST_TRIGGERED)
        toggle_collision(false);
    data["timeout"] = timeout;
    data["health"] = health;
    data["last_activator"] = last_activator->get_path();
    return data;
}

void Trigger::data_load(Dictionary data) {
    trigger_state = (uint8_t)(int)data["trigger_state"];
    timeout = (float)data["timeout"];
    health = (int16_t)(int)data["health"];
    last_activator = get_node_or_null((NodePath)data["last_activator"]);
}

Trigger::Trigger() {
    add_to_group("TRIGGER");
    add_to_group("SAV");
    set_monitoring(true);
    set_monitorable(true);
    trigger_callable = Callable(this, "_on_ent_entered");
    connect("body_entered", trigger_callable);
}

Trigger::~Trigger() {}

void Trigger::_ready() {
    if (!Engine::get_singleton()->is_editor_hint()) {
        GAME = get_node<GameManager>(GAME_PATH);
        if (GAME->appearance_check(appearance_flags)) {
            queue_free();
            return;
        }
        call("apply_flags");
        if ((flags & TRIGGER_FLAG_CONTINUOUS) || health > 0) {
            if (is_connected("body_entered", trigger_callable))
                disconnect("body_entered", trigger_callable);
        }
        else if (flags & TRIGGER_FLAG_NO_TOUCH) {
            if (is_connected("body_entered", trigger_callable))
                disconnect("body_entered", trigger_callable);
            set_monitoring(false);
            set_monitorable(false);
        }
        GAME->set_targetname(this, targetname);
        if (flags & TRIGGER_FLAG_ON_START)
            call_deferred("_use2");
    }
}

void Trigger::state_idle(float delta) {
    if (trigger_state == ST_TRIGGERED && wait >= 0.0f) {
        if (timeout > 0.0f)
            timeout -= delta;
        else
            call_deferred("reset_trigger");
    }
    else if (trigger_state == ST_DELAYED) {
        if (timeout > 0.0f)
            timeout -= delta;
        else
            call("_use2");
    }
}

void Trigger::state_physics(float delta) {
    if ((flags & TRIGGER_FLAG_CONTINUOUS) || (GAME->time < 0.1f && is_monitoring())) {
        if (timeout > 0.0f)
            timeout -= delta;
        else {
            Array ents = get_overlapping_bodies();
            for (int i = 0; i < ents.size(); i++)
                call("_on_ent_entered", ents[i]);
        }
    }
}

void Trigger::_process(float delta) {
    if (Engine::get_singleton()->is_editor_hint())
        return;
    if (GAME->gamestate != GS_LEVEL)
        return;
    call("state_idle", GAME->dt_idle);
}

void Trigger::_physics_process(float delta) {
    if (Engine::get_singleton()->is_editor_hint())
        return;
    if (GAME->gamestate != GS_LEVEL)
        return;
    call("state_physics", GAME->dt_phys);
}

/*******************************************************************************
TRIGGER: USE
A player can press the use key to trigger this.
*******************************************************************************/
void TriggerUse::_bind_methods() {
    ClassDB::bind_method(D_METHOD("reset_trigger"), &TriggerUse::reset_trigger);
}

void TriggerUse::reset_trigger() {
    toggle_collision(true);
    trigger_state = ST_READY;
}

TriggerUse::TriggerUse() {
    add_to_group("TRIGGER_USE");
    if (is_connected("body_entered", trigger_callable))
        disconnect("body_entered", trigger_callable);
    set_collision_layer(USE_LAYER);
    set_monitoring(false);
    set_monitorable(true);
}

TriggerUse::~TriggerUse() {}