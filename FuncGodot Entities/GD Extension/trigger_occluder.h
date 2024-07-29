#ifndef BKG_TRIGGER_OCCLUDER_H
#define BKG_TRIGGER_OCCLUDER_H

/*******************************************************************************
TRIGGER OCCLUDER
Used in occlusion culling. Created in Trenchbroom by painting an Occluder Texture
on one face and Skip textures on all others.
*******************************************************************************/
#include <godot_cpp/classes/occluder_instance3d.hpp>
using namespace godot;

class TriggerOccluder : public OccluderInstance3D {
    GDCLASS(TriggerOccluder, OccluderInstance3D)
protected:
    static void _bind_methods();
public:
    uint8_t appearance_flags = 0;
    String move_parent = String();

    TriggerOccluder();
    ~TriggerOccluder();
    void set_appearance_flags(int f); int get_appearance_flags();
    void _func_godot_apply_properties(Dictionary props);
    void _func_godot_build_complete();
    void _ready();
};

#endif