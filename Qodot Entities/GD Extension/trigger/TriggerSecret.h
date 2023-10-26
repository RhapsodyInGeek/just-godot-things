/*******************************************************************************
TRIGGER SECRET
*******************************************************************************/
class TriggerSecret : public Trigger {
    GDCLASS(TriggerSecret, Trigger)
protected:
    static void _bind_methods();
public:
    bool secret_found = false;
    TriggerSecret();
    ~TriggerSecret();
    bool is_secret_found();
    void _use2();
    Dictionary data_save();
    void data_load(Dictionary data);
};