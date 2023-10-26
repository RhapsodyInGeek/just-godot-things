#include "GameDefs.h"
/*******************************************************************************
LIGHT DX

Light node governed by Trenchbroom key / value pairs.
When trying to set up lights for a map, set the "DEBUG" constant to "true".
*******************************************************************************/
#define LIGHT_STEP 0.0833333f

void SpotLightDx::_bind_methods() {
    // PROPERTIES
    // properties
    ClassDB::bind_method(D_METHOD("set_properties", "_properties"), &SpotLightDx::set_properties);
    ClassDB::bind_method(D_METHOD("get_properties"), &SpotLightDx::get_properties);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");
    // targetname
    ClassDB::bind_method(D_METHOD("set_targetname", "targetname"), &SpotLightDx::set_targetname);
    ClassDB::bind_method(D_METHOD("get_targetname"), &SpotLightDx::get_targetname);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "targetname"), "set_targetname", "get_targetname");
    // energy 1
    ClassDB::bind_method(D_METHOD("set_energy_1", "_energy"), &SpotLightDx::set_energy_1);
    ClassDB::bind_method(D_METHOD("get_energy_1"), &SpotLightDx::get_energy_1);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "energy_1", PROPERTY_HINT_RANGE, "0.0,16.0"), "set_energy_1", "get_energy_1");
    // energy 2
    ClassDB::bind_method(D_METHOD("set_energy_2", "_energy"), &SpotLightDx::set_energy_2);
    ClassDB::bind_method(D_METHOD("get_energy_2"), &SpotLightDx::get_energy_2);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "energy_2", PROPERTY_HINT_RANGE, "0.0,16.0"), "set_energy_2", "get_energy_2");
    // color 1
    ClassDB::bind_method(D_METHOD("set_color_1", "color"), &SpotLightDx::set_color_1);
    ClassDB::bind_method(D_METHOD("get_color_1"), &SpotLightDx::get_color_1);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color_1"), "set_color_1", "get_color_1");
    // color 2
    ClassDB::bind_method(D_METHOD("set_color_2", "color"), &SpotLightDx::set_color_2);
    ClassDB::bind_method(D_METHOD("get_color_2"), &SpotLightDx::get_color_2);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color_2"), "set_color_2", "get_color_2");
    // style
    ClassDB::bind_method(D_METHOD("set_style", "_style"), &SpotLightDx::set_style);
    ClassDB::bind_method(D_METHOD("get_style"), &SpotLightDx::get_style);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "style"), "set_style", "get_style");

    // METHODS
    ClassDB::bind_method(D_METHOD("reparent_entity"), &SpotLightDx::reparent_entity);
    ClassDB::bind_method(D_METHOD("update_style", "style_id"), &SpotLightDx::update_style);
    ClassDB::bind_method(D_METHOD("use", "activator"), &SpotLightDx::use);
    ClassDB::bind_method(D_METHOD("turn_on"), &SpotLightDx::turn_on);
    ClassDB::bind_method(D_METHOD("turn_off"), &SpotLightDx::turn_off);
    ClassDB::bind_method(D_METHOD("show_light"), &SpotLightDx::show_light);
    ClassDB::bind_method(D_METHOD("data_save"), &SpotLightDx::data_save);
    ClassDB::bind_method(D_METHOD("data_load", "data"), &SpotLightDx::data_load);
}

