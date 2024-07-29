#ifndef TCFDX_PROP_MULTI_MESH_H
#define TCFDX_PROP_MULTI_MESH_H

/*************************************************************************
PROP MULTIMESH

Dynamic multimesh prop generator. Prop models are auto generated based upon
a customizable mesh resource path.

Target should be a world entity with a globalname, eg: func_geo, func_move.
**************************************************************************/
#include <godot_cpp/classes/multi_mesh_instance3d.hpp>
using namespace godot;

class PropMultiMesh : public MultiMeshInstance3D
{
    GDCLASS(PropMultiMesh, MultiMeshInstance3D)
public:
    String target = "";
    //String targetname = "";
    String target_mesh = "";
    String mesh_res_path = "";
    int amount = 128; // instance amount
    float random_rotation = 0.0f;
    float random_tilt = 0.0f;
    float base_scale = 1.0f;
    float random_scale = 0.0f;

    static void _bind_methods();

    void set_populate_multi_mesh(bool populate); bool get_populate_multi_mesh();
    void set_target(String t); String get_target();
    //void set_targetname(String t); String get_targetname();
    void set_target_mesh(String t); String get_target_mesh();
    void set_mesh_res_path(String p); String get_mesh_res_path();
    void set_amount(int a); int get_amount();
    void set_random_rotation(float r); float get_random_rotation();
    void set_random_tilt(float t); float get_random_tilt();
    void set_base_scale(float s); float get_base_scale();
    void set_random_scale(float s); float get_random_scale();

    void _populate_multi_mesh();
    void _func_godot_apply_properties(Dictionary props);
    void _func_godot_build_complete();

    PropMultiMesh();
    ~PropMultiMesh();
};

#endif