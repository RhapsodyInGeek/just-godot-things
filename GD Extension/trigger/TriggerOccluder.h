/*******************************************************************************
TRIGGER OCCLUDER
Used in occlusion culling. Created in Trenchbroom by painting an Occluder Texture
on one face and Skip textures on all others.
*******************************************************************************/
#include <godot_cpp/classes/occluder_instance3d.hpp>

class TriggerOccluder : public OccluderInstance3D {
    GDCLASS(TriggerOccluder, OccluderInstance3D)
protected:
    static void _bind_methods();
public:
    Dictionary properties = {};
    uint8_t appearance_flags = 0;

    TriggerOccluder();
    ~TriggerOccluder();
    void set_properties(Dictionary p); Dictionary get_properties();
    void reparent_entity();
    void set_appearance_flags(int f); int get_appearance_flags();
    void _ready();
};