#include "GameDefs.h"
/*******************************************************************************
TELEPORTER CLASS
If an Actor enters the teleport bounds, they warp to a random destination in a
group specified by "trg_target", as long as the destination has a Transform.
*******************************************************************************/
void Teleporter::_bind_methods() {
    // properties
    ClassDB::bind_method(D_METHOD("set_properties", "_properties"), &Teleporter::set_properties);
    ClassDB::bind_method(D_METHOD("get_properties"), &Teleporter::get_properties);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");
    // appearance flags
    ClassDB::bind_method(D_METHOD("set_appearance_flags", "_appearance_flags"), &Teleporter::set_appearance_flags);
    ClassDB::bind_method(D_METHOD("get_appearance_flags"), &Teleporter::get_appearance_flags);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "appearance_flags", PROPERTY_HINT_FLAGS,
        "Not On Easy,Not On Normal,Not On Hard,Not On Nightmare,Not In Deathmatch,Not In Team Deathmatch"), "set_appearance_flags", "get_appearance_flags");
    // targetname
    ClassDB::bind_method(D_METHOD("set_targetname", "_targetname"), &Teleporter::set_targetname);
    ClassDB::bind_method(D_METHOD("get_targetname"), &Teleporter::get_targetname);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "targetname"), "set_targetname", "get_targetname");
    // target
    ClassDB::bind_method(D_METHOD("set_target", "_target"), &Teleporter::set_target);
    ClassDB::bind_method(D_METHOD("get_target"), &Teleporter::get_target);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "target"), "set_target", "get_target");
    // message
    ClassDB::bind_method(D_METHOD("set_message", "_message"), &Teleporter::set_message);
    ClassDB::bind_method(D_METHOD("get_message"), &Teleporter::get_message);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "message", PROPERTY_HINT_MULTILINE_TEXT), "set_message", "get_message");
    // keep grav dir
    ClassDB::bind_method(D_METHOD("set_keep_grav_dir", "_keep"), &Teleporter::set_keep_grav_dir);
    ClassDB::bind_method(D_METHOD("get_keep_grav_dir"), &Teleporter::get_keep_grav_dir);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "keep_grav_dir"), "set_keep_grav_dir", "get_keep_grav_dir");

    ClassDB::bind_method(D_METHOD("_on_enter_teleporter", "ent"), &Teleporter::_on_enter_teleporter);
    ClassDB::bind_method(D_METHOD("use", "activator"), &Teleporter::use);
}

void Teleporter::set_properties(Dictionary p) {
    properties = p;
    if (!Engine::get_singleton()->is_editor_hint())
        return;
    if (properties.has("appearance_flags"))
        appearance_flags = (uint8_t)(int)properties["appearance_flags"];
    if (properties.has("targetname"))
        target = (String)properties["targetname"];
    if (properties.has("target"))
        target = (String)properties["target"];
    if (properties.has("message"))
        message = String(properties["message"]).replace("  ", " ");
    if (properties.has("keep_grav_dir"))
        keep_grav_dir = (bool)properties["keep_grav_dir"];
}

Dictionary Teleporter::get_properties() { return properties; }
void Teleporter::set_appearance_flags(int f) { appearance_flags = (uint8_t)f; } int Teleporter::get_appearance_flags() { return (int)appearance_flags; }
void Teleporter::set_targetname(String t) { targetname = t; } String Teleporter::get_targetname() { return targetname; }
void Teleporter::set_target(String t) { target = t; } String Teleporter::get_target() { return target; }
void Teleporter::set_message(String m) { message = m; } String Teleporter::get_message() { return message; }
void Teleporter::set_keep_grav_dir(bool k) { keep_grav_dir = k; } bool Teleporter::get_keep_grav_dir() { return keep_grav_dir; }

void Teleporter::_on_enter_teleporter(Node* ent) {
    if (ent->is_in_group("ACTOR")) {
        Actor* a = cast_to<Actor>(ent);
        if (a->teleport_delay > 0.0f)
            return;
        if (a->current_state == Actor::ST_GIBBED)
            return;
        Array destinations = get_tree()->get_nodes_in_group(target);
        if (destinations.size() > 0) {
            // In case we have multiple targets with the same targetname,
            // randomize the teleport destination. USE SPARINGLY!  ;)
            destinations.shuffle();
            Transform3D d = cast_to<Node3D>(destinations[0])->get_global_transform();
            if (keep_grav_dir)
                d = Transform3D(a->get_global_transform().basis, d.origin);
            a->teleport(d);
            a->teleport_delay = 0.7f;
        }
    }
    else if (ent->is_in_group("PROJECTILE") || ent->is_in_group("GIB")) {
        Node3D* tf = GAME->get_telefog(1);
        add_child(tf);
        tf->set_global_transform(ent->get("global_transform"));
        ent->queue_free();
    }
}

void Teleporter::use(Node* activator) {
    for (int e = 0; e < 2; e++) {
        Array entities;
        if (e == 0)
            entities = get_overlapping_bodies();
        else
            entities = get_overlapping_areas();
        for (int i = 0; i < entities.size(); i++) {
            _on_enter_teleporter(cast_to<Node>(entities[i]));
        }
    }
}

Teleporter::Teleporter() {
    add_to_group("TELEPORTER");
    connect("body_entered", Callable(this, "_on_enter_teleporter"));
    connect("area_entered", Callable(this, "_on_enter_teleporter"));
}

Teleporter::~Teleporter() {}

void Teleporter::_ready() {
    if (!Engine::get_singleton()->is_editor_hint() && properties.has("targetname")) {
        GAME = get_node<GameManager>(GAME_PATH);
        if (GAME->appearance_check(appearance_flags)) {
            queue_free();
            return;
        }
        if (targetname != "") {
            disconnect("body_entered", Callable(this, "_on_enter_teleporter"));
            disconnect("area_entered", Callable(this, "_on_enter_teleporter"));
            GAME->set_targetname(this, targetname);
        }
    }
}