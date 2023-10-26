#ifndef BKG_CONTROLSMANAGER_H
#define BKG_CONTROLSMANAGER_H

/********************************************************************************
CONTROLS MANAGER

Autoload singleton interface for user input. All input checks are requested
through this node.

To create a custom action map, you only need to modify the DEFAULT_ACTION_MAP
in ControlsManager.h and the ControlsManager will automatically generate the 
appropriate input actions at runtime. 
Make sure this class is the first in your autoloads.
********************************************************************************/
#include <string>
#include <unordered_map>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>

#define DEADZONE 0.333f

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

        const std::unordered_map<std::string, std::vector<int>> DEFAULT_ACTION_MAP = {
            {"move_forward",    { InputType::KEY,           KEY_W,              InputType::JOYAXIS,   JOY_AXIS_LEFT_Y }},
            {"move_backward",   { InputType::KEY,           KEY_S,              InputType::JOYAXIS,   JOY_AXIS_LEFT_Y }},
            {"move_right",      { InputType::KEY,           KEY_D,              InputType::JOYAXIS,   JOY_AXIS_LEFT_X }},
            {"move_left",       { InputType::KEY,           KEY_A,              InputType::JOYAXIS,   JOY_AXIS_LEFT_X }},
            {"jump",            { InputType::KEY,           KEY_SPACE,          InputType::JOYBUTTON, JOY_BUTTON_A }},
            {"walk",            { InputType::KEY,           KEY_SHIFT,          InputType::KEY,       KEY_SHIFT }},
            {"crouch",          { InputType::KEY,           KEY_CTRL,           InputType::JOYBUTTON, JOY_BUTTON_B }},
            {"use",             { InputType::KEY,           KEY_E,              InputType::JOYBUTTON, JOY_BUTTON_X }},
            {"attack",          { InputType::MOUSEBUTTON,   MOUSE_BUTTON_LEFT,  InputType::JOYAXIS,   JOY_AXIS_TRIGGER_RIGHT }},
            {"alt_attack",      { InputType::MOUSEBUTTON,   MOUSE_BUTTON_RIGHT, InputType::JOYAXIS,   JOY_AXIS_TRIGGER_LEFT }},
            {"torch",           { InputType::KEY,           KEY_F,              InputType::JOYBUTTON, JOY_BUTTON_Y }},
            {"menu",            { InputType::KEY,           KEY_ESCAPE,         InputType::JOYBUTTON, JOY_BUTTON_BACK }},
            {"objective",       { InputType::KEY,           KEY_TAB,            InputType::JOYBUTTON, JOY_BUTTON_LEFT_STICK }},
            {"console",         { InputType::KEY,           KEY_QUOTELEFT,      InputType::KEY,       KEY_QUOTELEFT }},
            {"weapon_wheel",    { InputType::KEY,           KEY_Q,              InputType::JOYBUTTON, JOY_BUTTON_RIGHT_SHOULDER }},
            {"weapon_1",        { InputType::KEY,           KEY_1,              InputType::KEY,       KEY_1}},
            {"weapon_2",        { InputType::KEY,           KEY_2,              InputType::KEY,       KEY_2}},
            {"weapon_3",        { InputType::KEY,           KEY_3,              InputType::KEY,       KEY_3}},
            {"weapon_4",        { InputType::KEY,           KEY_4,              InputType::KEY,       KEY_4}},
            {"weapon_5",        { InputType::KEY,           KEY_5,              InputType::KEY,       KEY_5}},
            {"weapon_6",        { InputType::KEY,           KEY_6,              InputType::KEY,       KEY_6}},
            {"weapon_7",        { InputType::KEY,           KEY_7,              InputType::KEY,       KEY_7}},
            {"weapon_8",        { InputType::KEY,           KEY_8,              InputType::KEY,       KEY_8}},
            {"weapon_9",        { InputType::KEY,           KEY_9,              InputType::KEY,       KEY_9}},
            {"weapon_10",       { InputType::KEY,           KEY_0,              InputType::KEY,       KEY_0 }},
            {"quicksave",       { InputType::KEY,           KEY_F5,             InputType::KEY,       KEY_F5 }},
            {"quickload",       { InputType::KEY,           KEY_F9,             InputType::KEY,       KEY_F9 }}
        };

        std::unordered_map<std::string, std::vector<int>> action_map = {};
        std::vector<String> action_list = {};

        Dictionary map = {};
        int8_t input_mode = 0;
        Vector2 mouse_invert = Vector2(1.0f,1.0f);
        Vector2 gamepad_invert = Vector2(1.0f,1.0f);
        float lockout = 0.0f;
        bool remap_mode = false;
        bool console_mode = false;
        String remap_action;
        Vector2 mouse_motion = Vector2();
        Vector2 mouse_sensitivity = Vector2(0.5f, 0.5f);
        Vector2 move_motion = Vector2();
        std::unordered_map<std::string, float> held_time = {};

        ControlsManager();
        ~ControlsManager();
        // PROPERTIES
        void set_mouse_sensitivity(Vector2 new_sensitivity = Vector2(0.5f, 0.5f)); Vector2 get_mouse_sensitivity();
        void set_mouse_motion(Vector2 new_mouse_motion); Vector2 get_mouse_motion();
        void set_move_motion(Vector2 new_move_motion); Vector2 get_move_motion();
        void set_mouse_invert(Vector2 new_invert = Vector2(1.0f,1.0f)); Vector2 get_mouse_invert();
        void set_gamepad_invert(Vector2 new_invert = Vector2(1.0f,1.0f)); Vector2 get_gamepad_invert();
        void set_lockout(float new_lockout); float get_lockout(); bool locked();

        // INPUT MODE DETECTION
        void _gamepad_connected(int device_id, bool is_connected);
        void input_mode_swap(InputEvent* event);

        // MAPPING
        void set_action_map();
        void reset_to_defaults();
        void set_remap_mode(String new_remap_action, bool new_remap_mode = true);
        bool remap(InputEvent* event, String action);
        void dict_to_map(int mode, Dictionary new_map);
        Dictionary map_to_dict(int mode);
        String action_to_ui(String action, int mode = -1);

        // INPUT STATES
        bool pressed(String action);
        bool released(String action);
        bool held(String action);
        void update_held_time(float delta);
        float get_held_time(String action);

        // LOCKOUT
        void release_all();
        void mouse_lock(bool locked);

        // BASE PROCESSING
        void ready();
        void process(float delta);
        void input(InputEvent* event);
        void _notification(int _notif);
    };
};

#endif // !BKG_CONTROLSMANAGER_H
