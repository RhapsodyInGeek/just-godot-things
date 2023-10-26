/*******************************************************************************
TRIGGER: COUNTER
Whenever this trigger is triggered by other entities, it increments a counter.
When that counter reaches the specified amount, it triggers its target.
Can optionally repeat the counter.
*******************************************************************************/
class TriggerCounter : public Node {
    GDCLASS(TriggerCounter, Node)
protected:
    static void _bind_methods();
public:
    // Trigger Properties
    GameManager* GAME;
    enum { ST_READY, ST_DELAYED, ST_TRIGGERED, ST_REMOVED };
    Dictionary properties = {};
    uint8_t appearance_flags = 0;
    uint32_t flags = TRIGGER_FLAG_PLAYER_ONLY;
    uint8_t trigger_state = ST_READY;
    String targetname = "", target = "", targetfunc = "", message = "";
    float delay = 0.0f, timeout = 0.0f;
    Node* last_activator = nullptr;
    Callable trigger_callable;
    // Counter Properties
    int16_t count = 1, count_ct = 0, repeat = 0;
    String count_message = "";
    bool count_up = false;

    TriggerCounter();
    ~TriggerCounter();
    // Trigger Properties
    void set_properties(Dictionary p); Dictionary get_properties();
    void set_appearance_flags(int f); int get_appearance_flags();
    void set_flags(int f); int get_flags();
    void set_targetname(String t); String get_targetname();
    void set_target(String t); String get_target();
    void set_targetfunc(String f); String get_targetfunc();
    void set_message(String m); String get_message();
    void set_delay(float t); float get_delay();
    void set_wait(float t); float get_wait();    
    // Counter Properties
    void set_count(int c);
    int get_count();
    void set_repeat(int r);
    int get_repeat();
    void set_count_message(String m);
    String get_count_message();
    // Methods
    void use(Node* activator);
    void _use2();
    Node* get_last_activator();
    // Data
    Dictionary data_save();
    void data_load(Dictionary data);
    // Base Processing
    void _ready();
    void _process(float delta);
};