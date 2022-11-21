/*******************************************************************************
FUNC_MOVE
Moving brush. Primarily used for doors, but can also be used for platforms and
even decorations.
*******************************************************************************/
#pragma once
#include "Godot.hpp"
#include <Math.hpp>"
#include <vector>
#include <File.hpp>
#include <ArrayMesh.hpp>
#include "MeshInstance.hpp"
#include <BoxShape.hpp>
#include "CollisionShape.hpp"
#include "Area.hpp"
#include <KinematicCollision.hpp>
#include "KinematicBody.hpp"
#include "Timer.hpp"
#include "GameManager.h"
#include "Actor.h"

using namespace godot;

class FuncMove : public KinematicBody
{
private:
	GODOT_CLASS(FuncMove, KinematicBody);
protected:
	SoundManager* SND; GameManager* GAME;
	Dictionary properties = {};
	enum { ST_READY, ST_MOVE, ST_WAIT };
	int spawnflags = 0, grav_type = 0, door_state = ST_READY;
	Vector3 move_pos[2], move_rot = Vector3::ZERO, move_scale = Vector3::ONE;
	float speed = 1.0f, wait = -1.0f, wait_ct = 0.0f;
	float move_progress = 0.0f, move_progress_target = 0.0f, crush_ct = 0.0f;
	int loop = 0, locked = 0, crush_damage = 2;
	bool move_paused = false;
	Transform trg_area_t = Transform::IDENTITY;
	String locked_message = "", move_sound = "", locked_sound = "", unlock_sound = "";
	Area* block_area; // block area used for crush detection
	AudioStreamPlayer3D* sfx[2];
	Ref<AudioStream> s_move, s_locked, s_unlock;
public:
	static void _register_methods();
	// Property Setgets
	void set_properties(Dictionary p); Dictionary get_properties();
	void set_spawnflags(int f); int get_spawnflags();
	void set_grav_type(int g); int get_grav_type();
	void set_move_pos(Vector3 p); Vector3 get_move_pos();
	void set_move_rot(Vector3 r); Vector3 get_move_rot();
	void set_move_scale(Vector3 s); Vector3 get_move_scale();
	void set_speed(float s); float get_speed();
	void set_wait(float w); float get_wait();
	void set_loop(int l); int get_loop();
	void set_locked(int l); int get_locked();
	void set_crush_damage(int d); int get_crush_damage();
	void set_locked_message(String m); String get_locked_message();
	void set_move_sound(String s); String get_move_sound();
	void set_locked_sound(String s); String get_locked_sound();
	void set_unlock_sound(String s); String get_unlock_sound();
	void reparent();
	void update_children();
	void remove();
	// Move Methods
	void trigger(Node* caller); void toggle();
	void lock(); void unlock();
	void play_motion(); void reverse_motion(Node* ent = nullptr); void pause_motion(bool set_pause = true);
	void stop(float mt = 0.0f); void stop_at_end(); void stop_at_pos();
	void loop_none(); void loop_pong(); void loop_restart();
	void play_move_sound(); void stop_move_sound();
	Vector3 get_rot_delta();
	void state_physics(float delta);
	// Save Data
	Dictionary data_save();
	void data_load(Dictionary data);
	// Base Processing
	void _init();
	void _ready();
	void _physics_process(float delta);
};


/*******************************************************************************
FUNC_DOOR
Automatically opens when approached.
Don't use with doors that need to open together.
*******************************************************************************/
class FuncDoor : public FuncMove
{
private:
	GODOT_SUBCLASS(FuncDoor, FuncMove);
	Area* trigger_area;
public:
	static void _register_methods();
	void update_children();
	void trigger(Node* caller);
	void reverse_motion(Node* ent = nullptr);
	void state_physics(float delta);
	void _init();
	void _ready();
};
