/*******************************************************************************
FUNC_MOVE
Moving brush. Primarily used for doors, but can also be used for platforms and
even decorations.
*******************************************************************************/

class FuncMove : public AnimatableBody3D {
    GDCLASS(FuncMove, AnimatableBody3D)
protected:
    static void _bind_methods();
public:
    GameManager* GAME;
    Dictionary properties = {};
    uint8_t appearance_flags = 0;
    uint32_t flags = 0;
    String targetname = "";
    uint8_t lock_flags = 0;
    uint8_t grav_type = 0;
    enum { ST_READY, ST_MOVE, ST_WAIT };
    uint8_t move_state = ST_READY;
    Vector3 move_pos[2], move_rot = VECTOR3_ZERO, move_scale = VECTOR3_ONE;
    Vector3 hinge_offset = VECTOR3_ZERO;
    float speed = 1.0f, wait = -1.0f, wait_ct = 0.0f;
    float move_progress = 0.0f, move_progress_target = 0.0f, crush_ct = 0.0f;
    int8_t loop = 0;
    uint32_t locked = 0;
    int16_t crush_damage = 2;
    bool move_paused = false;
    Transform3D trg_area_t = Transform3D();
    String locked_message = "", move_sound = "", locked_sound = "", unlock_sound = "";
    Area3D* block_area; // block area used for crush detection
    AudioStreamPlayer3D* sfx[2];
    Ref<AudioStream> s_move, s_locked, s_unlock;
    
    FuncMove();
    ~FuncMove();
    // Properties
    void set_properties(Dictionary p); Dictionary get_properties();
    void set_appearance_flags(int f); int get_appearance_flags();
    void set_flags(int f); int get_flags();
    void set_targetname(String t); String get_targetname();
    void set_lock_flags(int f); int get_lock_flags();
    void set_grav_type(int g); int get_grav_type();
    void set_hinge_offset(Vector3 h); Vector3 get_hinge_offset();
    void set_move_pos(Vector3 p); Vector3 get_move_pos();
    void set_move_rot(Vector3 r); Vector3 get_move_rot();
    void set_move_scale(Vector3 s); Vector3 get_move_scale();
    void set_speed(float s); float get_speed();
    void set_wait(float w); float get_wait();
    void set_loop(int l); int get_loop();
    void set_locked(int l); int get_locked();
    void set_crush_damage(int d); int get_crush_damage();
    void set_locked_message(String m); String get_locked_message();
    void set_move_sound(String s); String get_move_sound();
    void set_locked_sound(String s); String get_locked_sound();
    void set_unlock_sound(String s); String get_unlock_sound();
    // Scene Tree Modification
    void reparent_entity();
    void update_children();
    void update_hinge();
    // Activation
    void use(Node* activator);
    void toggle();
    // Access
    void lock();
    void unlock();
    // Movement
    void mv_forward();
    void mv_reverse(Node* ent = nullptr);
    void mv_pause(bool set_pause = true);
    void stop(float move_targ = 0.0f);
    void stop_at_end();
    void stop_at_pos();
    void loop_no();
    void loop_pong();
    void loop_wrap();
    void loop_wrap_reverse();
    void play_move_sound();
    void stop_move_sound();
    Vector3 get_rot_delta();
    // Data
    Dictionary data_save();
    void data_load(Dictionary data);
    // State Management
    void state_physics(float delta);
    // Base Processing
    void _ready();
    void _physics_process(float delta);
};


/*******************************************************************************
FUNC_DOOR
Automatically opens when approached.
Don't use with doors that need to open together.
*******************************************************************************/
class FuncDoor : public FuncMove {
    GDCLASS(FuncDoor, FuncMove)
protected:
    static void _bind_methods();
public:
    Area3D* trigger_area;
    FuncDoor();
    ~FuncDoor();
    void update_children();
    void use(Node* activator);
    void mv_reverse(Node* ent = nullptr);
    void state_physics(float delta);
    void _ready();
};