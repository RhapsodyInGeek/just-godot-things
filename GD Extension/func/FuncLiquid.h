/*******************************************************************************
FUNC: LIQUID VOLUME
Entering this volume sets the Player's move state to swimming and tints the view.

COMMON TINTS
Water - 50 80 130 128
Slime - 0 25 5 150
Lava - 255 80 0 150
Space - ???

Certain liquid types autogenerate a hurt trigger.
*******************************************************************************/
class FuncLiquid : public Area3D {
    GDCLASS(FuncLiquid, Area3D)
protected:
    static void _bind_methods();
public:
    Dictionary properties = {};
    uint8_t appearance_flags = 0;
    uint8_t liquid_type = WATER;
    Color liquid_color = Color::hex(0x80325082);
    FuncLiquid();
    ~FuncLiquid();
    void set_properties(Dictionary p); Dictionary get_properties();
    void reparent_entity(); void update_children();
    void set_appearance_flags(int f); int get_appearance_flags();
    void set_liquid_type(int t); int get_liquid_type();
    void set_liquid_color(Color c); Color get_liquid_color();
    void _on_ent_entered(Node* ent);
    void _on_ent_exited(Node* ent);
    void _ready();
};