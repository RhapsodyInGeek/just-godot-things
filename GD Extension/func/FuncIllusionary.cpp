/*******************************************************************************

FUNC_ILLUSIONARY

Non-solid geometry class. Used for detailing, decoration, secrets, etc...

*******************************************************************************/
void FuncIllusionary::_bind_methods() {
    // PROPERTIES
    // properties
    ClassDB::bind_method(D_METHOD("set_properties", "_properties"), &FuncIllusionary::set_properties);
    ClassDB::bind_method(D_METHOD("get_properties"), &FuncIllusionary::get_properties);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");
    // appearance_flags
    ClassDB::bind_method(D_METHOD("set_appearance_flags", "_appearance_flags"), &FuncIllusionary::set_appearance_flags);
    ClassDB::bind_method(D_METHOD("get_appearance_flags"), &FuncIllusionary::get_appearance_flags);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "appearance_flags", PROPERTY_HINT_FLAGS,
        APPEARANCE_FLAG_LIST), "set_appearance_flags", "get_appearance_flags");
    // flags
    ClassDB::bind_method(D_METHOD("set_flags", "_flags"), &FuncIllusionary::set_flags);
    ClassDB::bind_method(D_METHOD("get_flags"), &FuncIllusionary::get_flags);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "flags", PROPERTY_HINT_FLAGS,
        FUNC_FLAG_LIST), "set_flags", "get_flags");
    // targetname
    ClassDB::bind_method(D_METHOD("set_targetname", "targetname"), &FuncBreak::set_targetname);
    ClassDB::bind_method(D_METHOD("get_targetname"), &FuncBreak::get_targetname);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "targetname"), "set_targetname", "get_targetname");

    // METHODS
    ClassDB::bind_method(D_METHOD("reparent_entity"), &FuncIllusionary::reparent_entity);
}

void FuncIllusionary::set_properties(Dictionary p) {
    properties = p;
    if (!Engine::get_singleton()->is_editor_hint())
        return;
    if (properties.has("moveparent"))
        call_deferred("reparent_entity");
    if (properties.has("appearance_flags"))
        appearance_flags = (uint8_t)(int)properties["appearance_flags"];
    if (properties.has("flags"))
        flags = (uint32_t)(int)properties["flags"];
    for (int i = 0; i < get_child_count(); i++) {
        if (get_child(i)->get_class() == "MeshInstance3D") {
            MeshInstance3D* m = cast_to<MeshInstance3D>(get_child(i));
            m->set_gi_mode(GeometryInstance3D::GI_MODE_STATIC);
            if (properties.has("shadowcast"))
                m->set_cast_shadows_setting(GeometryInstance3D::ShadowCastingSetting((int)properties["shadowcast"]));
            else
                m->set_cast_shadows_setting(GeometryInstance3D::SHADOW_CASTING_SETTING_ON);
            if (m->get_mesh()->get_class() == "ArrayMesh")
                Ref<ArrayMesh>(m->get_mesh())->lightmap_unwrap(Transform3D(), INVERSE_SCALE);
            if (flags & FUNC_FLAG_NO_CULL)
                m->set_ignore_occlusion_culling(true);
        }
    }
}

Dictionary FuncIllusionary::get_properties() { return properties; }
void FuncIllusionary::set_appearance_flags(int f) { appearance_flags = (uint8_t)f; }
int FuncIllusionary::get_appearance_flags() { return (int)appearance_flags; }
void FuncIllusionary::set_flags(int f) { flags = (uint32_t)f; }
int FuncIllusionary::get_flags() { return flags; }

void FuncIllusionary::reparent_entity() {
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
            for (int i = 0; i < get_child_count(); i++) {
                Node* n = get_child(i);
                n->set_owner(get_owner());
                if (n->get_class() == "MeshInstance3D") {
                    cast_to<MeshInstance3D>(n)->set_layer_mask(CULL_DYNAMIC);
                    cast_to<MeshInstance3D>(n)->set_ignore_occlusion_culling(true);
                }
            }
        }
}

FuncIllusionary::FuncIllusionary() {
    add_to_group("WORLD");
    add_to_group("FUNC_ILLUSIONARY");
}

FuncIllusionary::~FuncIllusionary() {}

void FuncIllusionary::_ready() {
    if (!Engine::get_singleton()->is_editor_hint())
        if (get_node<GameManager>(GAME_PATH)->appearance_check(appearance_flags))
            queue_free();
}