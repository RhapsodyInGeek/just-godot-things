#include "GameDefs.h"
/*******************************************************************************
TRIGGER: COUNTER

Whenever this trigger is triggered by other entities, it increments a counter.
When that counter reaches the specified amount, it triggers its target.
Can optionally repeat the counter.
*******************************************************************************/

void TriggerCounter::_bind_methods()
{
    // PROPERTIES
    // properties
    ClassDB::bind_method(D_METHOD("set_properties", "_properties"), &TriggerCounter::set_properties);
    ClassDB::bind_method(D_METHOD("get_properties"), &TriggerCounter::get_properties);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");
    // appearance_flags
    ClassDB::bind_method(D_METHOD("set_appearance_flags", "_appearance_flags"), &TriggerCounter::set_appearance_flags);
    ClassDB::bind_method(D_METHOD("get_appearance_flags"), &TriggerCounter::get_appearance_flags);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "appearance_flags", PROPERTY_HINT_FLAGS,
        APPEARANCE_FLAG_LIST), "set_appearance_flags", "get_appearance_flags");
    // flags
    ClassDB::bind_method(D_METHOD("set_flags", "_flags"), &TriggerCounter::set_flags);
    ClassDB::bind_method(D_METHOD("get_flags"), &TriggerCounter::get_flags);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "flags", PROPERTY_HINT_FLAGS,
        TRIGGER_FLAG_LIST), "set_flags", "get_flags");
    // targetname
    ClassDB::bind_method(D_METHOD("set_targetname", "targetname"), &TriggerCounter::set_targetname);
    ClassDB::bind_method(D_METHOD("get_targetname"), &TriggerCounter::get_targetname);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "targetname"), "set_targetname", "get_targetname");
    // target
    ClassDB::bind_method(D_METHOD("set_target", "_target"), &TriggerCounter::set_target);
    ClassDB::bind_method(D_METHOD("get_target"), &TriggerCounter::get_target);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "target"), "set_target", "get_target");
    // targetfunc
    ClassDB::bind_method(D_METHOD("set_targetfunc", "_targetfunc"), &TriggerCounter::set_targetfunc);
    ClassDB::bind_method(D_METHOD("get_targetfunc"), &TriggerCounter::get_targetfunc);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "targetfunc"), "set_targetfunc", "get_targetfunc");
    // message
    ClassDB::bind_method(D_METHOD("set_message", "_message"), &TriggerCounter::set_message);
    ClassDB::bind_method(D_METHOD("get_message"), &TriggerCounter::get_message);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "message", PROPERTY_HINT_MULTILINE_TEXT), "set_message", "get_message");
    // delay
    ClassDB::bind_method(D_METHOD("set_delay", "_delay"), &TriggerCounter::set_delay);
    ClassDB::bind_method(D_METHOD("get_delay"), &TriggerCounter::get_delay);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "delay"), "set_delay", "get_delay");
    // count
    ClassDB::bind_method(D_METHOD("set_count", "_count"), &TriggerCounter::set_count);
    ClassDB::bind_method(D_METHOD("get_count"), &TriggerCounter::get_count);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "count"), "set_count", "get_count");
    // repeat
    ClassDB::bind_method(D_METHOD("set_repeat", "_repeat"), &TriggerCounter::set_repeat);
    ClassDB::bind_method(D_METHOD("get_repeat"), &TriggerCounter::get_repeat);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "repeat"), "set_repeat", "get_repeat");
    // message_count
    ClassDB::bind_method(D_METHOD("set_count_message", "_count_message"), &TriggerCounter::set_count_message);
    ClassDB::bind_method(D_METHOD("get_count_message"), &TriggerCounter::get_count_message);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "count_message", PROPERTY_HINT_MULTILINE_TEXT), "set_count_message", "get_count_message");
    
    ClassDB::bind_method(D_METHOD("use", "activator"), &TriggerCounter::use);
    ClassDB::bind_method(D_METHOD("_use2"), &TriggerCounter::_use2);
    ClassDB::bind_method(D_METHOD("get_last_activator"), &TriggerCounter::get_last_activator);
    ClassDB::bind_method(D_METHOD("data_save"), &TriggerCounter::data_save);
    ClassDB::bind_method(D_METHOD("data_load", "data"), &TriggerCounter::data_load);
}

