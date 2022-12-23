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
#include "FuncLiquid.h"

void FuncLiquid::_register_methods()
{
	register_property("properties", &FuncLiquid::set_properties, &FuncLiquid::get_properties, {});
	register_property("liquid_type", &FuncLiquid::set_liquid_type, &FuncLiquid::get_liquid_type, 0, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_ENUM,
		"Water,Slime,Lava,Space");
	register_property("liquid_color", &FuncLiquid::set_liquid_color, &FuncLiquid::get_liquid_color, GameManager::get_color(0x325082, 0.5f));
	register_method("reparent", &FuncLiquid::reparent);
	register_method("update_children", &FuncLiquid::update_children);
	register_method("_on_ent_entered", &FuncLiquid::_on_ent_entered);
	register_method("_on_ent_exited", &FuncLiquid::_on_ent_exited);
}

void FuncLiquid::set_properties(Dictionary p)
{
	properties = p;
	if (!Engine::get_singleton()->is_editor_hint())
		return;
	if (properties.has("liquid_type"))
	{
		liquid_type = properties["liquid_type"];
		switch (liquid_type)
		{
		case GameManager::SLIME:
			liquid_color = GameManager::get_color(0x001905, 0.588235f);
			break;
		case GameManager::LAVA:
			liquid_color = GameManager::get_color(0xff5000, 0.784313f);
			break;
		case GameManager::SPACE:
			liquid_color = GameManager::get_color(0x001905, 0.588235f);
			break;
		default:
			liquid_color = GameManager::get_color(0x204080, 0.5f);
		};
	};
	if (properties.has("_color"))
	{
		liquid_color = properties["_color"];
		liquid_color.a = 0.5f;
	};
	if (properties.has("opacity"))
		liquid_color.a = properties["opacity"];
	if (properties.has("moveparent"))
		call_deferred("reparent");
	for (int i = 0; i < get_child_count(); i++)
	{
		//if (get_child(i)->get_class() == "CollisionShape")
			//cast_to<CollisionShape>(get_child(i))->get_shape()->set_margin(0.01f);
		if (get_child(i)->get_class() == "MeshInstance")
		{
			MeshInstance* m = cast_to<MeshInstance>(get_child(i));
			m->set_cast_shadows_setting(GeometryInstance::SHADOW_CASTING_SETTING_OFF);
			m->set_flag(GeometryInstance::FLAG_USE_BAKED_LIGHT, true);
			if (m->get_mesh()->get_class() == "ArrayMesh")
				Ref<ArrayMesh>(m->get_mesh())->lightmap_unwrap(Transform::IDENTITY, 0.3125f);
			if (properties.has("nocull"))
				if (properties["nocull"])
					m->set_portal_mode(CullInstance::PORTAL_MODE_GLOBAL);
		};
	};
	call_deferred("update_children");
}

Dictionary FuncLiquid::get_properties() { return properties; }
void FuncLiquid::set_liquid_type(int l) { liquid_type = l; }
int FuncLiquid::get_liquid_type() { return liquid_type; }
void FuncLiquid::set_liquid_color(Color c) { liquid_color = c; }
Color FuncLiquid::get_liquid_color() { return liquid_color; }

void FuncLiquid::reparent()
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
			for (int i = 0; i < get_child_count(); i++)
			{
				Node* n = get_child(i);
				n->set_owner(get_owner());
				if (n->get_class() == "MeshInstance")
					cast_to<MeshInstance>(n)->set_portal_mode(GeometryInstance::PORTAL_MODE_GLOBAL);
			};
		};
}

void FuncLiquid::update_children()
{
	// Slime and Lava HURT
	int dmg = 0;
	switch (liquid_type)
	{
	case GameManager::SLIME:
		dmg = 8;
		break;
	case GameManager::LAVA:
		dmg = 30;
		break;
	};
	// We can have custom damage liquid volumes because why not?
	if (properties.has("damage"))
		dmg = properties["damage"];
	// If it's not a damage type liquid, get rid of any existing hurt volumes
	if (has_node("hurt") && dmg != cast_to<TriggerHurt>(get_node("hurt"))->get_damage())
	{
		Node* oldhurt = get_node("hurt");
		oldhurt->set_name("remove");
		oldhurt->queue_free();
	}
	// Bring on the hurt!
	if (dmg > 0 && !has_node("hurt"))
	{
		TriggerHurt* hurt = TriggerHurt::_new();
		add_child(hurt);
		hurt->set_name("hurt");
		hurt->set_owner(get_owner());
		for (int i = 0; i < get_child_count(); i++)
			if (get_child(i)->get_class() == "CollisionShape")
			{
				CollisionShape* c = cast_to<CollisionShape>(get_child(i)->duplicate());
				hurt->add_child(c);
				c->set_owner(hurt->get_owner());
			};
		hurt->set_damage(dmg);
		hurt->set("delay",1.0f);
		hurt->set_can_gib(false);
	};
}

void FuncLiquid::_on_ent_entered(Node* ent)
{
	if (ent->is_in_group("ACTOR"))
		ent->call("enter_water", this);
}

void FuncLiquid::_on_ent_exited(Node* ent)
{
	if (ent->is_in_group("ACTOR"))
		ent->call("exit_water", this);
}

void FuncLiquid::_init()
{
	connect("body_entered", this, "_on_ent_entered");
	connect("body_exited", this, "_on_ent_exited");
	set_monitorable(true);
	set_monitoring(true);
	set_collision_layer(GameManager::LIQUID_LAYER);
	set_collision_mask(GameManager::ACTOR_LAYER + GameManager::DEAD_LAYER);
}
