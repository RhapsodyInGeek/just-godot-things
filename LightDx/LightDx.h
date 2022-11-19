/*******************************************************************************
LIGHT DX
Light node governed by Trenchbroom key / value pairs
Register as a tool class in NativeLib.cpp
*******************************************************************************/
#pragma once
#include "Godot.hpp"
#include <Math.hpp>
#include "Engine.hpp"
#include "Light.hpp"
//#include "Game.h"

class LightDx : public Light
{
private:
	GODOT_CLASS(LightDx, Light);
	Dictionary properties = {};
	String style = "m";
	float style_ct = 0.0f, style_time = 3600.0f, energy_1 = 1.0f, energy_2 = 0.0f;
	Color color_1 = Color(1.0f, 1.0f, 1.0f), color_2 = Color();
	std::vector<float> style_vec = {};
public:
	static void _register_methods();
	void set_properties(Dictionary new_properties);	Dictionary get_properties();
	void set_style(String s); String get_style();
	void set_style_time(float t); float get_style_time();
	void set_energy_1(float e); float get_energy_1();
	void set_energy_2(float e); float get_energy_2();
	void set_color_1(Color c); Color get_color_1();
	void set_color_2(Color c); Color get_color_2();
	void reparent();
	void trigger(Node* caller);
	void update_style(int style_id);
	Dictionary data_save();
	void data_load(Dictionary data);
	void _init();
	void _ready();
	void _process(float delta);
};
