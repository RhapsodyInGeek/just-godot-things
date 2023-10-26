/*******************************************************************************
TELEPORTER CLASS
If an Actor enters the teleport bounds, they warp to a random destination in a
group specified by "trg_target", as long as the destination has a Transform.
********************************************************************************/
class Teleporter : public Area3D {
    GDCLASS(Teleporter, Area3D)
protected:
    static void _bind_methods();
public:
    GameManager* GAME;
    Dictionary properties = {};
    uint8_t appearance_flags = 0;
    String targetname = "", target = "", message = "";
    bool keep_grav_dir = false;
    Teleporter();
    ~Teleporter();
    void set_properties(Dictionary p); Dictionary get_properties();
    void set_appearance_flags(int f); int get_appearance_flags();
    void set_targetname(String t); String get_targetname();
    void set_target(String t); String get_target();
    void set_message(String m); String get_message();
    void set_keep_grav_dir(bool k);
    bool get_keep_grav_dir();
    void _on_enter_teleporter(Node* ent);
    void use(Node* activator);
    void _ready();
};