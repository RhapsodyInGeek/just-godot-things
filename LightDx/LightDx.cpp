/*******************************************************************************
LIGHT DX
Light node governed by Trenchbroom key / value pairs
Register as tool class in NativeLib.cpp
*******************************************************************************/
#include "LightDx.h"

void LightDx::_register_methods()
{
	register_property("properties", &LightDx::set_properties, &LightDx::get_properties, {});
	register_property("energy_1", &LightDx::set_energy_1, &LightDx::get_energy_1, 1.0f);
	register_property("energy_2", &LightDx::set_energy_2, &LightDx::get_energy_2, 0.0f);
	register_property("color_1", &LightDx::set_color_1, &LightDx::get_color_1, Color(1.0f, 1.0f, 1.0f));
	register_property("color_2", &LightDx::set_color_2, &LightDx::get_color_2, Color());
	register_property("style", &LightDx::set_style, &LightDx::get_style, String("m"));
	register_property("style_time", &LightDx::set_style_time, &LightDx::get_style_time, 3600.0f);
	register_method("reparent", &LightDx::reparent);
	register_method("update_style", &LightDx::update_style);
	register_method("trigger", &LightDx::trigger);
	register_method("data_save", &LightDx::data_save);
	register_method("data_load", &LightDx::data_load);
	register_method("_ready", &LightDx::_ready);
	register_method("_process", &LightDx::_process);
}

void LightDx::set_properties(Dictionary new_properties)
{
	properties = new_properties;
	if (!Engine::get_singleton()->is_editor_hint())
		return;
	// Make sure we light both the world visual layer and the view weapon layer
	set_layer_mask(3);
	set_cull_mask(3);
	set_bake_mode(BAKE_ALL);
	if (get_class() == "SpotLight")
	{
		// Spotlight direction
		set_rotation_degrees(GameManager::demangler(properties,1));
		if (properties.has("mangle"))
		{
			Vector3 m = properties["mangle"];
			set_rotation_degrees(Vector3(m.y, m.x + 180.0f, m.z));
		}
		else
			set_rotation_degrees(Vector3(0.0f, 180.0f, 0.0f));
		// Spotlight cone angle
		if (properties.has("angle"))
			set_param(PARAM_SPOT_ANGLE, properties["angle"]);
		if (properties.has("delay"))
			set_param(PARAM_SPOT_ATTENUATION, properties["delay"]);
	};
	// Default energy is 1.0f
	if (properties.has("light"))
	{
		set_negative(float(properties["light"]) < 0.0f);
		energy_1 = abs(float(properties["light"]));
		set_param(PARAM_ENERGY, energy_1);
		set_param(PARAM_INDIRECT_ENERGY, energy_1);
	};
	// Used for styles - we can interpolate between 2 energy levels, most useful for controlling subtle pulses or flickers
	if (properties.has("light2"))
		energy_2 = properties["light2"];
	// Range is 'wait'/32, since 32 Trenchbroom units = 1 Godot unit
	if (properties.has("wait"))
		set_param(PARAM_RANGE, float(properties["wait"]) * 0.03125f);
	else
		set_param(PARAM_RANGE, 9.375f);
	// Light falloff
	if (properties.has("delay"))
		set_param(PARAM_ATTENUATION, properties["delay"]);
	// Dynamic shadows can be expensive - use sparingly
	if (properties.has("shadows"))
	{
		set_shadow(bool(properties["shadows"]));
		if (int(properties["shadows"]) > 1)
			set_bake_mode(BAKE_INDIRECT);
	};
	// Mood lighting
	if (properties.has("_color"))
	{
		color_1 = properties["_color"];
		color_2 = color_1;
		set_color(color_1);
	};
	// Used with style - we can interpolate between 2 colors
	if (properties.has("_color2"))
		color_2 = properties["_color2"];
	// Animation style - please see the style functions below
	if (properties.has("style"))
		update_style(properties["style"]);
	// Lightmap setup
	if (style.length() > 1 || properties.has("targetname"))
		set_bake_mode(BAKE_DISABLED);
	if (properties.has("start_off"))
	{
		set_visible(!bool("start_off"));
		if (is_visible() == false)
			set_bake_mode(BAKE_DISABLED);
	};
	if (properties.has("moveparent"))
	{
		set_bake_mode(BAKE_DISABLED);
		call_deferred("reparent");
	};
}

