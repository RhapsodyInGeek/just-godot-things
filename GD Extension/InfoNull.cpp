/*******************************************************************************

INFO NULL

Used for targeting and other reference stuff.

*******************************************************************************/
void InfoNull::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_properties", "_properties"), &InfoNull::set_properties);
    ClassDB::bind_method(D_METHOD("get_properties"), &InfoNull::get_properties);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");

    // targetname
    ClassDB::bind_method(D_METHOD("set_targetname", "targetname"), &InfoNull::set_targetname);
    ClassDB::bind_method(D_METHOD("get_targetname"), &InfoNull::get_targetname);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "targetname"), "set_targetname", "get_targetname");
}

void InfoNull::set_properties(Dictionary p) {
    properties = p;
    if (!Engine::get_singleton()->is_editor_hint())
        return;
    set_rotation_degrees(GameManager::demangler(properties));
    if (properties.has("name") && (String)properties["name"] != "")
        set_name((String)properties["name"]);
    if (properties.has("targetname"))
        targetname = (String)properties["targetname"];
}

Dictionary InfoNull::get_properties() { return properties; }
void InfoNull::set_targetname(String t) { targetname = t; } String InfoNull::get_targetname() { return targetname; }

InfoNull::InfoNull() {}
InfoNull::~InfoNull() {}

void InfoNull::_ready() {
    if (!Engine::get_singleton()->is_editor_hint())
        get_node<GameManager>(GAME_PATH)->set_targetname(this, targetname);
}