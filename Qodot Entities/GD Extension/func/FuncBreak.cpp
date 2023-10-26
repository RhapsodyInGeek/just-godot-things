#include "GameDefs.h"
/*******************************************************************************
FUNC_BREAK
Destructible geometry.
*******************************************************************************/
void FuncBreak::_bind_methods() {
	// PROPERTIES
	// properties
	ClassDB::bind_method(D_METHOD("set_properties", "_properties"), &FuncBreak::set_properties);
	ClassDB::bind_method(D_METHOD("get_properties"), &FuncBreak::get_properties);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");
	// appearance_flags
	ClassDB::bind_method(D_METHOD("set_appearance_flags", "_appearance_flags"), &FuncBreak::set_appearance_flags);
	ClassDB::bind_method(D_METHOD("get_appearance_flags"), &FuncBreak::get_appearance_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "appearance_flags", PROPERTY_HINT_FLAGS,
		"Not On Easy,Not On Normal,Not On Hard,Not On Nightmare,Not In Deathmatch,Not In Team Deathmatch"), "set_appearance_flags", "get_appearance_flags");
	// flags
	ClassDB::bind_method(D_METHOD("set_flags", "_flags"), &FuncBreak::set_flags);
	ClassDB::bind_method(D_METHOD("get_flags"), &FuncBreak::get_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "flags", PROPERTY_HINT_FLAGS,
		"Clip Collision, , ,No Jump,No Cull"), "set_flags", "get_flags");
	// grav type
	ClassDB::bind_method(D_METHOD("set_grav_type", "_grav_type"), &FuncBreak::set_grav_type);
	ClassDB::bind_method(D_METHOD("get_grav_type"), &FuncBreak::get_grav_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "grav_type", PROPERTY_HINT_ENUM, 
		"Normal,Don't Change,Always Change,Flip"), "set_grav_type", "get_grav_type");
	// health
	ClassDB::bind_method(D_METHOD("set_health", "_health"), &FuncBreak::set_health);
	ClassDB::bind_method(D_METHOD("get_health"), &FuncBreak::get_health);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "health"), "set_health", "get_health");
	// bleed type
	ClassDB::bind_method(D_METHOD("set_bleed_type", "_bleed_type"), &FuncBreak::set_bleed_type);
	ClassDB::bind_method(D_METHOD("get_bleed_type"), &FuncBreak::get_bleed_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "bleed_type", PROPERTY_HINT_ENUM,
		"Red,Yellow Green,Green,Black,Orange,Blue,Light Blue"), "set_bleed_type", "get_bleed_type");
	// targetname
	ClassDB::bind_method(D_METHOD("set_targetname", "targetname"), &FuncBreak::set_targetname);
	ClassDB::bind_method(D_METHOD("get_targetname"), &FuncBreak::get_targetname);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "targetname"), "set_targetname", "get_targetname");
	// target
	ClassDB::bind_method(D_METHOD("set_target", "_target"), &FuncBreak::set_target);
	ClassDB::bind_method(D_METHOD("get_target"), &FuncBreak::get_target);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "target"), "set_target", "get_target");
	// targetfunc
	ClassDB::bind_method(D_METHOD("set_targetfunc", "_targetfunc"), &FuncBreak::set_targetfunc);
	ClassDB::bind_method(D_METHOD("get_targetfunc"), &FuncBreak::get_targetfunc);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "targetfunc"), "set_targetfunc", "get_targetfunc");
	// message
	ClassDB::bind_method(D_METHOD("set_message", "_message"), &FuncBreak::set_message);
	ClassDB::bind_method(D_METHOD("get_message"), &FuncBreak::get_message);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "message", PROPERTY_HINT_MULTILINE_TEXT), "set_message", "get_message");
	// hit sound
	ClassDB::bind_method(D_METHOD("set_hit_sound", "_hit_sound"), &FuncBreak::set_hit_sound);
	ClassDB::bind_method(D_METHOD("get_hit_sound"), &FuncBreak::get_hit_sound);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "hit_sound"), "set_hit_sound", "get_hit_sound");
	// break sound
	ClassDB::bind_method(D_METHOD("set_break_sound", "_break_sound"), &FuncBreak::set_break_sound);
	ClassDB::bind_method(D_METHOD("get_break_sound"), &FuncBreak::get_break_sound);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "break_sound"), "set_break_sound", "get_break_sound");

	// METHODS
	ClassDB::bind_method(D_METHOD("reparent_entity"), &FuncBreak::reparent_entity);
	ClassDB::bind_method(D_METHOD("update_children"), &FuncBreak::update_children);
	ClassDB::bind_method(D_METHOD("break_geo"), &FuncBreak::break_geo);
	ClassDB::bind_method(D_METHOD("damage", "inflictor", "attacker", "damage"), &FuncBreak::damage);
	ClassDB::bind_method(D_METHOD("bleed", "hit_xform", "amount"), &FuncBreak::bleed);
	ClassDB::bind_method(D_METHOD("use", "activator"), &FuncBreak::use);
	ClassDB::bind_method(D_METHOD("data_save"), &FuncBreak::data_save);
	ClassDB::bind_method(D_METHOD("data_load", "data"), &FuncBreak::data_load);
}