void TriggerCounter::set_properties(Dictionary p) {
    properties = p;
    if (!Engine::get_singleton()->is_editor_hint())
        return;
    // Trigger
    if (properties.has("appearance_flags"))
        appearance_flags = (uint8_t)(int)properties["appearance_flags"];
    if (properties.has("flags"))
        flags = (uint32_t)(int)properties["flags"];
    if (properties.has("targetname"))
        targetname = (String)properties["targetname"];
    if (properties.has("target"))
        target = (String)properties["target"];
    if (properties.has("targetfunc"))
        targetfunc = (String)properties["targetfunc"];
    if (properties.has("message"))
        message = String(properties["message"]).replace("  ", " ");
    if (properties.has("delay"))
        delay = (float)properties["delay"];
    // Counter
    if (properties.has("count"))
        count = (int16_t)(int)properties["count"];
    if (properties.has("repeat"))
        repeat = (int16_t)(int)properties["repeat"];
    if (properties.has("count_message"))
        count_message = String(properties["count_message"]).replace("  ", " ");
    if (properties.has("count_up"))
        count_up = (bool)properties["count_up"];
}

Dictionary TriggerCounter::get_properties() { return properties; }

// Trigger Properties
void TriggerCounter::set_appearance_flags(int f) { appearance_flags = (uint8_t)f; } int TriggerCounter::get_appearance_flags() { return (int)appearance_flags; }
void TriggerCounter::set_flags(int f) { flags = (uint32_t)f; } int TriggerCounter::get_flags() { return (int)flags; }
void TriggerCounter::set_targetname(String t) { targetname = t; } String TriggerCounter::get_targetname() { return targetname; }
void TriggerCounter::set_target(String t) { target = t; } String TriggerCounter::get_target() { return target; }
void TriggerCounter::set_targetfunc(String f) { targetfunc = f; } String TriggerCounter::get_targetfunc() { return targetfunc; }
void TriggerCounter::set_message(String m) { message = m; } String TriggerCounter::get_message() { return message; }
void TriggerCounter::set_delay(float t) { delay = t; } float TriggerCounter::get_delay() { return delay; }

// Counter Properties
void TriggerCounter::set_count(int c) { count = c; }
int TriggerCounter::get_count() { return count; }
void TriggerCounter::set_repeat(int r) { repeat = r; }
int TriggerCounter::get_repeat() { return repeat; }
void TriggerCounter::set_count_message(String m) { count_message = m; }
String TriggerCounter::get_count_message() { return count_message; }

void TriggerCounter::use(Node* activator) {
    if (trigger_state == ST_READY && count_ct > -1) {
        count_ct++;
        if (count_ct >= count) {
            last_activator = activator;
            if (delay > 0) {
                trigger_state = ST_DELAYED;
                timeout = delay;
            }
            else
                _use2();
        }
        else if (count_message != "") {
            GAME->snd_msg_hint();
            Array curr_cnt = Array::make();
            if (count_up)
                curr_cnt.append(String::num(count_ct));
            else
                curr_cnt.append(String::num(count - count_ct));
            GAME->message(count_message, curr_cnt);
        }
    }
}

void TriggerCounter::_use2() {
    if (count_ct >= count) {
        trigger_state = ST_TRIGGERED;
        if (target.find("ACTIVATOR") != -1 && last_activator != nullptr) // target keyword, allows us to specifically trigger the one that activated us
            last_activator->add_to_group("ACTIVATOR");
        GAME->use_targets(this, target);
        last_activator->call_deferred("remove_from_group", "ACTIVATOR");

        if (repeat > 0)
            repeat--;
        if (repeat != 0) {
            count_ct = 0;
            trigger_state = ST_READY;
        }
        else
            count_ct = -1;
    }
}

Node* TriggerCounter::get_last_activator() {
    return last_activator;
}

Dictionary TriggerCounter::data_save() {
    Dictionary data = Dictionary();
    data["trigger_state"] = trigger_state;
    data["timeout"] = timeout;
    data["last_activator"] = last_activator;
    data["count_ct"] = count_ct;
    data["repeat"] = repeat;
    return data;
}

void TriggerCounter::data_load(Dictionary data) {
    trigger_state = (uint8_t)(int)data["trigger_state"];
    timeout = data["timeout"];
    last_activator = get_node_or_null((NodePath)data["last_activator"]);
    count_ct = (int16_t)(int)data["count_ct"];
    repeat = (int16_t)(int)data["repeat"];
}

TriggerCounter::TriggerCounter() {
    add_to_group("TRIGGER");
    add_to_group("SAV");
}

TriggerCounter::~TriggerCounter() {}

void TriggerCounter::_ready() {
    if (!Engine::get_singleton()->is_editor_hint()) {
        GAME = get_node<GameManager>(GAME_PATH);
        if (GAME->appearance_check(appearance_flags)) {
            queue_free();
            return;
        }
        call("apply_flags");
        GAME->set_targetname(this, targetname);
        if (flags & TRIGGER_FLAG_ON_START)
            call_deferred("_use2");
    }
}

void TriggerCounter::_process(float delta) {
    if (trigger_state == ST_DELAYED) {
        if (timeout > 0.0f)
            timeout -= GAME->dt_idle;
        else
            _use2();
    }
}
