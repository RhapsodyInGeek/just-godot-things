/*******************************************************************************
TRIGGER OCCLUDER
Used in occlusion culling. Created in Trenchbroom by painting an Occluder Texture
on one face and Skip textures on all others.
*******************************************************************************/
#include "trigger_occluder.h"
#include <godot_cpp/classes/mesh_data_tool.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/polygon_occluder3d.hpp>
#include <godot_cpp/classes/engine.hpp>

void TriggerOccluder::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_func_godot_apply_properties", "props"), &TriggerOccluder::_func_godot_apply_properties);
}

void TriggerOccluder::set_appearance_flags(int f) { appearance_flags = (uint8_t)f; }
int TriggerOccluder::get_appearance_flags() { return (int)appearance_flags; }

void TriggerOccluder::_func_godot_apply_properties(Dictionary props) {
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
}

TriggerOccluder::TriggerOccluder() {}
TriggerOccluder::~TriggerOccluder() {}