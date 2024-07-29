#include "sprite_text.h"
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

/*******************************************************************************

SPRITE FONT CLASS

Font definition object instanced by SpriteText objects.
Setting character size and character set will automatically cut the frames appropriately.

*******************************************************************************/
void SpriteFont::_bind_methods() {
    // character_size
    ClassDB::bind_method(D_METHOD("set_character_size", "_size"), &SpriteFont::set_character_size);
    ClassDB::bind_method(D_METHOD("get_character_size"), &SpriteFont::get_character_size);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "character_size"), "set_character_size", "get_character_size");
    // character set
    ClassDB::bind_method(D_METHOD("set_character_set", "_character_set"), &SpriteFont::set_character_set);
    ClassDB::bind_method(D_METHOD("get_character_set"), &SpriteFont::get_character_set);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "character_set", PROPERTY_HINT_MULTILINE_TEXT), "set_character_set", "get_character_set");
}

void SpriteFont::set_character_size(Vector2 new_character_size) { character_size = new_character_size; }
Vector2 SpriteFont::get_character_size() { return character_size; }
void SpriteFont::set_character_set(String new_character_set) { character_set = new_character_set; }
String SpriteFont::get_character_set() { return character_set; }

SpriteFont::SpriteFont() {}
SpriteFont::~SpriteFont() {}

/*******************************************************************************

SPRITE TEXT CLASS

ReferenceRect node that writes and displays text. It creates a child ReferenceRect
"scroll" object that SpriteFont characters are then added and positioned to.

*******************************************************************************/
void SpriteText::_bind_methods() {
    // PROPERTIES
    // text update
    ClassDB::bind_method(D_METHOD("set_text_update", "_text_update"), &SpriteText::set_text_update);
    ClassDB::bind_method(D_METHOD("get_text_update"), &SpriteText::get_text_update);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "text_update"), "set_text_update", "get_text_update");
    // font
    ClassDB::bind_method(D_METHOD("set_font", "_font"), &SpriteText::set_font);
    ClassDB::bind_method(D_METHOD("get_font"), &SpriteText::get_font);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "font", PROPERTY_HINT_FILE, "*.tscn"), "set_font", "get_font");
    // font scale
    ClassDB::bind_method(D_METHOD("set_font_scale", "_font_scale"), &SpriteText::set_font_scale);
    ClassDB::bind_method(D_METHOD("get_font_scale"), &SpriteText::get_font_scale);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "font_scale"), "set_font_scale", "get_font_scale");
    // font color
    ClassDB::bind_method(D_METHOD("set_font_color", "_font_color"), &SpriteText::set_font_color);
    ClassDB::bind_method(D_METHOD("get_font_color"), &SpriteText::get_font_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "font_color"), "set_font_color", "get_font_color");
    // font shader
    ClassDB::bind_method(D_METHOD("set_font_shader", "_font_shader"), &SpriteText::set_font_shader);
    ClassDB::bind_method(D_METHOD("get_font_shader"), &SpriteText::get_font_shader);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "font_shader", PROPERTY_HINT_FILE, "*.gdshader"), "set_font_shader", "get_font_shader");
    // text
    ClassDB::bind_method(D_METHOD("set_text", "_text"), &SpriteText::set_text);
    ClassDB::bind_method(D_METHOD("get_text"), &SpriteText::get_text);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "text", PROPERTY_HINT_MULTILINE_TEXT), "set_text", "get_text");
    // write speed
    ClassDB::bind_method(D_METHOD("set_write_speed", "_write_speed"), &SpriteText::set_write_speed);
    ClassDB::bind_method(D_METHOD("get_write_speed"), &SpriteText::get_write_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "write_speed"), "set_write_speed", "get_write_speed");
    // alignment
    ClassDB::bind_method(D_METHOD("set_alignment", "_alignment"), &SpriteText::set_alignment);
    ClassDB::bind_method(D_METHOD("get_alignment"), &SpriteText::get_alignment);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "alignment", PROPERTY_HINT_ENUM, "Left,Center,Right"), "set_alignment", "get_alignment");
    // word wrap
    ClassDB::bind_method(D_METHOD("set_word_wrap", "_word_wrap"), &SpriteText::set_word_wrap);
    ClassDB::bind_method(D_METHOD("get_word_wrap"), &SpriteText::get_word_wrap);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "word_wrap"), "set_word_wrap", "get_word_wrap");
    // text margin
    ClassDB::bind_method(D_METHOD("set_text_margin", "_text_margin"), &SpriteText::set_text_margin);
    ClassDB::bind_method(D_METHOD("get_text_margin"), &SpriteText::get_text_margin);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "text_margin"), "set_text_margin", "get_text_margin");
    // auto scroll
    ClassDB::bind_method(D_METHOD("set_auto_scroll", "_auto_scroll"), &SpriteText::set_auto_scroll);
    ClassDB::bind_method(D_METHOD("get_auto_scroll"), &SpriteText::get_auto_scroll);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_scroll"), "set_auto_scroll", "get_auto_scroll");
    // scroll speed
    ClassDB::bind_method(D_METHOD("set_scroll_speed", "_scroll_speed"), &SpriteText::set_scroll_speed);
    ClassDB::bind_method(D_METHOD("get_scroll_speed"), &SpriteText::get_scroll_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "scroll_speed"), "set_scroll_speed", "get_scroll_speed");

    // METHODS
    ClassDB::bind_method(D_METHOD("add_scroll_box"), &SpriteText::add_scroll_box);
    ClassDB::bind_method(D_METHOD("clear"), &SpriteText::clear);
    ClassDB::bind_method(D_METHOD("write", "new_text", "clear_old_text"), &SpriteText::write);

    // SIGNALS
    ADD_SIGNAL(MethodInfo("write_complete"));
}

