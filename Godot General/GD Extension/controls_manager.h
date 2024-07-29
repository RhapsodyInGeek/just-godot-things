#ifndef BKG_CONTROLSMANAGER_H
#define BKG_CONTROLSMANAGER_H

/********************************************************************************
CONTROLS MANAGER
********************************************************************************/
#include <string>
#include <unordered_map>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>

#define DEADZONE 0.333f
#define MAX_INPUT_TYPES 2

namespace godot
{
    class ControlsManager : public Node
    {
        GDCLASS(ControlsManager, Node)
    private:
        static void _bind_methods();
    public:
        enum InputMode { Keyboard, Xbox, PSX, SNES };

        Input* INPUT = nullptr;

        enum InputType { KEY, MOUSEBUTTON, MOUSEAXIS, JOYBUTTON, JOYAXIS };
        enum ActionMapElement { ACTION_TYPE, ACTION_INPUT, REMAP_AXIS_VALUE, ACTION_AXIS_VALUE = 4 };

        const std::unordered_map<std::string, std::vector<int>> DEFAULT_ACTION_MAP = {
            {"move_forward",    { InputType::KEY,           KEY_W,              InputType::JOYAXIS,   JOY_AXIS_LEFT_Y, -1 }},
            {"move_backward",   { InputType::KEY,           KEY_S,              InputType::JOYAXIS,   JOY_AXIS_LEFT_Y, 1 }},
            {"move_right",      { InputType::KEY,           KEY_D,              InputType::JOYAXIS,   JOY_AXIS_LEFT_X, 1 }},
            {"move_left",       { InputType::KEY,           KEY_A,              InputType::JOYAXIS,   JOY_AXIS_LEFT_X, -1 }},
            {"aim_up",          { InputType::KEY,           KEY_NONE,           InputType::JOYAXIS,   JOY_AXIS_RIGHT_Y, -1 }},
            {"aim_down",        { InputType::KEY,           KEY_NONE,           InputType::JOYAXIS,   JOY_AXIS_RIGHT_Y, 1 }},
            {"aim_right",       { InputType::KEY,           KEY_NONE,           InputType::JOYAXIS,   JOY_AXIS_RIGHT_X, 1 }},
            {"aim_left",        { InputType::KEY,           KEY_NONE,           InputType::JOYAXIS,   JOY_AXIS_RIGHT_X, -1 }},
            {"jump",            { InputType::KEY,           KEY_SPACE,          InputType::JOYBUTTON, JOY_BUTTON_A, 0 }},
            {"walk",            { InputType::KEY,           KEY_SHIFT,          InputType::KEY,       KEY_SHIFT, 0 }},
            {"crouch",          { InputType::KEY,           KEY_CTRL,           InputType::JOYBUTTON, JOY_BUTTON_B, 0 }},
            {"use",             { InputType::KEY,           KEY_E,              InputType::JOYBUTTON, JOY_BUTTON_X, 0 }},
            {"attack",          { InputType::MOUSEBUTTON,   MOUSE_BUTTON_LEFT,  InputType::JOYAXIS,   JOY_AXIS_TRIGGER_RIGHT, 0 }},
            {"alt_attack",      { InputType::MOUSEBUTTON,   MOUSE_BUTTON_RIGHT, InputType::JOYAXIS,   JOY_AXIS_TRIGGER_LEFT, 0 }},
            {"torch",           { InputType::KEY,           KEY_F,              InputType::JOYBUTTON, JOY_BUTTON_Y, 0 }},
            {"menu",            { InputType::KEY,           KEY_ESCAPE,         InputType::JOYBUTTON, JOY_BUTTON_START, 0 }},
            {"objective",       { InputType::KEY,           KEY_TAB,            InputType::JOYBUTTON, JOY_BUTTON_BACK, 0 }},
            {"console",         { InputType::KEY,           KEY_QUOTELEFT,      InputType::KEY,       KEY_QUOTELEFT, 0 }},
            {"command_enter",   { InputType::KEY,           KEY_ENTER,          InputType::KEY,       KEY_ENTER, 0 }},
            {"weapon_wheel",    { InputType::KEY,           KEY_Q,              InputType::JOYBUTTON, JOY_BUTTON_RIGHT_SHOULDER, 0 }},
            {"weapon_1",        { InputType::KEY,           KEY_1,              InputType::KEY,       KEY_1, 0}},
            {"weapon_2",        { InputType::KEY,           KEY_2,              InputType::KEY,       KEY_2, 0}},
            {"weapon_3",        { InputType::KEY,           KEY_3,              InputType::KEY,       KEY_3, 0}},
            {"weapon_4",        { InputType::KEY,           KEY_4,              InputType::KEY,       KEY_4, 0}},
            {"weapon_5",        { InputType::KEY,           KEY_5,              InputType::KEY,       KEY_5, 0}},
            {"weapon_6",        { InputType::KEY,           KEY_6,              InputType::KEY,       KEY_6, 0}},
            {"weapon_7",        { InputType::KEY,           KEY_7,              InputType::KEY,       KEY_7, 0}},
            {"weapon_8",        { InputType::KEY,           KEY_8,              InputType::KEY,       KEY_8, 0}},
            {"weapon_9",        { InputType::KEY,           KEY_9,              InputType::KEY,       KEY_9, 0}},
            {"weapon_10",       { InputType::KEY,           KEY_0,              InputType::KEY,       KEY_0, 0 }},
            {"quicksave",       { InputType::KEY,           KEY_F5,             InputType::KEY,       KEY_F5, 0 }},
            {"quickload",       { InputType::KEY,           KEY_F9,             InputType::KEY,       KEY_F9, 0 }}
        };

