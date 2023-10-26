#include "GameDefs.h"
/*******************************************************************************
TRIGGER HURT
Any Actors in this volume get beat up.
*******************************************************************************/
void TriggerHurt::_bind_methods() {
    // damage
    ClassDB::bind_method(D_METHOD("set_damage", "_damage"), &TriggerHurt::set_damage);
    ClassDB::bind_method(D_METHOD("get_damage"), &TriggerHurt::get_damage);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "damage"), "set_damage", "get_damage");
    // can gib
    ClassDB::bind_method(D_METHOD("set_can_gib", "_can_gib"), &TriggerHurt::set_can_gib);
    ClassDB::bind_method(D_METHOD("get_can_gib"), &TriggerHurt::get_can_gib);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "can_gib"), "set_can_gib", "get_can_gib");

    ClassDB::bind_method(D_METHOD("update_properties"), &TriggerHurt::update_properties);
    ClassDB::bind_method(D_METHOD("apply_flags"), &TriggerHurt::apply_flags);
    ClassDB::bind_method(D_METHOD("_on_ent_entered", "ent"), &TriggerHurt::_on_ent_entered);
    ClassDB::bind_method(D_METHOD("_on_ent_exited", "ent"), &TriggerHurt::_on_ent_exited);
    ClassDB::bind_method(D_METHOD("use", "activator"), &TriggerHurt::use);
    ClassDB::bind_method(D_METHOD("state_physics", "delta"), &TriggerHurt::state_physics);
}

void TriggerHurt::update_properties() {
    Trigger::update_properties();
    health_max = 0;
    health = 0;
    if (properties.has("damage"))
        damage = (int16_t)(int)properties["damage"];
    if (!delay)
        delay = 1.0f;
    if (properties.has("can_gib"))
        can_gib = (bool)properties["can_gib"];
}

void TriggerHurt::apply_flags() {}

void TriggerHurt::set_damage(int d) { damage = d; }
int TriggerHurt::get_damage() { return damage; }
void TriggerHurt::set_delay(float d) { delay = d; }
float TriggerHurt::get_delay() { return delay; }
void TriggerHurt::set_can_gib(bool g) { can_gib = g; }
bool TriggerHurt::get_can_gib() { return can_gib; }

void TriggerHurt::_on_ent_entered(Node* ent) {
    if (ent->is_in_group("ACTOR")) {
        if ((flags & TRIGGER_FLAG_PLAYER_ONLY) && !(cast_to<Actor>(ent)->flags & Actor::FL_PLAYER))
            return;
        ent_timer new_ent;
        new_ent.ent = cast_to<Actor>(ent);
        new_ent.delay = 0.0f;
        ent_timers.push_back(new_ent);
    }
}

void TriggerHurt::_on_ent_exited(Node* ent) {
    for (std::deque<ent_timer>::iterator itr = ent_timers.begin(); itr != ent_timers.end(); itr++)
        if (itr->ent == ent) {
            ent_timers.erase(itr);
            return;
        }
}

void TriggerHurt::use(Node* activator) {
    Array ents = get_overlapping_bodies();
    for (int i = 0; i < ents.size(); i++) {
        Node* ent = cast_to<Node>(ents[i]);
        if (ent->is_in_group("ACTOR")) {
            if ((flags & TRIGGER_FLAG_PLAYER_ONLY) && !(cast_to<Actor>(ent)->flags & Actor::FL_PLAYER))
                continue;
            ent->call("damage", this, this, damage);
            ent->call("knockback", GameManager::rand_vec3(), damage * 0.1f);
        }
    }
}

TriggerHurt::TriggerHurt() {
    add_to_group("TRIGGER_HURT");
    set_monitorable(false);
    connect("body_exited", Callable(this, "_on_ent_exited"));
}

TriggerHurt::~TriggerHurt() {}

void TriggerHurt::state_physics(float delta) {
    Trigger::state_physics(delta);
    if (ent_timers.size() > 0)
        for (std::deque<ent_timer>::iterator itr = ent_timers.begin(); itr != ent_timers.end(); itr++) {
            itr->delay -= delta;
            if (itr->delay < 0.0f) {
                if (itr->ent != nullptr) {
                    if (itr->ent->health > 0 || can_gib) {
                        itr->ent->call("damage", this, this, damage);
                        itr->ent->call("knockback", GameManager::rand_vec3(), damage * 0.1f);
                        itr->delay = delay;
                    }
                }
                else
                    ent_timers.erase(itr);
            }
        }
}