/*****************************************************
PROPERTIES
*****************************************************/
void SpriteText::set_text_update(bool is_test) {
    text_update = is_test;
    write(text);
    text_update = false;
}
bool SpriteText::get_text_update() { return text_update; }

// Font
void SpriteText::set_font(String path) {
    font_path = path;
    set_font_res();
}

String SpriteText::get_font() { return font_path; }

bool SpriteText::set_font_res() {
    if (font_path == "")
        return false;
    Ref<FileAccess> file = nullptr;
    if (!file->file_exists(font_path))
        return false;
    font_res = ResourceLoader::get_singleton()->load(font_path);
    SpriteFont* c = cast_to<SpriteFont>(font_res->instantiate());
    font_size = c->character_size;
    c->queue_free();
    return true;
}

void SpriteText::set_font_scale(Vector2 s) { font_scale = s; } Vector2 SpriteText::get_font_scale() { return font_scale; }
void SpriteText::set_font_color(Color new_font_color) { font_color = new_font_color; } Color SpriteText::get_font_color() { return font_color; }

void SpriteText::set_font_shader(String new_font_shader) {
    font_shader = new_font_shader;
    set_font_shader_res();
}
String SpriteText::get_font_shader() { return font_shader; }

bool SpriteText::set_font_shader_res() {
    if (font_shader == "")
        return false;
    Ref<FileAccess> file = nullptr;
    if (!file->file_exists(font_shader))
        return false;
    font_shader_res = ResourceLoader::get_singleton()->load(font_shader);
    return true;
}

// Writing
void SpriteText::set_text(String new_text) {
    text = new_text;
    if (Engine::get_singleton()->is_editor_hint())
        write(new_text, true);
}
String SpriteText::get_text() { return text; }

