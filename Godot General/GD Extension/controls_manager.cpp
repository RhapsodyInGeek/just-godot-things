/***************************************************************************

CONTROLS MANAGER

Autoload singleton interface for user input. All input checks are requested
through this node.

***************************************************************************/
#include "controls_manager.h"
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input_event_joypad_button.hpp>
#include <godot_cpp/classes/input_event_joypad_motion.hpp>

using namespace godot;

void ControlsManager::_bind_methods() {
    // PROPERTIES
    // mouse sensitivity
    ClassDB::bind_method(D_METHOD("set_mouse_sensitivity", "new_sensitivity"), &ControlsManager::set_mouse_sensitivity, DEFVAL(Vector2(3.0f, 3.0f)));
    ClassDB::bind_method(D_METHOD("get_mouse_sensitivity"), &ControlsManager::get_mouse_sensitivity);
    ClassDB::add_property("ControlsManager", PropertyInfo(Variant::VECTOR2, "mouse_sensitivity"), "set_mouse_sensitivity", "get_mouse_sensitivity");
    // mouse motion
    ClassDB::bind_method(D_METHOD("set_mouse_motion", "new_mouse_motion"), &ControlsManager::set_mouse_motion);
    ClassDB::bind_method(D_METHOD("get_mouse_motion"), &ControlsManager::get_mouse_motion);
    ClassDB::add_property("ControlsManager", PropertyInfo(Variant::VECTOR2, "mouse_motion"), "set_mouse_motion", "get_mouse_motion");
    // move motion
    ClassDB::bind_method(D_METHOD("set_move_motion", "new_move_motion"), &ControlsManager::set_move_motion);
    ClassDB::bind_method(D_METHOD("get_move_motion"), &ControlsManager::get_move_motion);
    ClassDB::add_property("ControlsManager", PropertyInfo(Variant::VECTOR2, "move_motion"), "set_move_motion", "get_move_motion");
    // mouse invert
    ClassDB::bind_method(D_METHOD("set_mouse_invert", "new_invert"), &ControlsManager::set_mouse_invert, DEFVAL(Vector2(1.0f, 1.0f)));
    ClassDB::bind_method(D_METHOD("get_mouse_invert"), &ControlsManager::get_mouse_invert);
    ClassDB::add_property("ControlsManager", PropertyInfo(Variant::VECTOR2, "mouse_invert"), "set_mouse_invert", "get_mouse_invert");
    // gamepad sensitivity
    ClassDB::bind_method(D_METHOD("set_gamepad_sensitivity", "new_sensitivity"), &ControlsManager::set_gamepad_sensitivity, DEFVAL(Vector2(3.0f, 3.0f)));
    ClassDB::bind_method(D_METHOD("get_gamepad_sensitivity"), &ControlsManager::get_gamepad_sensitivity);
    ClassDB::add_property("ControlsManager", PropertyInfo(Variant::VECTOR2, "gamepad_sensitivity"), "set_gamepad_sensitivity", "get_gamepad_sensitivity");
    // gamepad invert
    ClassDB::bind_method(D_METHOD("set_gamepad_invert", "new_invert"), &ControlsManager::set_gamepad_invert, DEFVAL(Vector2(1.0f, 1.0f)));
    ClassDB::bind_method(D_METHOD("get_gamepad_invert"), &ControlsManager::get_gamepad_invert);
    ClassDB::add_property("ControlsManager", PropertyInfo(Variant::VECTOR2, "gamepad_invert"), "set_gamepad_invert", "get_gamepad_invert");
    // lockout
    ClassDB::bind_method(D_METHOD("set_lockout", "new_lockout"), &ControlsManager::set_lockout);
    ClassDB::bind_method(D_METHOD("get_lockout"), &ControlsManager::get_lockout);
    ClassDB::add_property("ControlsManager", PropertyInfo(Variant::FLOAT, "lockout"), "set_lockout", "get_lockout");
    ClassDB::bind_method(D_METHOD("locked"), &ControlsManager::locked);

    // INPUT MODE DETECTION
    ClassDB::bind_method(D_METHOD("_gamepad_connected"), &ControlsManager::_gamepad_connected);

    // INPUT STATES
    ClassDB::bind_method(D_METHOD("pressed", "action"), &ControlsManager::pressed);
    ClassDB::bind_method(D_METHOD("released", "action"), &ControlsManager::released);
    ClassDB::bind_method(D_METHOD("held", "action"), &ControlsManager::held);
    ClassDB::bind_method(D_METHOD("get_held_time", "action"), &ControlsManager::get_held_time);
    ClassDB::bind_method(D_METHOD("axis", "negative_action", "positive_action"), &ControlsManager::axis);
    ClassDB::bind_method(D_METHOD("release_all"), &ControlsManager::release_all);

    // LOCKOUT
    ClassDB::bind_method(D_METHOD("set_mouse_mode", "mouse_mode"), &ControlsManager::set_mouse_mode);
    ClassDB::bind_method(D_METHOD("get_mouse_mode"), &ControlsManager::get_mouse_mode);

    // SIGNALS
    ADD_SIGNAL(MethodInfo("console_input"));
    ADD_SIGNAL(MethodInfo("action_pressed", PropertyInfo(Variant::STRING, "action")));
    ADD_SIGNAL(MethodInfo("action_released", PropertyInfo(Variant::STRING, "action")));
    ADD_SIGNAL(MethodInfo("action_remapped", PropertyInfo(Variant::STRING, "action"), PropertyInfo(Variant::BOOL, "remap_successful")));

    // BASE PROCESSING
    ClassDB::bind_method(D_METHOD("input", "event"), &ControlsManager::input);
}

