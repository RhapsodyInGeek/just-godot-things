#include "GameDefs.h"
/*******************************************************************************
TRIGGER SECRET
*******************************************************************************/
void TriggerSecret::_bind_methods() {
    ClassDB::bind_method(D_METHOD("is_secret_found"), &TriggerSecret::is_secret_found);
    ClassDB::bind_method(D_METHOD("_use2"), &TriggerSecret::_use2);
    ClassDB::bind_method(D_METHOD("data_save"), &TriggerSecret::data_save);
    ClassDB::bind_method(D_METHOD("data_load", "data"), &TriggerSecret::data_load);
}

bool TriggerSecret::is_secret_found() { return secret_found; }

void TriggerSecret::_use2() {
    if (!secret_found) {
        trigger_state = ST_TRIGGERED;
        secret_found = true;
        GAME->snd_notif_secret();
        if (message == "")
            message = "$secretfound";
        GAME->message(message);
        toggle_collision(false);
    }
}

Dictionary TriggerSecret::data_save() {
    Dictionary data = Trigger::data_save();
    data["secret_found"] = secret_found;
    return data;
}

void TriggerSecret::data_load(Dictionary data) {
    Trigger::data_load(data);
    secret_found = data["secret_found"];
    if (secret_found)
        toggle_collision(false);
}

TriggerSecret::TriggerSecret() { add_to_group("SECRET"); }
TriggerSecret::~TriggerSecret() {}