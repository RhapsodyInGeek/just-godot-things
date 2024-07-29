#ifndef TCFDX_PROP_DYNAMIC_H
#define TCFDX_PROP_DYNAMIC_H

/*************************************************************************
PROP DYNAMIC

Dynamic prop generator. Prop models are auto generated based upon a
customizable GLTF resource path.

Some props have access to an AnimationPlayer. Other props may have
StaticBody3D collision. We can provide optional access to these with
generic functions.
**************************************************************************/
#include <godot_cpp/classes/node3d.hpp>
using namespace godot;

class PropDynamic : public Node3D
{
    GDCLASS(PropDynamic, Node3D)
protected:
    static void _bind_methods();
public:
    uint8_t appearance_flags = 0;
    //uint32_t flags = 0;
    uint8_t grav_type = 1;
    String prop_res_path = "";
    int style = 0;
    String anim = "", next_anim = "";
    String targetname = "";
    String move_parent = "";

    void set_appearance_flags(int f); int get_appearance_flags();
    //void set_flags(int f); int get_flags();
    void set_grav_type(int g); int get_grav_type();
    void set_prop_res_path(String p); String get_prop_res_path();
    void update_prop();
    void set_anim(String a); String get_anim();
    void set_next_anim(String a); String get_next_anim();
    void set_targetname(String t); String get_targetname();
    void _func_godot_apply_properties(Dictionary props);
    void _func_godot_build_complete();
    void use(Node* activator = nullptr);
    PropDynamic();
    ~PropDynamic();
    void _ready();
};

#endif