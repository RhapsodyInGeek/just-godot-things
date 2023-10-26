/*******************************************************************************
TRIGGER HURT
Any Actors in this volume get beat up.
*******************************************************************************/
class TriggerHurt : public Trigger {
    GDCLASS(TriggerHurt, Trigger)
protected:
    static void _bind_methods();
public:
    int16_t damage = 10;
    bool can_gib = true;
    struct ent_timer { Actor* ent; float delay; };
    std::deque<ent_timer> ent_timers;

    TriggerHurt();
    ~TriggerHurt();
    // Properties
    void update_properties();
    void apply_flags();
    void set_damage(int d);
    int get_damage();
    void set_delay(float d);
    float get_delay();
    void set_can_gib(bool g);
    bool get_can_gib();
    // Methods
    void _on_ent_entered(Node* ent);
    void _on_ent_exited(Node* ent);
    void use(Node* ent);
    // Base Processing
    void state_physics(float delta);
};