void SpriteText::set_write_speed(float new_write_speed) { write_speed = new_write_speed; } float SpriteText::get_write_speed() { return write_speed; }
void SpriteText::set_alignment(int new_alignment) { alignment = new_alignment; } int SpriteText::get_alignment() { return alignment; }
void SpriteText::set_word_wrap(bool is_word_wrap) { word_wrap = is_word_wrap; } bool SpriteText::get_word_wrap() { return word_wrap; }
void SpriteText::set_text_margin(Vector2 new_margin) { text_margin = new_margin; } Vector2 SpriteText::get_text_margin() { return text_margin; }
void SpriteText::set_auto_scroll(bool is_auto_scroll) { auto_scroll = is_auto_scroll; } bool SpriteText::get_auto_scroll() { return auto_scroll; }
void SpriteText::set_scroll_speed(float new_scroll_speed) { scroll_speed = new_scroll_speed; } float SpriteText::get_scroll_speed() { return scroll_speed; }

// Need to call this as a separate deferred function to make it work right
void SpriteText::add_scroll_box() {
    scr = memnew(ReferenceRect);
    scr->set_name("scroll");
    add_child(scr);
    scr->set_owner(this);
    scr->set_size(Vector2(get_size().x, get_size().y + 1.0f));
    scr->set_border_color(Color(1.0f, 0.0f, 0.0f, 0.5f));
}

/*****************************************************
WRITE
*****************************************************/
void SpriteText::clear() {
    if (is_queued_for_deletion())
        return;
    if (scr == nullptr)
        return;
    while (scr->get_child_count() > 0) {
        scr->get_child(0)->queue_free();
        scr->remove_child(scr->get_child(0));
    }
    text = String();
    write_pos = Vector2();
    scr->set_size(Vector2(scr->get_size().x, font_size.y));
    scr->set_position(Vector2());
}

void SpriteText::write(String new_text, bool clear_text) {
    if (is_queued_for_deletion())
        return;
    if (scr == nullptr)
        return;
    if (clear_text)
        clear();
    if (!set_font_res()) {
        text = String();
        return;
    }
    text = new_text;
    write_progress = 0;
    write_ct = 0.0f;
    write_loop(get_process_delta_time());
}

void SpriteText::write_loop(float delta) {
    if (scr == nullptr)
        return;

    // Writing
    if (write_progress < text.length()) {
        // Instant gratification
        if (write_speed < 0.0f) {
            for (int i = 0; i < text.length() - write_progress; i++)
                if (is_inside_tree())
                    write_char(i + write_progress);
            write_progress = text.length();
        }
        // Typewriter effect
        else {
            if (write_ct <= 0)
            {
                write_ct = write_speed;
                write_char(write_progress);
                write_progress++;
            }
            else
                write_ct -= delta;
        }
    }

    // Finish writing
    if (write_progress == text.length()) {
        write_progress++;
        emit_signal("write_complete");
    }
    if (scr->get_child_count() > MAX_CHARS)
        scr->get_child(0)->queue_free();
}

