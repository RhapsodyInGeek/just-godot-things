#ifndef BKG_SPRITETEXT_H
#define BKG_SPRITETEXT_H

/*********************************
STANDARD LIBRARIES
**********************************/
#include <string>
#include <vector>
/*********************************
GODOT BINDINGS
**********************************/
#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <godot_cpp/classes/texture.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/reference_rect.hpp>

namespace godot {
    /*******************************************************************************

    SPRITE FONT CLASS

    Font definition object instanced by SpriteText objects.
    Setting character size and character set will automatically cut the frames appropriately.

    *******************************************************************************/
    class SpriteFont : public Sprite2D {
        GDCLASS(SpriteFont, Sprite2D)
    protected:
        static void _bind_methods();
    public:
        Vector2 character_size = Vector2(8.0f, 8.0f);
        String character_set = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789., !? '\":()+-*/=@#$_";
        SpriteFont();
        ~SpriteFont();
        void set_character_size(Vector2 new_size);
        Vector2 get_character_size();
        void set_character_set(String new_character_set);
        String get_character_set();
    };

    /*******************************************************************************

    SPRITE TEXT CLASS

    ReferenceRect node that writes and displays text. It creates a child ReferenceRect
    "scroll" object that SpriteFont characters are then added and positioned to.

    *******************************************************************************/
#define MAX_CHARS 1024

    class SpriteText : public ReferenceRect {
        GDCLASS(SpriteText, ReferenceRect)
    protected:
        static void _bind_methods();
    public:
        // PROPERTIES
        bool text_update = false;
        ReferenceRect* scr = nullptr;
        // Font
        String font_path = String();
        Ref<PackedScene> font_res = nullptr;
        Vector2 font_scale = Vector2(1.0f, 1.0f), font_size = Vector2(8.0f, 8.0f);
        Color font_color = Color(1.0f, 1.0f, 1.0f, 1.0f);
        String font_shader = String();
        Ref<Material> font_shader_res;
        // Writing Setup
        String text = String();
        float write_speed = -1.0f, scroll_speed = 64.0f;
        enum ALIGN { LEFT, CENTER, RIGHT };
        int alignment = ALIGN::LEFT;
        bool word_wrap = true, auto_scroll = true;
        Vector2 text_margin = Vector2(0.0f, 2.0f);
        // Writing Handler
        int write_progress = 0, next_word_length = 0;
        float write_ct = 0.0f;
        Vector2 write_pos = Vector2();

        SpriteText();
        ~SpriteText();
        // PROPERTIES
        void set_text_update(bool is_test); bool get_text_update();
        // Font
        void set_font(String path); String get_font();
        bool set_font_res();
        void set_font_scale(Vector2 s); Vector2 get_font_scale();
        void set_font_color(Color new_font_color); Color get_font_color();
        void set_font_shader(String new_font_shader); String get_font_shader();
        bool set_font_shader_res();
        // Writing
        void set_text(String new_text);  String get_text();
        void set_write_speed(float new_write_speed); float get_write_speed();
        void set_alignment(int new_alignment); int get_alignment();
        void set_word_wrap(bool is_word_wrap);  bool get_word_wrap();
        void set_text_margin(Vector2 new_margin); Vector2 get_text_margin();
        void set_auto_scroll(bool is_auto_scroll); bool get_auto_scroll();
        void set_scroll_speed(float new_scroll_speed); float get_scroll_speed();

        // METHODS
        void add_scroll_box();
        // Write
        void clear();
        void write(String new_text, bool clear_text = true);
        void write_loop(float delta);
        void write_char(int i);
        // Base Processing
        void ready();
        void process(float delta);
        void _notification(int _notif);
    };
}

#endif // !BKG_SPRITETEXT_H