/*******************************************************************************
FUNC_ILLUSIONARY
Non-solid geometry class. Used for detailing, decoration, secrets, etc...
*******************************************************************************/
class FuncIllusionary : public Node3D {
    GDCLASS(FuncIllusionary, Node3D)
protected:
    static void _bind_methods();
public:
    Dictionary properties = {};
    uint8_t appearance_flags = 0;
    uint32_t flags = 0;
    FuncIllusionary();
    ~FuncIllusionary();
    void set_properties(Dictionary p); Dictionary get_properties();
    void set_appearance_flags(int f); int get_appearance_flags();
    void set_flags(int f); int get_flags();
    void reparent_entity();
    void _ready();
};