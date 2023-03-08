/*******************************************************************************
FUNC: LIQUID VOLUME
Entering this volume sets the Player's move state to swimming and tints the view.

COMMON TINTS
Water - 50 80 130 128
Slime - 0 25 5 150
Lava - 255 80 0 150
Space - ???

Certain liquid types autogenerate a hurt trigger.
*******************************************************************************/
#pragma once
#include "Common.h"
#include <Shape.hpp>
#include <ArrayMesh.hpp>
#include "CollisionShape.hpp"
#include "MeshInstance.hpp"
#include "Trigger.h"

class FuncLiquid : public Area
{
private:
	GODOT_CLASS(FuncLiquid, Area);
	Dictionary properties = {};
	int liquid_type = GameManager::WATER;
	Color liquid_color = Color::hex(0x80325082);
public:
	static void _register_methods();
	void set_properties(Dictionary p); Dictionary get_properties();
	void reparent(); void update_children();
	void set_liquid_type(int t); int get_liquid_type();
	void set_liquid_color(Color c); Color get_liquid_color();
	void _on_ent_entered(Node* ent);
	void _on_ent_exited(Node* ent);
	void _init();
};
