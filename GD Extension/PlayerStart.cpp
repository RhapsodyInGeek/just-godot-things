#include "GameDefs.h"
/*******************************************************************************
INFO_PLAYER_START
Handles spawning in the Player entity. Why not just spawn in the player directly?
Future proofing in case we want to do anything with multiplayer, and also to take
care of resetting / setting inventory.
*******************************************************************************/
void PlayerStart::_bind_methods() {
    // properties
    ClassDB::bind_method(D_METHOD("set_properties", "_properties"), &PlayerStart::set_properties);
    ClassDB::bind_method(D_METHOD("get_properties"), &PlayerStart::get_properties);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");
    // appearance flags
    ClassDB::bind_method(D_METHOD("set_appearance_flags", "_appearance_flags"), &PlayerStart::set_appearance_flags);
    ClassDB::bind_method(D_METHOD("get_appearance_flags"), &PlayerStart::get_appearance_flags);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "appearance_flags", PROPERTY_HINT_FLAGS, APPEARANCE_FLAG_LIST), "set_appearance_flags", "get_appearance_flags");
    // flags
    ClassDB::bind_method(D_METHOD("set_flags", "_flags"), &PlayerStart::set_flags);
    ClassDB::bind_method(D_METHOD("get_flags"), &PlayerStart::get_flags);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "flags", PROPERTY_HINT_FLAGS, "Single Player,Coop,Deathmatch,Team Deathmatch,Reset Health,Reset Weapons"), "set_flags", "get_flags");
    // index
    ClassDB::bind_method(D_METHOD("set_index", "_index"), &PlayerStart::set_index);
    ClassDB::bind_method(D_METHOD("get_index"), &PlayerStart::get_index);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "index", PROPERTY_HINT_RANGE, "0,255"), "set_index", "get_index");

    ClassDB::bind_method(D_METHOD("spawn_player"), &PlayerStart::spawn_player);
}

void PlayerStart::set_properties(Dictionary new_properties) {
    properties = new_properties;
    if (!Engine::get_singleton()->is_editor_hint())
        return;
    set_rotation_degrees(GameManager::demangler(properties));
    if (properties.has("appearance_flags"))
        appearance_flags = (uint8_t)(int)properties["appearance_flags"];
    if (properties.has("flags"))
        flags = (uint8_t)(int)properties["flags"];
    if (properties.has("index"))
        index = (uint8_t)(int)properties["index"];
}

Dictionary PlayerStart::get_properties() { return properties; }
void PlayerStart::set_appearance_flags(int f) { appearance_flags = (uint8_t)f; }
int PlayerStart::get_appearance_flags() { return (int)appearance_flags; }
void PlayerStart::set_flags(int f) { flags = (uint8_t)f; }
int PlayerStart::get_flags() { return (int)flags; }
void PlayerStart::set_index(int i) { index = (uint8_t)i; }
int PlayerStart::get_index() { return (int)index; }

void PlayerStart::spawn_player() {
    GameManager* GAME = get_node<GameManager>(GAME_PATH);
    if (((flags & FL_SINGLEPLAYER) && GAME->gamemode == SINGLEPLAYER) || ((flags & FL_COOP) && GAME->gamemode == COOP)) {
        if (flags & FL_RESET_HEALTH)
            GAME->reset_health();
        if (flags & FL_RESET_WEAPONS)
            GAME->reset_weapons();
        Player* p = cast_to<Player>(GAME->PLAYER_RES->instantiate());
        p->flags = 0;
        get_parent()->add_child(p);
        p->set_owner(get_parent());
        Transform3D t = get_global_transform();
        p->set_global_transform(t);
        p->grav_dir = -t.BASIS_Y;
        if (GAME->gamemode == SINGLEPLAYER)
            GAME->emit_signal("player_spawned", p->get_path());
        return;
    }
    if (flags & FL_DEATHMATCH && GAME->gamemode == DEATHMATCH)
        return;
    if ((flags & FL_TEAM_DEATHMATCH) && GAME->gamemode == TEAM_DEATHMATCH)
        return;
}

PlayerStart::PlayerStart() { add_to_group("SPAWNPOINT"); }
PlayerStart::~PlayerStart() {}

void PlayerStart::_ready() {
    if (!Engine::get_singleton()->is_editor_hint()) {
        GameManager* GAME = get_node<GameManager>(GAME_PATH);

        if (GAME->appearance_check(appearance_flags)) {
            queue_free();
            return;
        }

        if (((flags & FL_SINGLEPLAYER) && GAME->gamemode == GAMEMODES::SINGLEPLAYER) || ((flags & FL_COOP) && GAME->gamemode == GAMEMODES::COOP))
            call_deferred("spawn_player");
        else if (flags & FL_DEATHMATCH && GAME->gamemode == GAMEMODES::DEATHMATCH)
            return;
        else if ((flags & FL_TEAM_DEATHMATCH) && GAME->gamemode == GAMEMODES::TEAM_DEATHMATCH)
            return;
    };
}