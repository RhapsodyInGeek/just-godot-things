#include "GameDefs.h"
#include <godot_cpp/classes/lightmap_gi.hpp>
/**********************************************************
WORLDSPAWN CLASS
Manages overall map settings: WorldEnvironment vars,
gravity, etc...
***********************************************************/
void Worldspawn::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_properties", "_properties"), &Worldspawn::set_properties);
    ClassDB::bind_method(D_METHOD("get_properties"), &Worldspawn::get_properties);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");

    ClassDB::bind_method(D_METHOD("apply_lightmap_properties"), &Worldspawn::apply_lightmap_properties);
    ClassDB::bind_method(D_METHOD("set_brightness", "_brightness"), &Worldspawn::set_brightness);
}

void Worldspawn::set_properties(Dictionary p) {
    properties = p;
    if (!Engine::get_singleton()->is_editor_hint())
        return;

    // WORLD ENVIRONMENT
    Ref<Environment> env = memnew(Environment);
    // Base settings
    env->set_fog_enabled(false);
    env->set_tonemapper(Environment::TONE_MAPPER_FILMIC);
    env->set_glow_enabled(false);
    // Background
    env->set_background(Environment::BG_COLOR);
    if (properties.has("color_bg"))
        env->set_bg_color(properties["color_bg"]);
    else
        env->set_bg_color(Color());
    // Ambient light
    env->set_ambient_source(Environment::AMBIENT_SOURCE_COLOR);
    if (properties.has("color_ambient"))
        env->set_ambient_light_color(properties["color_ambient"]);
    else
        env->set_ambient_light_color(Color::hex(0xFFFFFFFF));
    if (properties.has("ambient_light"))
        env->set_ambient_light_energy(properties["ambient_light"]);
    else
        env->set_ambient_light_energy(0.0f);
    env->set_ambient_light_sky_contribution(0.0f);
    // Brightness setup
    env->set_adjustment_enabled(true);
    env->set_adjustment_brightness(1.0f);
    set_environment(env);

    call_deferred("apply_lightmap_properties");
}

Dictionary Worldspawn::get_properties() { return properties; }

void Worldspawn::apply_lightmap_properties() {
    // Find existing lightmap, else build a new one
    LightmapGI* lit;
    if (get_owner()->has_node("lightmap"))
        lit = get_owner()->get_node<LightmapGI>("lightmap");
    else {
        lit = memnew(LightmapGI);
        lit->set_name("lightmap");
        get_owner()->add_child(lit);
        lit->set_owner(get_owner());
    }
    lit->get_parent()->call_deferred("move_child", lit, 0);
    lit->set_layer_mask(LIGHT_LAYER_MASK);
    // Bake Quality
    if (properties.has("lit_quality"))
        lit->set_bake_quality(LightmapGI::BakeQuality((int)properties["lit_quality"]));
    else
        lit->set_bake_quality(LightmapGI::BakeQuality::BAKE_QUALITY_MEDIUM);
    // Bounces
    if (properties.has("lit_bounces"))
        lit->set_bounces((int)properties["lit_bounces"]);
    else
        lit->set_bounces(3);
    // Lightmapper Probes Subdivision
    if (properties.has("lit_probes_subdiv"))
        lit->set_generate_probes(LightmapGI::GenerateProbes((int)properties["lit_probes_subdiv"]));
    else
        lit->set_generate_probes(LightmapGI::GenerateProbes::GENERATE_PROBES_SUBDIV_8);
    // Use Denoiser
    if (properties.has("lit_denoiser"))
        lit->set_use_denoiser((bool)properties["lit_denoiser"]);
    else
        lit->set_use_denoiser(true);
}

void Worldspawn::set_brightness(float b) {
    get_environment()->set_adjustment_brightness(b);
}

Worldspawn::Worldspawn() {}
Worldspawn::~Worldspawn() {}

void Worldspawn::_ready() {
    if (!Engine::get_singleton()->is_editor_hint()) {
        GameManager* GAME = get_node<GameManager>(GAME_PATH);
        if (properties.has("music"))
            get_node<MusicManager>("/root/MUSIC")->music_play(properties["music"]);
        if (properties.has("gravity"))
            GAME->set_grav_strength(properties["gravity"]);
        else
            GAME->set_grav_strength(GRAV_STRENGTH);
        GAME->connect("brightness_updated", Callable(this, "set_brightness"));
        set_brightness(GAME->brightness);
    }
}