void FuncBreak::set_properties(Dictionary p) {
	properties = p;
	if (!Engine::get_singleton()->is_editor_hint())
		return;
	if (properties.has("appearance_flags"))
		appearance_flags = (uint8_t)(int)properties["appearance_flags"];
	if (properties.has("flags"))
		flags = (uint32_t)(int)properties["flags"];
	if (properties.has("grav_type"))
		grav_type = (uint8_t)(int)properties["grav_type"];
	if (properties.has("health"))
		health = (int16_t)(int)properties["health"];
	if (properties.has("bleed_type"))
		bleed_type = (uint8_t)(int)properties["bleed_type"];
	if (properties.has("targetname"))
		targetname = (String)properties["targetname"];
	if (properties.has("target"))
		target = (String)properties["target"];
	if (properties.has("targetfunc"))
		targetfunc = (String)properties["targetfunc"];
	if (properties.has("message"))
		message = String(properties["message"]).replace("  ", " ");
	if (properties.has("hitsound"))
		hit_sound = (String)properties["hitsound"];
	if (properties.has("sound"))
		break_sound = (String)properties["sound"];
	if (properties.has("moveparent"))
		call_deferred("reparent_entity");
	call_deferred("update_children");
}

Dictionary FuncBreak::get_properties() { return properties; }
void FuncBreak::set_appearance_flags(int f) { appearance_flags = (uint8_t)f; } int FuncBreak::get_appearance_flags() { return (int)appearance_flags; }
void FuncBreak::set_flags(int f) { flags = f; } int FuncBreak::get_flags() { return flags; }
void FuncBreak::set_grav_type(int g) { grav_type = g; } int FuncBreak::get_grav_type() { return grav_type; }
void FuncBreak::set_health(int h) { health = h; } int FuncBreak::get_health() { return health; }
void FuncBreak::set_bleed_type(int b) { bleed_type = (uint8_t)b; } int FuncBreak::get_bleed_type() { return (int)bleed_type; }
void FuncBreak::set_targetname(String t) { targetname = t; } String FuncBreak::get_targetname() { return targetname; }
void FuncBreak::set_target(String t) { target = t; } String FuncBreak::get_target() { return target; }
void FuncBreak::set_targetfunc(String f) { targetfunc = f; } String FuncBreak::get_targetfunc() { return targetfunc; }
void FuncBreak::set_message(String m) { message = m; } String FuncBreak::get_message() { return message; }
void FuncBreak::set_hit_sound(String path) { hit_sound = path; } String FuncBreak::get_hit_sound() { return hit_sound; }
void FuncBreak::set_break_sound(String path) { break_sound = path; } String FuncBreak::get_break_sound() { return break_sound; }

void FuncBreak::reparent_entity() {
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
			for (int i = 0; i < get_child_count(); i++)	{
				Node* n = get_child(i);
				n->set_owner(get_owner());
				if (n->get_class() == "MeshInstance3D")
					cast_to<MeshInstance3D>(n)->set_ignore_occlusion_culling(true);
			}
		}
}

void FuncBreak::update_children() {
	MeshInstance3D* m;
	float volume = 1.0f;
	StaticBody3D* body = nullptr;
	if (!has_node("break_body")) {
		body = memnew(StaticBody3D);
		body->set_collision_mask(0);
		body->set_collision_layer(MAP_LAYER);
		add_child(body);
		body->set_name("break_body");
		body->set_owner(get_owner());
	}
	for (int i = 0; i < get_child_count(); i++) {
		if (get_child(i)->get_class() == "CollisionShape3D" && body != nullptr) {
			CollisionShape3D* c = cast_to<CollisionShape3D>(get_child(i)->duplicate());
			c->set_shape(c->get_shape()->duplicate());
			c->get_shape()->set_margin(0.01f);
			body->add_child(c);
			c->set_owner(body->get_owner());
			cast_to<CollisionShape3D>(get_child(i))->get_shape()->set_margin(0.07f);
		}
		else if (get_child(i)->get_class() == "MeshInstance3D") {
			MeshInstance3D* m = cast_to<MeshInstance3D>(get_child(i));
			m->set_name("meshmesh");
			m->set_layer_mask(CULL_DYNAMIC);
			m->set_ignore_occlusion_culling(true);
			m->set_gi_mode(GeometryInstance3D::GI_MODE_STATIC);
			if (properties.has("shadowcast"))
				m->set_cast_shadows_setting(GeometryInstance3D::ShadowCastingSetting((int)properties["shadowcast"]));
			else
				m->set_cast_shadows_setting(GeometryInstance3D::SHADOW_CASTING_SETTING_ON);
			if (m->get_mesh()->get_class() == "ArrayMesh")
				Ref<ArrayMesh>(m->get_mesh())->lightmap_unwrap(Transform3D(), INVERSE_SCALE);
			volume = fmaxf(m->get_mesh()->get_aabb().get_volume() * 0.5f, 3.0f);
		}
	}
	// Audio
	if (hit_sound == "" && break_sound == "" && has_node("sfx")) {
		Node* s = get_node<AudioStreamPlayer3D>("sfx");
		s->set_name("remove");
		s->queue_free();
	}
	else if (hit_sound != "" && break_sound != "" && !has_node("sfx")) {
		sfx = memnew(AudioStreamPlayer3D);
		sfx->set_bus("SfxWorld");
		sfx->set_attenuation_filter_cutoff_hz(10000);
		sfx->set_unit_size(fminf(volume, 10.6f));
		sfx->set_max_distance(fminf(volume * 3.0f, 32.0f));
		add_child(sfx);
		sfx->set_name("sfx");
		sfx->set_owner(get_owner());
	}
}