/*************************************************
PROPERTIES
*************************************************/
void ControlsManager::set_mouse_sensitivity(Vector2 new_sensitivity) {
    mouse_sensitivity.x = Math::clamp(new_sensitivity.x, 0.1f, 20.0f);
    mouse_sensitivity.y = Math::clamp(new_sensitivity.y, 0.1f, 20.0f);
}
Vector2 ControlsManager::get_mouse_sensitivity() { return mouse_sensitivity; }

void ControlsManager::set_mouse_motion(Vector2 new_mouse_motion) { mouse_motion = new_mouse_motion; }
Vector2 ControlsManager::get_mouse_motion() { return mouse_motion; }

void ControlsManager::set_move_motion(Vector2 new_move_motion) { move_motion = new_move_motion; }
Vector2 ControlsManager::get_move_motion() { return move_motion; }

void ControlsManager::set_mouse_invert(Vector2 new_invert) {
    if (new_invert.length() != 0)
        mouse_invert = new_invert.sign();
}
Vector2 ControlsManager::get_mouse_invert() { return mouse_invert; }

void ControlsManager::set_gamepad_sensitivity(Vector2 new_sensitivity) {
    gamepad_sensitivity.x = Math::clamp(new_sensitivity.x, 0.1f, 20.0f);
    gamepad_sensitivity.y = Math::clamp(new_sensitivity.y, 0.1f, 20.0f);
}
Vector2 ControlsManager::get_gamepad_sensitivity() { return gamepad_sensitivity; }

void ControlsManager::set_gamepad_invert(Vector2 new_invert) {
    if (new_invert.length() != 0)
        gamepad_invert = new_invert.sign();
}
Vector2 ControlsManager::get_gamepad_invert() { return gamepad_invert; }

void ControlsManager::set_lockout(float new_lockout) { lockout = new_lockout; }
float ControlsManager::get_lockout() { return lockout; }
bool ControlsManager::locked() { return lockout > 0.0f; }

/*************************************************
INPUT MODE DETECTION
*************************************************/
// Gamepad initialize
void ControlsManager::_gamepad_connected(int device_id, bool is_connected) {
    if (is_connected) {
        if (INPUT->is_joy_known(device_id)) {
            // Determine what kind of controller it is
            if (input_mode != InputMode::Keyboard) {
                String joy_name = INPUT->get_joy_name(device_id);
                if (joy_name.find("SNES") > -1)
                    input_mode = InputMode::SNES;
                else if (joy_name.find("PS4") > -1)
                    input_mode = InputMode::PSX;
                else
                    input_mode = InputMode::Xbox;
            }
        }
    }
}

// Detect current input method; used for UI display mostly
void ControlsManager::input_mode_swap(InputEvent* event) {
    if (input_mode == InputMode::Keyboard) {
        bool pad = false;
        if (event->get_class() == "InputEventJoypadButton")
            pad = true;
        // If we don't check the deadzone, it will always be putting out the JoypadMotion event
        if (event->get_class() == "InputEventJoypadMotion" && abs(float(event->call("get_axis_value"))) > DEADZONE)
            pad = true;
        if (pad) {
            String joy_name = INPUT->get_joy_name(0);
            if (joy_name.find("SNES") > -1)
                input_mode = InputMode::SNES;
            else if (joy_name.find("PS4") > -1)
                input_mode = InputMode::PSX;
            else
                input_mode = InputMode::Xbox;
        }
    }
    else if (event->get_class() == "InputEventKey" || event->get_class() == "InputEventMouseButton")
        input_mode = InputMode::Keyboard;
}

