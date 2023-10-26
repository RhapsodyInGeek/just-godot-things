#include "GameDefs.h"
#include <godot_cpp/classes/polygon_occluder3d.hpp>
/*******************************************************************************
TRIGGER OCCLUDER
Used in occlusion culling. Created in Trenchbroom by painting an Occluder Texture
on one face and Skip textures on all others.
*******************************************************************************/
void TriggerOccluder::_bind_methods() {
    // properties
    ClassDB::bind_method(D_METHOD("set_properties", "_properties"), &TriggerOccluder::set_properties);
    ClassDB::bind_method(D_METHOD("get_properties"), &TriggerOccluder::get_properties);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");
    // appearance_flags
    ClassDB::bind_method(D_METHOD("set_appearance_flags", "_appearance_flags"), &TriggerOccluder::set_appearance_flags);
    ClassDB::bind_method(D_METHOD("get_appearance_flags"), &TriggerOccluder::get_appearance_flags);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "appearance_flags", PROPERTY_HINT_FLAGS,
        APPEARANCE_FLAG_LIST), "set_appearance_flags", "get_appearance_flags");

    ClassDB::bind_method(D_METHOD("reparent_entity"), &TriggerOccluder::reparent_entity);
}

void TriggerOccluder::set_properties(Dictionary p) {
    properties = p;
    if (!Engine::get_singleton()->is_editor_hint())
        return;
    for (int i = 0; i < get_child_count(); i++) {
        if (get_child(i)->get_class() == "MeshInstance3D") {
            MeshInstance3D* m = cast_to<MeshInstance3D>(get_child(i));
            Ref<MeshDataTool> mdt;
            mdt.instantiate();
            mdt->create_from_surface(m->get_mesh(), 0);
            Vector3 n = mdt->get_face_normal(0);
            Vector3 axis = n.cross(VECTOR3_FORWARD);
            if (axis.length() < 1.0f)
                axis = n.cross(VECTOR3_UP);
            if (axis.length() < 1.0f)
                axis = n.cross(VECTOR3_RIGHT);
            float depth = 0.0f;
            // Save the mesh basis before the next part
            Basis mesh_basis = m->get_global_transform().basis;
            // Orient the occluder to the face normal
            Basis b = get_transform().basis;
            if (n != b[1] && -n != b[1])
                look_at(to_global(n), get_global_transform().BASIS_Y);
            else if (n != b[2] && -n != b[2])
                look_at(to_global(n), get_global_transform().BASIS_Z);
            else if (n != b[0] && -n != b[0])
                look_at(to_global(n), get_global_transform().BASIS_X);
            // Reorient the mesh
            m->set_global_transform(Transform3D(mesh_basis, m->get_global_position()));
            // Get the vertex points and convert them from Vector3 to Vector2
            PackedVector2Array pts;
            for (int j = 0; j < mdt->get_vertex_count(); j++)
            {
                Vector3 v = mdt->get_vertex(j);
                v = to_local(v + get_global_position());
                depth = v.z;
                pts.append(Vector2(v.x, v.y));
            }
            Ref<PolygonOccluder3D> poly;
            poly.instantiate();
            poly->set_polygon(pts);
            set_occluder(poly);
            translate(Vector3(0.0f, 0.0f, depth));
        }
        get_child(i)->queue_free();
    }
    if (properties.has("appearance_flags"))
        appearance_flags = (uint8_t)(int)properties["appearance_flags"];
    if (properties.has("moveparent"))
        call_deferred("reparent_entity");
}

Dictionary TriggerOccluder::get_properties() { return properties; }

void TriggerOccluder::set_appearance_flags(int f) { appearance_flags = (uint8_t)f; }
int TriggerOccluder::get_appearance_flags() { return (int)appearance_flags; }

void TriggerOccluder::reparent_entity() {
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
        }
}

TriggerOccluder::TriggerOccluder() {}
TriggerOccluder::~TriggerOccluder() {}

void TriggerOccluder::_ready() {
    if (!Engine::get_singleton()->is_editor_hint())
        if (get_node<GameManager>(GAME_PATH)->appearance_check(appearance_flags))
            queue_free();
}