void FuncBreak::break_geo() {
	health = 0;
	hide();
	set_collision_layer(0);
	set_monitorable(false);
	if (has_node("break_body"))
		get_node<StaticBody3D>("break_body")->set_collision_layer(0);
	MeshInstance3D* m = get_node<MeshInstance3D>("meshmesh");
	AABB aabb = m->get_aabb();
	int gib_ct = (int)Math::clamp(aabb.get_volume(), 8.0f, 64.0f);
	for (int i = 0; i < gib_ct; i++)
	{
		Gib* gib = cast_to<Gib>(GAME->get_break_gib());
		MeshInstance3D* m2 = gib->get_node<MeshInstance3D>("mesh");
		m2->set_scale(m2->get_scale() * Math::clamp(aabb.get_volume() * 0.25f, 1.0f, 2.0f));
		m2->set_material_override(m->get_mesh()->surface_get_material(rand() % m->get_mesh()->get_surface_count()));
		gib->power = 35.0f;
		get_owner()->add_child(gib);
		gib->set_transform(Transform3D(m->get_transform().basis, m->to_global(GAME->rand_vec3() * 0.5f * aabb.get_volume())));
		gib->erase_ct = 15.0f + GameManager::randf() * 5.0f;
	};
	GAME->use_targets(this, target);
}

void FuncBreak::damage(int amount, Node* attack, NodePath attacker) {
	if (health > 0) {
		health -= amount;
		if (health <= 0){
			break_geo();
			if (!s_break.is_null())
				sfx->set_stream(s_break);
		}
		sfx->play();
	}
}

void FuncBreak::bleed(Transform3D hit_xform, int amount) {
	Node3D* fx;
	if (bleed_type > 0)
		fx = GAME->get_bleed(bleed_type - 1);
	//else
	//	fx = WPN->get_impact(WeaponManager::IM_GRAY);
	get_parent()->add_child(fx);
	fx->set_transform(hit_xform);
}

void FuncBreak::use(Node* activator) { 
	break_geo();
	if (!s_break.is_null())
		sfx->set_stream(s_break);
	sfx->play();
}

Dictionary FuncBreak::data_save() {
	Dictionary data;
	data["broken"] = !is_visible();
	data["health"] = health;
	return data;
}

void FuncBreak::data_load(Dictionary data) {
	if ((bool)data["broken"] == true) {
		health = 0;
		hide();
		set_collision_layer(0);
		set_monitorable(false);
		if (has_node("break_body"))
			get_node<StaticBody3D>("break_body")->set_collision_layer(0);
	}
	else
		health = (int16_t)(int)data["health"];
}

FuncBreak::FuncBreak() {
	add_to_group("WORLD");
	add_to_group("FUNC_BREAK");
	add_to_group("SAV");
	set_collision_mask(0);
	set_collision_layer(ACTOR_LAYER);
	set_monitoring(false);
	set_monitorable(true);
}

FuncBreak::~FuncBreak() {}

void FuncBreak::_ready() {
	if (!Engine::get_singleton()->is_editor_hint())	{
		GAME = get_node<GameManager>("/root/GameManager");
		if (GAME->appearance_check(appearance_flags)) {
			queue_free();
			return;
		}
		//WPN = cast_to<WeaponManager>(get_node("/root/WeaponManager"));
		s_hit = GameManager::update_sound(hit_sound);
		s_break = GameManager::update_sound(break_sound);
		if (has_node("sfx")) {
			sfx = get_node<AudioStreamPlayer3D>("sfx");
			sfx->set_stream(s_hit);
		}
		GAME->set_targetname(this, targetname);
	}
}