void SpriteText::write_char(int i) {
    if (scr == nullptr)
        return;
    if (!font_res.is_valid())
        return;
    if (i >= text.length())
        return;
    float px = write_pos.x, py = write_pos.y;
    float fx = font_size.x * font_scale.x + text_margin.x, fy = font_size.y * font_scale.y + text_margin.y;
    // Create the character
    // Newline
    if (text[i] == L'\n') {
        py += 1;
        px = 0;
    }
    else if (text[i] == L'\\' && text[i + 1] == L'n') {
        py += 1;
        px = 0;
        write_progress++;
    }
    // Space
    else if (text[i] == L' ') {
        write_ct = 0.0f;
        px += 1;
        if (word_wrap) {
            if (text.find(" ", i + 1) < 0)
                next_word_length = text.length() - i - 1;
            else
                next_word_length = text.find(" ", i + 1) - i - 1;
            // We only wrap if we don't find a newline in the middle of our word
            if (text.find("\n", i + 1) == -1 || text.find("\n", i + 1) > i + next_word_length - 2)
                if ((px + next_word_length) * fx >= get_size().x) {
                    py += 1;
                    px = 0;
                }
        }
        if (px > 0) {
            SpriteFont* c = cast_to<SpriteFont>(font_res->instantiate());
            c->hide();
            c->set_scale(font_scale);
            scr->add_child(c);
            c->set_owner(scr);
            c->set_position(Vector2(px * fx, py * fy));
        }
    }
    // Standard character
    else {
        SpriteFont* c = cast_to<SpriteFont>(font_res->instantiate());
        // Are we trying to parse a control hint?
        if (text[i] == L'$' && text[i + 1] == L'c') {
            c->set_frame(int(Math::clamp(int(text[i + 2]) * 10 + int(text[i + 3]), 0, 48)));
            write_progress += 3;
        }
        // Find the correct character in our text texture
        else {
            int frame = c->get_character_set().find(String(&text[i]).left(1), 0);
            if (frame >= 0)
                c->set_frame(Math::min(frame, int(c->get_hframes() * c->get_vframes())));
            else
                c->hide();
        }
        // Colorize the character
        c->set_modulate(font_color);
        // Obvious
        c->set_scale(font_scale);
        // We can apply a custom shader to the individual characters
        if (font_shader != "") {
            //c->set_material(font_shader_res->duplicate());
            Ref<ShaderMaterial>((c)->get_material())->set_shader_parameter("char_index", i);
        }
        // Add the character to the tree
        scr->add_child(c);
        c->set_owner(scr);
        // Positioning is complicated, huh?
        if (float(px) * fx >= get_size().x) {
            py += 1;
            px = 0;
        }
        c->set_position(Vector2(px * fx, py * fy));
        if (alignment != ALIGN::LEFT) {
            int t_ct = int(scr->get_child_count()) - 1;
            for (int j = 0; j < px + 1; j++) {
                if (t_ct - j >= 0) {
                    SpriteFont* align_c = cast_to<SpriteFont>(scr->get_child(t_ct - j));
                    float x = 0.0f;
                    if (alignment == ALIGN::CENTER) {
                        x = scr->get_size().x * 0.5f + px * 0.5f * fx;
                        x -= fx * j;
                    }
                    else if (alignment == ALIGN::RIGHT) {
                        x = scr->get_size().x - fx + text_margin.x;
                        x -= fx * j;
                    }
                    align_c->set_position(Vector2(x, py * fy));
                }
            }
        }
        px += 1;
    }
    scr->set_size(Vector2(scr->get_size().x, float(py + 1) * fy + 1.0f));
    write_pos = Vector2(px, py);
}

/*****************************************************
BASE PROCESSING
*****************************************************/
SpriteText::SpriteText() {}
SpriteText::~SpriteText() {}

void SpriteText::ready() {
    set_clip_contents(true);
    set_font_res();
    if (has_node("scroll")) {
        scr = get_node<ReferenceRect>("scroll");
        scr->set_size(Vector2(get_size().x, get_size().y + 1.0f));
    }
    else
        call_deferred("add_scroll_box");
    set_border_color(Color(0.0f, 1.0f, 0.0f, 0.5f));
}

void SpriteText::process(float delta) {
    // safety in case we haven't autogenerated our scroll node yet
    if (scr == nullptr) {
        if (has_node("scroll")) {
            scr = get_node<ReferenceRect>("scroll");
            scr->set_size(Vector2(get_size().x, get_size().y + text_margin.y + 1.0f));
        }
        else
            return;
    }

    // Writing
    write_loop(delta);

    // Scrolling
    scr->set_size(Vector2(get_size().x, scr->get_size().y));
    if (auto_scroll && scr->get_size().y > get_size().y) {
        if (scroll_speed > 0.0f)
            scr->set_position(Vector2(scr->get_position().x, fmaxf(scr->get_position().y - scroll_speed * delta, get_size().y - scr->get_size().y)));
        else
            scr->set_position(Vector2(scr->get_position().x, get_size().y - scr->get_size().y));
    }
}

void SpriteText::_notification(int _notif) {
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