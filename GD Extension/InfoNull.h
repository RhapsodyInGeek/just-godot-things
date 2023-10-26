/*******************************************************************************
INFO NULL
Used for targeting and other reference stuff.
*******************************************************************************/
class InfoNull : public Node3D {
    GDCLASS(InfoNull, Node3D)
protected:
    static void _bind_methods();
public:
    Dictionary properties = {};
    String targetname = "";
    InfoNull();
    ~InfoNull();
    void set_properties(Dictionary new_properties);	Dictionary get_properties();
    void set_targetname(String t); String get_targetname();
    void _ready();
};