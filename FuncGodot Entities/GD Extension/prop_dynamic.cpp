/*************************************************************************
PROP DYNAMIC

Dynamic prop generator. Prop models are auto generated based upon a
customizable GLTF resource path.

Some props have access to an AnimationPlayer. Other props may have
StaticBody3D collision. We can provide optional access to these with
generic functions.
**************************************************************************/
#include "prop_dynamic.h"
#include "../sys/game_manager.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include "../func/func_move.h"

void PropDynamic::_bind_methods() {
	// appearance_flags
	ClassDB::bind_method(D_METHOD("set_appearance_flags", "appearance_flags"), &PropDynamic::set_appearance_flags);
	ClassDB::bind_method(D_METHOD("get_appearance_flags"), &PropDynamic::get_appearance_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "appearance_flags", PROPERTY_HINT_FLAGS,
		APPEARANCE_FLAG_LIST), "set_appearance_flags", "get_appearance_flags");
	// grav type
	ClassDB::bind_method(D_METHOD("set_grav_type", "grav_type"), &PropDynamic::set_grav_type);
	ClassDB::bind_method(D_METHOD("get_grav_type"), &PropDynamic::get_grav_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "grav_type", PROPERTY_HINT_ENUM,
		GRAV_TYPE_LIST), "set_grav_type", "get_grav_type");
	// prop_res_path
	ClassDB::bind_method(D_METHOD("set_prop_res_path", "_prop_res_path"), &PropDynamic::set_prop_res_path);
	ClassDB::bind_method(D_METHOD("get_prop_res_path"), &PropDynamic::get_prop_res_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "prop_res_path"), "set_prop_res_path", "get_prop_res_path");
	// next_anim
	ClassDB::bind_method(D_METHOD("set_next_anim", "_next_anim"), &PropDynamic::set_next_anim);
	ClassDB::bind_method(D_METHOD("get_next_anim"), &PropDynamic::get_next_anim);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "next_anim"), "set_next_anim", "get_next_anim");
	// targetname
	ClassDB::bind_method(D_METHOD("set_targetname", "_targetname"), &PropDynamic::set_targetname);
	ClassDB::bind_method(D_METHOD("get_targetname"), &PropDynamic::get_targetname);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "targetname"), "set_targetname", "get_targetname");

	ClassDB::bind_method(D_METHOD("_func_godot_apply_properties", "props"), &PropDynamic::_func_godot_apply_properties);
	ClassDB::bind_method(D_METHOD("_func_godot_build_complete"), &PropDynamic::_func_godot_build_complete);
	ClassDB::bind_method(D_METHOD("update_prop"), &PropDynamic::update_prop);
}

void PropDynamic::set_appearance_flags(int f) { appearance_flags = (uint8_t)f; }
int PropDynamic::get_appearance_flags() { return (int)appearance_flags; }
void PropDynamic::set_grav_type(int g) { grav_type = (uint8_t)g; }
int PropDynamic::get_grav_type() { return (int)grav_type; }

void PropDynamic::set_prop_res_path(String p) { 
	if (prop_res_path != p) {
		prop_res_path = p;
		if (Engine::get_singleton()->is_editor_hint())
			update_prop();
	}
}

String PropDynamic::get_prop_res_path() { return prop_res_path; }

void PropDynamic::update_prop() {
	if (get_owner() != nullptr) {
		if (get_child_count() > 0) {
			for (int i = 0; i < get_child_count(); i++) {
				get_child(i)->set_name("remove");
				get_child(i)->queue_free();
			}
		}

		if (FileAccess::file_exists(prop_res_path) || FileAccess::file_exists(prop_res_path + ".import")) {
			Ref<PackedScene> prop_res = ResourceLoader::get_singleton()->load(prop_res_path);
			Node3D* prop = cast_to<Node3D>(prop_res->instantiate());
			add_child(prop);
			prop->set_owner(get_owner());
		}
		else
			UtilityFunctions::printerr("Invalid prop path!");
	}
}

void PropDynamic::set_anim(String a) { anim = a; }
String PropDynamic::get_anim() { return anim; }
void PropDynamic::set_next_anim(String a) { next_anim = a; }
String PropDynamic::get_next_anim() { return next_anim; }

void PropDynamic::set_targetname(String t) { targetname = t; }
String PropDynamic::get_targetname() { return targetname; }

void PropDynamic::_func_godot_apply_properties(Dictionary props) {
	if (props.has("prop_path")) {
		if ((String)props["prop_path"] != prop_res_path || get_child_count() == 0)
			set_prop_res_path((String)props["prop_path"]);
	}

	if (props.has("scale")) {
		set_scale(GameManager::id_vec_to_godot_vec(props["scale"]));
	}

	if (props.has("anim")) {
		if (get_child_count() > 0) {
			anim = (String)props["anim"];
			AnimationPlayer* ap = cast_to<AnimationPlayer>(get_child(0)->get_node_or_null("AnimationPlayer"));
			if (ap != nullptr) {
				if (ap->has_animation(anim)) {
					ap->play(anim);
					ap->seek(0.0f);
				}
			}
		}
	}

	if (props.has("next_anim"))
		set_next_anim((String)props["next_anim"]);

	if (props.has("moveparent") && (String)props["moveparent"] != "")
		move_parent = (String)props["moveparent"];
}

void PropDynamic::_func_godot_build_complete() {
	FuncMove::func_move_reparent(this, move_parent);
}

void PropDynamic::use(Node* activator) {
	AnimationPlayer* ap = cast_to<AnimationPlayer>(get_child(0)->get_node_or_null("AnimationPlayer"));
	if (ap != nullptr) {
		if (ap->has_animation(next_anim)) {
			ap->play(next_anim);
			ap->seek(0.0f);
		}
	}
}

PropDynamic::PropDynamic() {
	add_to_group("PROP");
}
PropDynamic::~PropDynamic() {}

void PropDynamic::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		if (get_child_count() == 0)
			update_prop();
	}
	else {
		if (GAME_AUTOLOAD->appearance_check(appearance_flags)) {
			queue_free();
			return;
		}
		GAME_AUTOLOAD->set_targetname(this, targetname);
		if (grav_type == GRV_KEEP && get_child_count() > 0) {
			Node3D* prop = cast_to<Node3D>(get_child(0));
			if (prop->get_child_count() > 0) {
				for (int i = 0; i < prop->get_child_count(); i++) {
					if (prop->get_child(i)->get_class() == "StaticBody3D") {
						StaticBody3D* prop_body = cast_to<StaticBody3D>(prop->get_child(i));
						prop_body->add_to_group("WORLD");
						prop_body->add_to_group(GRAV_KEEP);
						prop_body->set_collision_layer(PHYSICS_LAYER_MAP);
						prop_body->set_collision_mask(0);
					}
				}
			}
		}

		if (anim != "") {
			if (get_child_count() > 0) {
				AnimationPlayer* ap = cast_to<AnimationPlayer>(get_child(0)->get_node_or_null("AnimationPlayer"));
				if (ap != nullptr) {
					if (ap->has_animation(anim)) {
						ap->play(anim);
						ap->seek(0.0f);
					}
				}
			}
		}
	}
}