        std::unordered_map<std::string, std::vector<int>> action_map = {};
        std::vector<String> action_list = {};

        Dictionary map = {};
        int8_t input_mode = 0;
        Vector2 mouse_motion = Vector2();
        Vector2 mouse_sensitivity = Vector2(3.0f, 3.0f);
        Vector2 mouse_invert = Vector2(1.0f,1.0f);
        Vector2 gamepad_sensitivity = Vector2(3.0f, 3.0f);
        Vector2 gamepad_invert = Vector2(1.0f,1.0f);
        Vector2 move_motion = Vector2();
        float lockout = 0.0f;
        bool remap_mode = false;
        float remap_wait = 0.0f;
        bool console_mode = false;
        String remap_action;
        std::unordered_map<std::string, float> held_time = {};

        ControlsManager();
        ~ControlsManager();
        // PROPERTIES
        void set_mouse_sensitivity(Vector2 new_sensitivity = Vector2(3.0f, 3.0f)); Vector2 get_mouse_sensitivity();
        void set_mouse_motion(Vector2 new_mouse_motion); Vector2 get_mouse_motion();
        void set_move_motion(Vector2 new_move_motion); Vector2 get_move_motion();
        void set_mouse_invert(Vector2 new_invert = Vector2(1.0f,1.0f)); Vector2 get_mouse_invert();
        void set_gamepad_sensitivity(Vector2 new_sensitivity = Vector2(3.0f, 3.0f)); Vector2 get_gamepad_sensitivity();
        void set_gamepad_invert(Vector2 new_invert = Vector2(1.0f,1.0f)); Vector2 get_gamepad_invert();
        void set_lockout(float new_lockout); float get_lockout(); bool locked();

        // INPUT MODE DETECTION
        void _gamepad_connected(int device_id, bool is_connected);
        void input_mode_swap(InputEvent* event);

        // MAPPING
        void set_action_map();
        void reset_action_map_to_default();
        void reset_keyboard_map_to_default();
        void reset_gamepad_map_to_default();
        void reset_to_defaults();
        void set_remap_mode(String new_remap_action, float new_remap_wait = 0.1f, bool new_remap_mode = true);
        bool remap(InputEvent* event, String action);
        void dict_to_map(Dictionary new_map);
        Dictionary map_to_dict();
        String action_to_ui(String action, int mode = ControlsManager::Keyboard);

        // INPUT STATES
        bool pressed(String action);
        bool released(String action);
        bool held(String action);
        void update_held_time(float delta);
        float get_held_time(String action);
        float axis(String negative_action, String positive_action);

        // LOCKOUT
        void release_all();
        void set_mouse_mode(int mouse_mode);
        int get_mouse_mode();

        // BASE PROCESSING
        void ready();
        void process(float delta);
        void input(InputEvent* event);
        void _notification(int _notif);
    };
};

#endif // !BKG_CONTROLSMANAGER_H
