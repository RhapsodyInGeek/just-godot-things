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

using namespace godot;

class LightDx : public Light
{
private:
	GODOT_CLASS(LightDx, Light);
	Dictionary properties = {};
	String style = "";
	float style_ct = 0.0f, energy_1 = 1.0f, energy_2 = 0.0f;
	Color color_1 = Color(1.0f, 1.0f, 1.0f), color_2 = Color();
public:
	static void _register_methods();
	void set_properties(Dictionary new_properties);
	Dictionary get_properties();
	void reparent();
	void trigger(Node* caller);
	void set_style(int style_id);
	float style_proc(float delta);
	void _init();
	void _ready();
	void _physics_process(float delta);
};
