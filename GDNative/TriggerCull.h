/*******************************************************************************
TRIGGER ROOM
Used in culling.Created in Trenchbroom using a single convex brush.
*******************************************************************************/
#pragma once
#include "Godot.hpp"
#include <ArrayMesh.hpp>
#include <MeshDataTool.hpp>
#include "MeshInstance.hpp"
#include "Room.hpp"
#include "Portal.hpp"
#include <OccluderShapePolygon.hpp>
#include "Occluder.hpp"

using namespace godot;

class TriggerRoom : public Room
{
private:
	GODOT_CLASS(TriggerRoom, Room);
	Dictionary properties;
public:
	static void _register_methods();
	void set_properties(Dictionary p);
	Dictionary get_properties();
	void _init();
};

/*******************************************************************************
TRIGGER PORTAL
Used in Room culling. Created in Trenchbroom by painting a Portal Texture on
one face and Skip textures on all others.
*******************************************************************************/
class TriggerPortal : public Portal
{
private:
	GODOT_CLASS(TriggerPortal, Portal);
	Dictionary properties = {};
public:
	static void _register_methods();
	void set_properties(Dictionary p);
	Dictionary get_properties();
	void link();
	void _init();
};

/*******************************************************************************
TRIGGER OCCLUDER
Used in occlusion culling. Created in Trenchbroom by painting an Occluder Texture
on one face and Skip textures on all others.
*******************************************************************************/
class TriggerOccluder : public Occluder
{
private:
	GODOT_CLASS(TriggerOccluder, Occluder);
	Dictionary properties = {};
public:
	static void _register_methods();
	void set_properties(Dictionary p);
	Dictionary get_properties();
	void reparent();
	void _init();
};
