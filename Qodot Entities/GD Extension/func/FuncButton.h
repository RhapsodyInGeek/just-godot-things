/*******************************************************************************
FUNC: BUTTON
Visual trigger really.Intended to be used with func_move objects like doors or
elevators, but technically should be able to trigger anything.
*******************************************************************************/
class FuncButton : public Trigger {
    GDCLASS(FuncButton, Trigger)
protected:
    static void _bind_methods();
public:
    uint8_t lock_flags = 0;
    MeshInstance3D* button_mesh;
    Ref<ShaderMaterial> button_mat;
    Vector3 move_pos[2] = { Vector3() }, move_rot = Vector3(), move_scale = Vector3(1.0f, 1.0f, 1.0f);
    Vector3 hinge_offset = VECTOR3_ZERO;
    float move_progress = 0.0f, move_progress_target = 0.0f, speed = 1.0f;
    uint32_t locked = 0;
    String press_sound, locked_sound, unlock_sound, locked_message;
    AudioStreamPlayer3D* sfx;
    Ref<AudioStream> s_pressed, s_locked, s_unlock;

    // Properties
    void update_properties();
    void set_lock_flags(int f); int get_lock_flags();
    void set_move_pos(Vector3 p); Vector3 get_move_pos();
    void set_move_rot(Vector3 r); Vector3 get_move_rot();
    void set_move_scale(Vector3 s); Vector3 get_move_scale();
    void set_hinge_offset(Vector3 h); Vector3 get_hinge_offset();
    void set_speed(float s); float get_speed();
    void set_locked(int l); int get_locked();
    void set_locked_message(String m); String get_locked_message();
    void set_press_sound(String s); String get_press_sound();
    void set_locked_sound(String s); String get_locked_sound();
    void set_unlock_sound(String s); String get_unlock_sound();
    void reparent_entity();
    void update_children();
    void update_hinge();
    // Methods
    void use(Node* activator);
    void reset_trigger();
    void lock(); void unlock();
    // Data
    Dictionary data_save();
    void data_load(Dictionary data);
    // State Management
    void state_physics(float delta);
    // Base Procesisng
    FuncButton();
    ~FuncButton();
    void _ready();
};