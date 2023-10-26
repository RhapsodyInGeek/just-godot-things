/*******************************************************************************
FUNC_BREAK
Destructible geometry.
*******************************************************************************/
class FuncBreak : public Area3D {
    GDCLASS(FuncBreak, Area3D)
protected:
    static void _bind_methods();
public:
    GameManager* GAME;
    Dictionary properties = {};
    uint8_t appearance_flags = 0;
    uint32_t flags = 0;
    uint8_t grav_type = 0;
    int16_t health = 1;
    uint8_t bleed_type = 0;
    String targetname = "", target = "", targetfunc = "", message = "";
    String hit_sound = "", break_sound = "";
    Ref<RandomNumberGenerator> rng;
    Ref<AudioStream> s_hit, s_break;
    AudioStreamPlayer3D* sfx;

    FuncBreak();
    ~FuncBreak();
    // Properties
    void set_properties(Dictionary p); Dictionary get_properties();
    void set_appearance_flags(int f); int get_appearance_flags();
    void set_flags(int f); int get_flags();
    void set_grav_type(int g); int get_grav_type();
    void set_health(int h); int get_health();
    void set_bleed_type(int b); int get_bleed_type();
    void set_targetname(String t); String get_targetname();
    void set_target(String t); String get_target();
    void set_targetfunc(String f); String get_targetfunc();
    void set_message(String m); String get_message();
    void set_hit_sound(String path); String get_hit_sound();
    void set_break_sound(String path); String get_break_sound();
    void reparent_entity(); void update_children();
    // Methods
    void break_geo();
    void damage(int amount, Node* attack = nullptr, NodePath attacker = NodePath());
    void bleed(Transform3D hit_xform, int amount);
    void use(Node* activator);
    // Data
    Dictionary data_save();
    void data_load(Dictionary data);
    // Base Processing
    void _ready();
};