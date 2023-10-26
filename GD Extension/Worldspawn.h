/**********************************************************
WORLDSPAWN CLASS
Manages overall map settings: WorldEnvironment vars,
gravity, etc...
***********************************************************/
class Worldspawn : public WorldEnvironment {
    GDCLASS(Worldspawn, WorldEnvironment)
protected:
    static void _bind_methods();
public:
    Dictionary properties = {};
    Worldspawn();
    ~Worldspawn();
    void set_properties(Dictionary p); Dictionary get_properties();
    void apply_lightmap_properties();
    void set_brightness(float b);
    void _ready();
};