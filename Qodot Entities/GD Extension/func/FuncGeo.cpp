#include "GameDefs.h"

/*******************************************************************************

FUNC_GEO

Main map geometry class.

*******************************************************************************/
void FuncGeo::_bind_methods() {
    // PROPERTIES
    // properties
    ClassDB::bind_method(D_METHOD("set_properties", "properties"), &FuncGeo::set_properties);
    ClassDB::bind_method(D_METHOD("get_properties"), &FuncGeo::get_properties);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");
    // appearance_flags
    ClassDB::bind_method(D_METHOD("set_appearance_flags", "appearance_flags"), &FuncGeo::set_appearance_flags);
    ClassDB::bind_method(D_METHOD("get_appearance_flags"), &FuncGeo::get_appearance_flags);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "appearance_flags", PROPERTY_HINT_FLAGS,
        APPEARANCE_FLAG_LIST), "set_appearance_flags", "get_appearance_flags");
    // flags
    ClassDB::bind_method(D_METHOD("set_flags", "flags"), &FuncGeo::set_flags);
    ClassDB::bind_method(D_METHOD("get_flags"), &FuncGeo::get_flags);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "flags", PROPERTY_HINT_FLAGS,
        FUNC_FLAG_LIST), "set_flags", "get_flags");
    // grav type
    ClassDB::bind_method(D_METHOD("set_grav_type", "grav_type"), &FuncGeo::set_grav_type);
    ClassDB::bind_method(D_METHOD("get_grav_type"), &FuncGeo::get_grav_type);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "grav_type", PROPERTY_HINT_ENUM, 
        GRAV_TYPE_LIST), "set_grav_type", "get_grav_type");
}

void FuncGeo::set_properties(Dictionary p) {
    properties = p;

    if (!Engine::get_singleton()->is_editor_hint())
        return;

    if (properties.has("appearance_flags"))
        appearance_flags = (uint8_t)(int)properties["appearance_flags"];
    if (properties.has("flags"))
        flags = (uint32_t)(int)properties["flags"];
    if (properties.has("grav_type"))
        grav_type = uint8_t((int)properties["grav_type"]);
    for (int i = 0; i < get_child_count(); i++) {
        if (get_child(i)->get_class() == "MeshInstance3D") {
            MeshInstance3D* m = cast_to<MeshInstance3D>(get_child(i));
            if (flags & FUNC_FLAG_NO_CULL)
                m->set_ignore_occlusion_culling(true);
            if (properties.has("cast_shadow"))
                m->set_cast_shadows_setting(GeometryInstance3D::ShadowCastingSetting((int)properties["cast_shadow"]));
        }
    }
}

Dictionary FuncGeo::get_properties() { return properties; }
void FuncGeo::set_appearance_flags(int f) { appearance_flags = (uint8_t)f; }
int FuncGeo::get_appearance_flags() { return (int)appearance_flags; }
void FuncGeo::set_flags(int f) { flags = (uint32_t)f; }
int FuncGeo::get_flags() { return (int)flags; }
void FuncGeo::set_grav_type(int g) { grav_type = (uint8_t)g; }
int FuncGeo::get_grav_type() { return (int)grav_type; }

FuncGeo::FuncGeo() {
    add_to_group("WORLD");
    add_to_group("FUNC_GEO");
    set_collision_layer(MAP_LAYER);
}

FuncGeo::~FuncGeo() {}

void FuncGeo::_ready() {
    if (!Engine::get_singleton()->is_editor_hint())
        if (get_node<GameManager>(GAME_PATH)->appearance_check(appearance_flags))
            queue_free();
}