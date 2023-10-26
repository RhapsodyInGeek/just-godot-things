/*******************************************************************************
SPOTLIGHT DX
Spotlight variant of light node governed by Trenchbroom key / value pairs.
*******************************************************************************/
#include <godot_cpp/classes/spot_light3d.hpp>

class SpotLightDx : public SpotLight3D {
    GDCLASS(SpotLightDx, SpotLight3D)
protected:
    static void _bind_methods();
public:
    Dictionary properties = {};
    String targetname = "";
    bool lit = true;
    float
        style_ct = 0.0f,
        energy_1 = 1.0f,
        energy_2 = 0.0f;
    Color
        color_1 = Color(1.0f, 1.0f, 1.0f),
        color_2 = Color();
    String style = "m";
    std::vector<float> style_vec = {};

    SpotLightDx();
    ~SpotLightDx();
    // Properties
    void set_properties(Dictionary new_properties);	Dictionary get_properties();
    void set_targetname(String t); String get_targetname();
    void set_style(String s); String get_style();
    void set_energy_1(float e); float get_energy_1();
    void set_energy_2(float e); float get_energy_2();
    void set_color_1(Color c); Color get_color_1();
    void set_color_2(Color c); Color get_color_2();
    void reparent_entity();
    // Scripting
    void use(Node* activator);
    void turn_on();
    void turn_off();
    void show_light();
    void update_style(int style_id);
    // Data
    Dictionary data_save();
    void data_load(Dictionary data);
    // Base Processing
    void _ready();
    void _process(float delta);
};