/*************************************************
MAPPING
*************************************************/
void ControlsManager::set_action_map() {
    InputMap* IM = InputMap::get_singleton();
    String action;
    int map_input[2];

    for (std::unordered_map<std::string, std::vector<int>>::iterator itr = action_map.begin(); itr != action_map.end(); itr++) {
        action = String(itr->first.c_str());
        IM->action_erase_events(action);

        for (int j = 0; j < 2; j++) {
            if (j == InputMode::Keyboard) {
                map_input[ACTION_TYPE] = itr->second[0]; // input event type
                map_input[ACTION_INPUT] = itr->second[1]; // input global constant

                if (map_input[ACTION_INPUT] == KEY_NONE)
                    continue;

                switch (map_input[ACTION_TYPE]) {
                case InputType::KEY: {
                    Ref<InputEventKey> event;
                    event.instantiate();
                    event->set_keycode(Key(map_input[ACTION_INPUT]));
                    IM->action_add_event(action, event);
                    break;
                }
                case InputType::MOUSEBUTTON: {
                    Ref<InputEventMouseButton> event;
                    event.instantiate();
                    event->set_button_index(MouseButton(map_input[ACTION_INPUT]));
                    IM->action_add_event(action, event);
                    break;
                }
                }
            }
            else { // Gamepad
                map_input[ACTION_TYPE] = itr->second[2]; // input event type
                map_input[ACTION_INPUT] = itr->second[3]; // input global constant

                if (map_input[ACTION_INPUT] == -1)
                    continue;

                switch (map_input[ACTION_TYPE]) {
                case InputType::JOYBUTTON: {
                    Ref<InputEventJoypadButton> event;
                    event.instantiate();
                    event->set_button_index(JoyButton(map_input[ACTION_INPUT]));
                    IM->action_add_event(action, event);
                    break;
                }
                case InputType::JOYAXIS: {
                    Ref<InputEventJoypadMotion> event;
                    event.instantiate();
                    event->set_axis(JoyAxis(map_input[ACTION_INPUT]));
                    event->set_axis_value((float)itr->second[ACTION_AXIS_VALUE]);
                    IM->action_add_event(action, event);
                    IM->action_set_deadzone(action, DEADZONE);
                }
                }
            }
        }
    }
}

void ControlsManager::reset_action_map_to_default() {
    action_map = DEFAULT_ACTION_MAP;
    set_action_map();
}

void ControlsManager::reset_keyboard_map_to_default() {
    std::unordered_map<std::string, std::vector<int>> defmap = DEFAULT_ACTION_MAP;
    for (std::unordered_map<std::string, std::vector<int>>::iterator itr = defmap.begin(); itr != defmap.end(); itr++) {
        action_map[itr->first][0] = itr->second[0];
        action_map[itr->first][1] = itr->second[1];
    }
    set_action_map();
}

void ControlsManager::reset_gamepad_map_to_default() {
    std::unordered_map<std::string, std::vector<int>> defmap = DEFAULT_ACTION_MAP;
    for (std::unordered_map<std::string, std::vector<int>>::iterator itr = defmap.begin(); itr != defmap.end(); itr++) {
        action_map[itr->first][2] = itr->second[2];
        action_map[itr->first][3] = itr->second[3];
    }
    set_action_map();
}

void ControlsManager::reset_to_defaults() {
    reset_action_map_to_default();
    set_mouse_sensitivity();
    set_mouse_invert();
    set_gamepad_sensitivity();
    set_gamepad_invert();
}

void ControlsManager::set_remap_mode(String new_remap_action, float new_remap_wait, bool new_remap_mode) {
    remap_action = new_remap_action;
    remap_wait = new_remap_wait;
    remap_mode = new_remap_mode;
}

