/*******************************************************************************
LIGHT DX
Light node governed by Trenchbroom key / value pairs
Register as tool class in NativeLib.cpp
*******************************************************************************/
#include "LightDx.h"

void LightDx::_register_methods()
{
	register_property("properties", &LightDx::set_properties, &LightDx::get_properties, {});
	register_method("reparent", &LightDx::reparent);
	register_method("trigger", &LightDx::trigger);
	register_method("_ready", &LightDx::_ready);
	register_method("_physics_process", &LightDx::_physics_process);
}

void LightDx::set_properties(Dictionary new_properties)
{
	properties = new_properties;
	// Make sure we light both the world visual layer and the view weapon layer
	set_layer_mask(3);
	set_cull_mask(3);
	set_bake_mode(BAKE_ALL);
	if (get_class() == "SpotLight")
	{
		// Spotlight direction
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
		set_style(properties["style"]);
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

Dictionary LightDx::get_properties()
{
	return properties;
}

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

void LightDx::trigger(Node* caller)
{
	set_visible(!is_visible());
}

// LIGHT STYLES
void LightDx::set_style(int style_id)
{
	switch (style_id)
	{
	case(1):
		// Flicker 1
		style = "mmnmmommommnonmmonqnmmo";
		return;
	case(2):
		// Slow Strong Pulse
		style = "abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcba";
		return;
	case(3):
		// Candle 1
		style = "mmmmmaaaaammmmmaaaaaabcdefgabcdefg";
		return;
	case(4):
		// Fast Strobe
		style = "mamamamamama";
		return;
	case(5):
		// Gentle Pulse
		style = "jklmnopqrstuvwxyzyxwvutsrqponmlkj";
		return;
	case(6):
		// Flicker 2
		style = "nmonqnmomnmomomno";
		return;
	case(7):
		// Candle 2
		style = "mmmaaaabcdefgmmmmaaaammmaamm";
		return;
	case(8):
		// Candle 3
		style = "mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa";
		return;
	case(9):
		// Slow Strobe
		style = "aaaaaaaazzzzzzzz";
		return;
	case(10):
		// Flourescent Flicker
		style = "mmamammmmammamamaaamammma";
		return;
	case(11):
		// Slow Pulse, No Fade To Black
		style = "abcdefghijklmnopqrrqponmlkjihgfedcba";
		return;
	case(63):
		// 2nd
		style = "a";
		return;
	default:
		// Stay at energy_1, color_1
		style = "m";
		return;
	};
}

float LightDx::style_proc(float delta)
{
	style_ct += delta;
	int i = int(style_ct * 10) % style.length();
	float t = float(String("abcdefghijklmnopqrstuvwxyz").find(style[i])) * 0.083333f;
	return t;
}

void LightDx::_init(){}

void LightDx::_ready()
{
	//if (!Engine::get_singleton()->is_editor_hint() && properties.has("targetname"))
	//	cast_to<Game>(get_node("/root/GAME"))->set_node_targetname(this, properties["targetname"]);
}

void LightDx::_physics_process(float delta)
{
	if (style.length() > 1)
	{
		if (style_ct > 10800)
			style_ct = 0.0f;
		float t = style_proc(delta);
		set_param(Light::PARAM_ENERGY, Math::lerp(energy_1, energy_2, t) * float(is_visible()));
		set_color(color_1.linear_interpolate(color_2, t));
	};
}
