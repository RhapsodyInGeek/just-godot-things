/************************************************
TRIGGER BASE CLASS
************************************************/
class Trigger : public Area3D {
    GDCLASS(Trigger, Area3D)
protected:
    static void _bind_methods();
public:
    GameManager* GAME;
    enum { ST_READY, ST_DELAYED, ST_TRIGGERED, ST_REMOVED };
    Dictionary properties = {};
    uint8_t appearance_flags = 0;
    uint32_t flags = TRIGGER_FLAG_PLAYER_ONLY;
    uint8_t trigger_state = ST_READY;
    int16_t health = 0, health_max = 0;
    uint8_t bleed_type = 0;
    String targetname = "", target = "", targetfunc = "", message = "";
    float delay = 0.0f, wait = -1.0f, timeout = 0.0f;
    Node* last_activator = nullptr;
    Callable trigger_callable;

    // Properties
    void set_properties(Dictionary p); Dictionary get_properties(); void update_properties();
    void set_appearance_flags(int f); int get_appearance_flags();
    void set_flags(int f); int get_flags(); void apply_flags();
    void set_targetname(String t); String get_targetname();
    void set_target(String t); String get_target();
    void set_targetfunc(String f); String get_targetfunc();
    void set_message(String m); String get_message();
    void set_delay(float t); float get_delay();
    void set_wait(float t); float get_wait();
    void set_health(int h); int get_health();
    void set_health_max(int h); int get_health_max();
    void set_bleed_type(int b); int get_bleed_type();
    void reparent_entity();
    // Methods
    void damage(Node* inflictor, Node* attacker, int amount);
    void bleed(Transform3D hit_xform, int amount);
    void toggle_collision(bool toggle);
    void _on_ent_entered(Node* ent);
    void use(Node* activator);
    void _use2();
    void reset_trigger();
    Node* get_last_activator();
    // Data
    Dictionary data_save();
    void data_load(Dictionary data);
    // Base Processing
    Trigger();
    ~Trigger();
    void _ready();
    void state_idle(float delta);
    void state_physics(float delta);
    void _process(float delta);
    void _physics_process(float delta);
};

/*******************************************************************************
TRIGGER: USE
A player can press the use key to trigger this.
*******************************************************************************/
class TriggerUse : public Trigger {
    GDCLASS(TriggerUse, Trigger)
protected:
    static void _bind_methods();
public:
    TriggerUse();
    ~TriggerUse();
    void reset_trigger();
};