void SpotLightDx::set_properties(Dictionary new_properties) {
    properties = new_properties;
    if (!Engine::get_singleton()->is_editor_hint())
        return;
    if (properties.has("targetname"))
        targetname = (String)properties["targetname"];
    set_bake_mode(Light3D::BAKE_STATIC);
    // Spotlight direction
    set_rotation_degrees(GameManager::demangler(properties));
    if (properties.has("mangle")) {
        Vector3 m = properties["mangle"];
        set_rotation_degrees(Vector3(m.y, m.x + 180.0f, m.z));
    }
    else
        set_rotation_degrees(Vector3(0.0f, 180.0f, 0.0f));
    // Spotlight cone angle
    if (properties.has("angle"))
        set_param(Light3D::PARAM_SPOT_ANGLE, properties["angle"]);
    if (properties.has("delay"))
        set_param(Light3D::PARAM_SPOT_ATTENUATION, properties["delay"]);    
    // Default energy is 1.0f
    if (properties.has("light")) {
        set_negative(float(properties["light"]) < 0.0f);
        energy_1 = abs(float(properties["light"]));
        set_param(Light3D::PARAM_ENERGY, energy_1);
        set_param(Light3D::PARAM_INDIRECT_ENERGY, energy_1);
    }
    // Used for styles - we can interpolate between 2 energy levels, most useful for controlling subtle pulses or flickers
    if (properties.has("light2"))
        energy_2 = properties["light2"];
    // Range
    if (properties.has("wait"))
        set_param(Light3D::PARAM_RANGE, float(properties["wait"]) * INVERSE_SCALE);
    else
        set_param(Light3D::PARAM_RANGE, 9.375f); // equals 300 Trenchbroom units
    // Light falloff
    if (properties.has("delay"))
        set_param(Light3D::PARAM_ATTENUATION, properties["delay"]);
    if (properties.has("distance_fade_begin")) {
        float f = (float)properties["distance_fade_begin"];
        if (f < 0.0f)
            set_enable_distance_fade(false);
        else {
            set_enable_distance_fade(true);
            set_distance_fade_begin(f * INVERSE_SCALE);
        }
    }
    if (properties.has("distance_fade_length"))
        set_distance_fade_length((float)properties["distance_fade_length"] * INVERSE_SCALE);
    // Dynamic shadows can be expensive - use sparingly
    if (properties.has("shadows"))
        set_shadow(bool(properties["shadows"]));
    if (properties.has("distance_fade_shadow"))
        set_distance_fade_shadow((float)properties["distance_fade_shadow"] * INVERSE_SCALE);
    // Mood lighting
    if (properties.has("color")) {
        color_1 = properties["color"];
        color_2 = color_1;
        set_color(color_1);
    }
    // Used with style - we can interpolate between 2 colors
    if (properties.has("color2"))
        color_2 = properties["color2"];
    // Custom animation style - you can enter a custom a-z string to make unique light styles. Useful for custom levels.
    if (properties.has("custom_style")) {
        style = properties["custom_style"];
        style_vec.clear();
        for (int i = 0; i < style.length(); i++) {
            style_vec.push_back(float(String("abcdefghijklmnopqrstuvwxyz").find(String(&style[i]).left(1))) * LIGHT_STEP);
        }
    }
    // Animation style - please see get_light_style_pattern function in LightDefs.h
    else if (properties.has("style"))
        update_style(properties["style"]);
    // Lightmap setup
    if (style.length() > 1 || properties.has("targetname"))
        set_bake_mode(Light3D::BAKE_DISABLED);
    if (properties.has("start_off")) {
        set_visible(!bool(properties["start_off"]));
        if (is_visible() == false)
            set_bake_mode(Light3D::BAKE_DISABLED);
    }
    if (properties.has("moveparent"))
        call_deferred("reparent_entity");
}

Dictionary SpotLightDx::get_properties() { return properties; }
void SpotLightDx::set_targetname(String t) { targetname = t; } String SpotLightDx::get_targetname() { return targetname; }
void SpotLightDx::set_style(String s) { style = s; }
String SpotLightDx::get_style() { return style; }
void SpotLightDx::set_energy_1(float e) { energy_1 = e; }
float SpotLightDx::get_energy_1() { return energy_1; }
void SpotLightDx::set_energy_2(float e) { energy_2 = e; }
float SpotLightDx::get_energy_2() { return energy_2; }
void SpotLightDx::set_color_1(Color c) { color_1 = c; }
Color SpotLightDx::get_color_1() { return color_1; }
void SpotLightDx::set_color_2(Color c) { color_2 = c; }
Color SpotLightDx::get_color_2() { return color_2; }

void SpotLightDx::reparent_entity() {
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
            set_bake_mode(Light3D::BAKE_DISABLED);
        }
}

void SpotLightDx::use(Node* activator) {
    set_visible(!is_visible());
    lit = is_visible();
}

void SpotLightDx::turn_on() {
    lit = true;
    show();
}

void SpotLightDx::turn_off() {
    lit = false;
    hide();
}

void SpotLightDx::show_light() {
    if (lit)
        show();
}

// LIGHT STYLES
void SpotLightDx::update_style(int style_id) {
    style = LightDx::get_light_style_pattern(style_id);
    style_vec.clear();
    for (int i = 0; i < style.length(); i++)
        style_vec.push_back(float(String("abcdefghijklmnopqrstuvwxyz").find(String(&style[i]).left(1))) * LIGHT_STEP);
}

Dictionary SpotLightDx::data_save() {
    Dictionary data;
    data["on"] = is_visible();
    data["style"] = style;
    data["style_ct"] = style_ct;
    data["lit"] = lit;
    return data;
}

void SpotLightDx::data_load(Dictionary data) {
    set_visible(data["on"]);
    style = data["style"];
    style_ct = data["style_ct"];
    if (data.has("lit"))
        lit = data["lit"];
    add_to_group("SAV");
}

SpotLightDx::SpotLightDx() {
    set_layer_mask(LIGHT_LAYER_MASK);
}

SpotLightDx::~SpotLightDx() {}

void SpotLightDx::_ready() {
    if (!Engine::get_singleton()->is_editor_hint()) {
        lit = is_visible();
        if (get_bake_mode() != BAKE_STATIC)
            add_to_group("SAV");
        get_node<GameManager>(GAME_PATH)->set_targetname(this, targetname);
        style_vec.clear();
        for (int i = 0; i < style.length(); i++)
            style_vec.push_back(float(String("abcdefghijklmnopqrstuvwxyz").find(String(&style[i]).left(1))) * LIGHT_STEP);
    }
}

void SpotLightDx::_process(float delta) {
    if (style.length() > 1) {
        style_ct += delta;
        if (style_ct >= 3600.0f)
            style_ct -= 3600.0f;
        int i = int(floor(style_ct * 10.0f)) % style_vec.size();
        float t = style_vec[i];
        set_param(Light3D::PARAM_ENERGY, Math::lerp(energy_2, energy_1, t));
        set_color(color_2.lerp(color_1, t));
    }
}