Dictionary LightDx::get_properties() { return properties; }
void LightDx::set_style(String s) { style = s; }
String LightDx::get_style() { return style; }
void LightDx::set_style_time(float t) { style_time = t; }
float LightDx::get_style_time() { return style_time; }
void LightDx::set_energy_1(float e) { energy_1 = e; }
float LightDx::get_energy_1() { return energy_1; }
void LightDx::set_energy_2(float e) { energy_2 = e; }
float LightDx::get_energy_2() { return energy_2; }
void LightDx::set_color_1(Color c) { color_1 = c; }
Color LightDx::get_color_1() { return color_1; }
void LightDx::set_color_2(Color c) { color_2 = c; }
Color LightDx::get_color_2() { return color_2; }

void LightDx::reparent()
{
	String moveparent = properties["moveparent"];
	moveparent = "entity_func_move_" + moveparent;
	if (get_parent()->get_name() == moveparent)
		return;
	if (is_inside_tree())
		if (get_parent()->has_node(NodePath(moveparent)))
		{
			Transform t = get_global_transform();
			Node* new_parent = get_parent()->get_node(NodePath(moveparent));
			get_parent()->remove_child(this);
			new_parent->add_child(this);
			set_global_transform(t);
			set_owner(new_parent->get_owner());
			set("portal_mode", CullInstance::PORTAL_MODE_GLOBAL);
		};
}

void LightDx::trigger(Node* caller) { set_visible(!is_visible()); }

// LIGHT STYLES
void LightDx::update_style(int style_id)
{
	switch (style_id)
	{
	case(1):
		// Flicker 1
		style = "mmnmmommommnonmmonqnmmo";
		break;
	case(2):
		// Slow Strong Pulse
		style = "abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcba";
		break;
	case(3):
		// Candle 1
		style = "mmmmmaaaaammmmmaaaaaabcdefgabcdefg";
		break;
	case(4):
		// Fast Strobe
		style = "mamamamamama";
		break;
	case(5):
		// Gentle Pulse
		style = "jklmnopqrstuvwxyzyxwvutsrqponmlkj";
		break;
	case(6):
		// Flicker 2
		style = "nmonqnmomnmomomno";
		break;
	case(7):
		// Candle 2
		style = "mmmaaaabcdefgmmmmaaaammmaamm";
		break;
	case(8):
		// Candle 3
		style = "mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa";
		break;
	case(9):
		// Slow Strobe
		style = "aaaaaaaazzzzzzzz";
		break;
	case(10):
		// Flourescent Flicker
		style = "mmamammmmammamamaaamammma";
		break;
	case(11):
		// Slow Pulse, No Fade To Black
		style = "abcdefghijklmnopqrrqponmlkjihgfedcba";
		break;
	case(63):
		// 2nd
		style = "a";
		break;
	default:
		// Stay at energy_1, color_1
		style = "m";
		break;
	};
	style_vec.clear();
	for (int i = 0; i < style.length(); i++)
		style_vec.push_back(float(String("abcdefghijklmnopqrstuvwxyz").find(style[i])) * 0.0833333f);
}

Dictionary LightDx::data_save()
{
	Dictionary data;
	data["on"] = is_visible();
	data["style"] = style;
	data["style_ct"] = style_ct;
	return data;
}

void LightDx::data_load(Dictionary data)
{
	set_visible(data["on"]);
	style = data["style"];
	style_ct = data["style_ct"];
	add_to_group("SAV");
}

void LightDx::_init(){}

void LightDx::_ready()
{
	if (!Engine::get_singleton()->is_editor_hint())
	{
		if (get_bake_mode() != BAKE_ALL)
			add_to_group("SAV");
		if (properties.has("targetname"))
			cast_to<GameManager>(get_node("/root/GameManager"))->set_node_targetname(this, properties["targetname"]);
		style_vec.clear();
		for (int i = 0; i < style.length(); i++)
			style_vec.push_back(float(String("abcdefghijklmnopqrstuvwxyz").find(style[i])) * 0.0833333f);
	};
}

void LightDx::_process(float delta)
{
	if (style.length() > 1)
	{
		style_ct += delta;
		if (style_ct >= 3600.0f)
			style_ct -= 3600.0f;
		int i = int(floor(style_ct * 10.0f)) % style_vec.size();
		float t = style_vec[i];
		set_param(Light::PARAM_ENERGY, Math::lerp(energy_2, energy_1, t));
		set_color(color_2.linear_interpolate(color_1, t));
	};
}