bool ControlsManager::remap(InputEvent* event, String action) {
    String _class = event->get_class();

    // We don't accept mouse motion because it's more work than I feel like putting in right now, maybe later
    if (_class == "InputEventMouseMotion")
        return false;

    int new_input[3] = { -1, -1, 0 }, input_check[3] = { -1, -1, 0 };

    if (_class == "InputEventKey") {
        input_check[ACTION_INPUT] = event->call("get_keycode");
        if (input_check[ACTION_INPUT] != KEY_QUOTELEFT)
            if (input_check[ACTION_INPUT] < KEY_F1 || input_check[ACTION_INPUT] > KEY_F12) {
                new_input[ACTION_TYPE] = InputType::KEY;
                new_input[ACTION_INPUT] = input_check[ACTION_INPUT];
            }
    }
    else if (_class == "InputEventMouseButton") {
        new_input[ACTION_TYPE] = InputType::MOUSEBUTTON;
        new_input[ACTION_INPUT] = event->call("get_button_index");
    }
    else if (_class == "InputEventJoypadMotion") {
        new_input[ACTION_TYPE] = InputType::JOYAXIS;
        new_input[ACTION_INPUT] = event->call("get_axis");
        new_input[REMAP_AXIS_VALUE] = (int)Math::sign((float)event->call("get_axis_value"));
    }
    else if (_class == "InputEventJoypadButton") {
        input_check[ACTION_INPUT] = event->call("get_button_index");
        if (input_check[ACTION_INPUT] != JOY_BUTTON_GUIDE && input_check[1] != JOY_BUTTON_MISC1) {
            new_input[ACTION_TYPE] = InputType::JOYBUTTON;
            new_input[ACTION_INPUT] = input_check[ACTION_INPUT];
        }
    }

    if (new_input[ACTION_INPUT] >= 0) {
        // Grab the right map
        int in_ofs = 0;
        if (input_mode != InputMode::Keyboard)
            in_ofs = 2;

        std::string _action = action.utf8().get_data();

        // We need to make sure we don't have 2 actions assigned to the same input
        input_check[ACTION_TYPE] = action_map[_action][in_ofs];
        input_check[ACTION_INPUT] = action_map[_action][in_ofs + 1];
        input_check[REMAP_AXIS_VALUE] = action_map[_action][ACTION_AXIS_VALUE];
        for (std::unordered_map<std::string, std::vector<int>>::iterator itr = action_map.begin(); itr != action_map.end(); itr++) {
            if (itr->second[in_ofs + 1] == new_input[ACTION_INPUT] && itr->second[in_ofs] == new_input[ACTION_TYPE]) {
                if (input_check[ACTION_TYPE] != InputType::JOYAXIS || itr->second[ACTION_AXIS_VALUE] == new_input[REMAP_AXIS_VALUE]) {
                    action_map[itr->first][ACTION_TYPE + in_ofs] = input_check[0];
                    action_map[itr->first][ACTION_INPUT + in_ofs] = input_check[1];
                    if (input_check[ACTION_TYPE] == InputType::JOYAXIS)
                        action_map[itr->first][ACTION_AXIS_VALUE] = input_check[REMAP_AXIS_VALUE];
                    break;
                }
            }
        }
        
        // Setting up the new control
        action_map[_action][ACTION_TYPE + in_ofs] = new_input[ACTION_TYPE];
        action_map[_action][ACTION_INPUT + in_ofs] = new_input[ACTION_INPUT];
        if (new_input[ACTION_TYPE] == InputType::JOYAXIS)
            action_map[_action][ACTION_AXIS_VALUE] = new_input[REMAP_AXIS_VALUE];
        else
            action_map[_action][ACTION_AXIS_VALUE] = 0;
        
        // Remap Controls
        set_action_map();
        release_all();
        emit_signal("action_remapped", action, true);
        return true;
    }

    // We couldn't remap it?
    release_all();
    emit_signal("action_remapped", action, false);
    return false;
}

void ControlsManager::dict_to_map(Dictionary new_map) {
    Array keys = new_map.keys(), v;
    for (int i = 0; i < keys.size(); i++) {
        v = new_map[keys[i]];
        if (v.size() < 4) { // no unexpected crashes plz
            action_map = DEFAULT_ACTION_MAP;
            return;
        }
        action_map[String(keys[i]).utf8().get_data()] = {v[0], v[1], v[2], v[3]};
    }
}

Dictionary ControlsManager::map_to_dict() {
    Dictionary dict_map = {};
    for (std::unordered_map<std::string, std::vector<int>>::iterator itr = action_map.begin(); itr != action_map.end(); itr++)
        dict_map[String(itr->first.c_str())] = Array::make(itr->second[0], itr->second[1], itr->second[2], itr->second[3]);
    return dict_map;
}

String ControlsManager::action_to_ui(String action, int mode) {
    std::string _action = action.utf8().get_data();

    int ui_input[2] = { -1 };
    if (mode == InputMode::Keyboard) {
        ui_input[0] = action_map[_action][0];
        ui_input[1] = action_map[_action][1];
    }
    else { // Gamepad
        ui_input[0] = action_map[_action][2];
        ui_input[1] = action_map[_action][3];
    }

    if (ui_input[0] == InputType::KEY)
        return OS::get_singleton()->get_keycode_string(Key(ui_input[1]));

    return String::num(ui_input[0]) + ";" + String::num(ui_input[1]);
}

