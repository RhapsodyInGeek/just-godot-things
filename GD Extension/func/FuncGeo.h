/*******************************************************************************
FUNC_GEO
Main map geometry class.
*******************************************************************************/
class FuncGeo : public StaticBody3D {
    GDCLASS(FuncGeo, StaticBody3D)
protected:
    static void _bind_methods();
public:
    Dictionary properties = {};
    uint8_t appearance_flags = 0;
    uint32_t flags = 0;
    uint8_t grav_type = 0;
    FuncGeo();
    ~FuncGeo();
    void set_properties(Dictionary p); Dictionary get_properties();
    void set_appearance_flags(int f); int get_appearance_flags();
    void set_flags(int f); int get_flags();
    void set_grav_type(int g); int get_grav_type();
    void _ready();
};