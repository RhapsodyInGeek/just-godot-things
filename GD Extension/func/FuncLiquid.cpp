#include "GameDefs.h"
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
void FuncLiquid::_bind_methods() {
	// properties
	ClassDB::bind_method(D_METHOD("set_properties", "_properties"), &FuncLiquid::set_properties);
	ClassDB::bind_method(D_METHOD("get_properties"), &FuncLiquid::get_properties);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");
	// appearance_flags
	ClassDB::bind_method(D_METHOD("set_appearance_flags", "_appearance_flags"), &FuncLiquid::set_appearance_flags);
	ClassDB::bind_method(D_METHOD("get_appearance_flags"), &FuncLiquid::get_appearance_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "appearance_flags", PROPERTY_HINT_FLAGS,
		"Not On Easy,Not On Normal,Not On Hard,Not On Nightmare,Not In Deathmatch,Not In Team Deathmatch"), "set_appearance_flags", "get_appearance_flags");
	// liquid type
	ClassDB::bind_method(D_METHOD("set_liquid_type", "_liquid_type"), &FuncLiquid::set_liquid_type);
	ClassDB::bind_method(D_METHOD("get_liquid_type"), &FuncLiquid::get_liquid_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "liquid_type", PROPERTY_HINT_ENUM,
		"Water,Slime,Lava,Space"), "set_liquid_type", "get_liquid_type");
	// liquid color
	ClassDB::bind_method(D_METHOD("set_liquid_color", "_liquid_color"), &FuncLiquid::set_liquid_color);
	ClassDB::bind_method(D_METHOD("get_liquid_color"), &FuncLiquid::get_liquid_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "liquid_color"), "set_liquid_color", "get_liquid_color");

	ClassDB::bind_method(D_METHOD("reparent_entity"), &FuncLiquid::reparent_entity);
	ClassDB::bind_method(D_METHOD("update_children"), &FuncLiquid::update_children);
	ClassDB::bind_method(D_METHOD("_on_ent_entered", "ent"), &FuncLiquid::_on_ent_entered);
	ClassDB::bind_method(D_METHOD("_on_ent_exited", "ent"), &FuncLiquid::_on_ent_exited);
}

void FuncLiquid::set_properties(Dictionary p) {
	properties = p;
	if (!Engine::get_singleton()->is_editor_hint())
		return;
	if (properties.has("appearance_flags"))
		appearance_flags = (uint8_t)(int)properties["appearance_flags"];
	if (properties.has("liquid_type")) {
		liquid_type = (uint8_t)(int)properties["liquid_type"];
		switch (liquid_type) {
		case SLIME:
			liquid_color = GameManager::get_color(0x001905, 0.588235f);
			break;
		case LAVA:
			liquid_color = GameManager::get_color(0xff5000, 0.784313f);
			break;
		case SPACE:
			liquid_color = GameManager::get_color(0x001905, 0.588235f);
			break;
		default:
			liquid_color = GameManager::get_color(0x204080, 0.5f);
		}
	}
	if (properties.has("color")) {
		liquid_color = properties["color"];
		liquid_color.a = 0.5f;
	}
	if (properties.has("opacity"))
		liquid_color.a = properties["opacity"];
	if (properties.has("moveparent"))
		call_deferred("reparent_entity");
	for (int i = 0; i < get_child_count(); i++)	{
		//if (get_child(i)->get_class() == "CollisionShape3D")
			//cast_to<CollisionShape>(get_child(i))->get_shape()->set_margin(0.01f);
		if (get_child(i)->get_class() == "MeshInstance3D") {
			MeshInstance3D* m = cast_to<MeshInstance3D>(get_child(i));
			m->set_layer_mask(CULL_DYNAMIC);
			m->set_gi_mode(GeometryInstance3D::GI_MODE_STATIC);
			m->set_cast_shadows_setting(GeometryInstance3D::SHADOW_CASTING_SETTING_OFF);
			if (m->get_mesh()->get_class() == "ArrayMesh")
				Ref<ArrayMesh>(m->get_mesh())->lightmap_unwrap(Transform3D(), INVERSE_SCALE);
			m->set_ignore_occlusion_culling(false);
			if (properties.has("nocull"))
				if (properties["nocull"])
					m->set_ignore_occlusion_culling(true);
		}
	}
	call_deferred("update_children");
}