/*************************************************
INPUT STATES
*************************************************/
bool ControlsManager::pressed(String action) { return (lockout <= 0.0f && INPUT->is_action_just_pressed(action)); }

bool ControlsManager::released(String action) { return (lockout <= 0.0f && INPUT->is_action_just_released(action)); }

bool ControlsManager::held(String action) { return (lockout <= 0.0f && INPUT->is_action_pressed(action)); }

void ControlsManager::update_held_time(float delta) {
    std::string c;
    for (int i = 0; i < action_list.size(); i++) {
        c = action_list[i].utf8().get_data();
        if (held(action_list[i]))
            held_time[c] = float(held_time[c]) + delta;
        else
            held_time[c] = 0.0f;
    }
}

float ControlsManager::get_held_time(String action) {
    return held_time[action.utf8().get_data()];
}

float ControlsManager::axis(String negative_action, String positive_action) {
    if (lockout > 0.0f)
        return 0.0f;
    return INPUT->get_axis(negative_action, positive_action);
}

void ControlsManager::release_all() {
    for (int i = 0; i < action_list.size(); i++)
        INPUT->action_release(action_list[i]);
}

void ControlsManager::set_mouse_mode(int mouse_mode) {
    INPUT->set_mouse_mode(Input::MouseMode(mouse_mode));
}

int ControlsManager::get_mouse_mode() {
    return (int)INPUT->get_mouse_mode();
}

/*************************************************
BASE PROCESSING
*************************************************/
ControlsManager::ControlsManager() {
	set_process_mode(Node::PROCESS_MODE_ALWAYS);
    action_map = DEFAULT_ACTION_MAP;
    for (std::unordered_map<std::string, std::vector<int>>::iterator itr = action_map.begin(); itr != action_map.end(); itr++) {
        action_list.push_back(String(itr->first.c_str()));
        held_time[itr->first] = 0.0f;
    }
}

ControlsManager::~ControlsManager() {}

void ControlsManager::ready() {
    if (Engine::get_singleton()->is_editor_hint())
        return;

    INPUT = Input::get_singleton();
    INPUT->connect("joy_connection_changed", Callable(this, "_gamepad_connected"));
    
    InputMap* inmap = InputMap::get_singleton();
    for (int i = 0; i < action_list.size(); i++)
        inmap->add_action(action_list[i]);
    set_action_map();
}

void ControlsManager::process(float delta) {
    if (Engine::get_singleton()->is_editor_hint())
        return;

    if (INPUT->is_key_pressed(KEY_F4) && INPUT->is_key_pressed(KEY_ALT))
        get_tree()->quit();

    if (input_mode == InputMode::Keyboard)
        set_deferred("mouse_motion", Vector2());

    update_held_time(delta);

    if (lockout > 0) {
        release_all();
        lockout -= delta;
    }
}

void ControlsManager::input(InputEvent* event) {
    if (Engine::get_singleton()->is_editor_hint())
        return;

    // Dev console eats inputs
    if (console_mode) {
        if (event->get_class() == "InputEventKey")
            emit_signal("console_input", event);
        get_viewport()->set_input_as_handled();
        return;
    }

    // Input remapping eats inputs
    if (remap_mode) {
        // delay to make sure we don't immediately register a new input
        if (remap_wait > 0.0f)
            remap_wait -= get_process_delta_time();
        else if (remap(event, remap_action))
            remap_mode = false;
        get_viewport()->set_input_as_handled();
        return;
    }

    input_mode_swap(event);

    // Mouselook
    if (event->get_class() == "InputEventMouseMotion" && input_mode == InputMode::Keyboard)
        mouse_motion = Vector2(event->get("relative")) * 0.1f * mouse_sensitivity;
    
    // Input states
    for (int i = 0; i < action_list.size(); i++) {
        if (event->is_action_pressed(action_list[i])) {
            emit_signal("action_pressed", action_list[i]);
            break;
        }
        if (event->is_action_released(action_list[i])) {
            emit_signal("action_released", action_list[i]);
            break;
        }
    }
}

void ControlsManager::_notification(int _notif) {
    switch (_notif) {
    case NOTIFICATION_READY:
        ready();
        set_process(true);
        break;
    case NOTIFICATION_PROCESS:
        process(get_process_delta_time());
        break;
    }
}