Dictionary FuncLiquid::get_properties() { return properties; }
void FuncLiquid::set_appearance_flags(int f) { appearance_flags = (uint8_t)f; }
int FuncLiquid::get_appearance_flags() { return (int)appearance_flags; }
void FuncLiquid::set_liquid_type(int l) { liquid_type = (uint8_t)l; }
int FuncLiquid::get_liquid_type() { return (int)liquid_type; }
void FuncLiquid::set_liquid_color(Color c) { liquid_color = c; }
Color FuncLiquid::get_liquid_color() { return liquid_color; }

void FuncLiquid::reparent_entity() {
	String moveparent = properties["moveparent"];
	moveparent = "entity_func_move_" + moveparent;
	if (get_parent()->get_name() == moveparent)
		return;
	if (is_inside_tree())
		if (get_parent()->has_node(NodePath(moveparent))) {
			Transform3D t = get_global_transform();
			Node* new_parent = get_parent()->get_node<Node>(NodePath(moveparent));
			get_parent()->remove_child(this);
			new_parent->add_child(this);
			set_global_transform(t);
			set_owner(new_parent->get_owner());
			for (int i = 0; i < get_child_count(); i++) {
				Node* n = get_child(i);
				n->set_owner(get_owner());
				if (n->get_class() == "MeshInstance3D")
					cast_to<MeshInstance3D>(n)->set_ignore_occlusion_culling(true);
			}
		}
}

void FuncLiquid::update_children() {
	// Slime and Lava HURT
	int dmg = 0;
	switch (liquid_type) {
	case SLIME:
		dmg = 8;
		break;
	case LAVA:
		dmg = 30;
		break;
	}
	// We can have custom damage liquid volumes because why not?
	if (properties.has("damage"))
		dmg = properties["damage"];
	// If it's not a damage type liquid, get rid of any existing hurt volumes
	if (has_node("hurt") && dmg != get_node<TriggerHurt>("hurt")->get_damage()) {
		Node* oldhurt = get_node<Node>("hurt");
		oldhurt->set_name("remove");
		oldhurt->queue_free();
	}
	// Bring on the hurt!
	if (dmg > 0 && !has_node("hurt")) {
		TriggerHurt* hurt = memnew(TriggerHurt);
		add_child(hurt);
		hurt->set_name("hurt");
		hurt->set_owner(get_owner());
		for (int i = 0; i < get_child_count(); i++)
			if (get_child(i)->get_class() == "CollisionShape3D") {
				CollisionShape3D* c = cast_to<CollisionShape3D>(get_child(i)->duplicate());
				hurt->add_child(c);
				c->set_owner(hurt->get_owner());
			}
		hurt->damage = dmg;
		hurt->delay = 1.0f;
		hurt->can_gib = false;
	}
}

void FuncLiquid::_on_ent_entered(Node* ent) {
	if (ent->is_in_group("ACTOR"))
		ent->call("enter_water", this);
}

void FuncLiquid::_on_ent_exited(Node* ent) {
	if (ent->is_in_group("ACTOR"))
		ent->call("exit_water", this);
}

FuncLiquid::FuncLiquid() {
	add_to_group("FUNC_LIQUID");
	connect("body_entered", Callable(this, "_on_ent_entered"));
	connect("body_exited", Callable(this, "_on_ent_exited"));
	set_monitorable(true);
	set_monitoring(true);
	set_collision_layer(LIQUID_LAYER);
	set_collision_mask(ACTOR_LAYER + DEAD_LAYER);
}

FuncLiquid::~FuncLiquid() {}

void FuncLiquid::_ready() {
	if (!Engine::get_singleton()->is_editor_hint()) {
		if (get_node<GameManager>(GAME_PATH)->appearance_check(appearance_flags)) {
			queue_free();
		}
	}
}