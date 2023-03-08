/*******************************************************************************
TRIGGER ROOM
Used in culling.Created in Trenchbroom using a single convex brush.
*******************************************************************************/
#include "TriggerCull.h"

void TriggerRoom::_register_methods()
{
	register_property("properties", &TriggerRoom::set_properties, &TriggerRoom::get_properties, {});
}

void TriggerRoom::set_properties(Dictionary p)
{
	properties = p;
	if (!Engine::get_singleton()->is_editor_hint())
		return;
	if (properties.has("targetname"))
	{
		String n = properties["targetname"];
		set_name("Room_" + n);
	}
	else
		set_name("Room_" + String::num(get_name().to_int()) + "*");
	for (int i = 0; i < get_child_count(); i++)
		if (get_child(i)->get_class() == "MeshInstance")
			get_child(i)->set_name(get_name() + "-bound");
	hide();
}

Dictionary TriggerRoom::get_properties() { return properties; }

void TriggerRoom::_init(){}


/*******************************************************************************
TRIGGER PORTAL
Used in Room culling. Created in Trenchbroom by painting a Portal Texture on
one face and Skip textures on all others.
*******************************************************************************/
void TriggerPortal::_register_methods()
{
	register_property("properties", &TriggerPortal::set_properties, &TriggerPortal::get_properties, {});
	register_method("link", &TriggerPortal::link);
}

void TriggerPortal::set_properties(Dictionary p)
{
	properties = p;
	if (!Engine::get_singleton()->is_editor_hint())
		return;
	if (properties.has("target"))
	{
		String t = properties["target"];
		set_name("Portal_" + t + "*1");
		call_deferred("link");
	};
	if (properties.has("one_way"))
	{
		if (properties["one_way"])
			set_two_way(false);
		else
			set_two_way(true);
	};
	for (int i = 0; i < get_child_count(); i++)
	{
		if (get_child(i)->get_class() == "MeshInstance")
		{
			MeshInstance* m = cast_to<MeshInstance>(get_child(i));
			Ref<MeshDataTool> mdt = Ref<MeshDataTool>(MeshDataTool::_new());
			mdt->create_from_surface(m->get_mesh(), 0);
			Vector3 n = mdt->get_face_normal(0);
			Vector3 axis = n.cross(Vector3::FORWARD);
			if (axis.length() < 1.0f)
				axis = n.cross(Vector3::UP);
			if (axis.length() < 1.0f)
				axis = n.cross(Vector3::RIGHT);
			float depth = 0.0f;
			// Save the mesh basis before the next part
			Basis mesh_basis = m->get_global_transform().basis;
			// Orient the portal to the face normal
			Basis b = get_transform().basis;
			if (n != b.y && -n != b.y)
				look_at(to_global(n), get_global_transform().basis.y);
			else if (n != b.z && -n != b.z)
				look_at(to_global(n), get_global_transform().basis.z);
			else if (n != b.x && -n != b.x)
				look_at(to_global(n), get_global_transform().basis.x);
			// Reorient the mesh
			m->set_global_transform(Transform(mesh_basis, m->get_global_translation()));
			// Get the vertex points and convert them from Vector3 to Vector2
			PoolVector2Array pts;
			for (int j = 0; j < mdt->get_vertex_count(); j++)
			{
				Vector3 v = mdt->get_vertex(j);
				v = to_local(v + get_global_translation());
				depth = v.z;
				pts.append(Vector2(v.x, v.y));
			};
			set_points(pts);
			translate(Vector3(0.0f, 0.0f, depth));
		};
		get_child(i)->queue_free();
	};
}

Dictionary TriggerPortal::get_properties() { return properties; }

void TriggerPortal::link()
{
	if (get_parent()->get_class() != "Room")
		if (is_inside_tree())
		{
			String room_target = properties["target"];
			room_target = "Room_" + room_target;
			if (get_parent()->has_node(NodePath(room_target)))
			{
				Node* r = get_parent()->get_node(NodePath(room_target));
				if (r->get_class() == "Room")
				{
					Transform t = get_global_transform();
					get_parent()->remove_child(this);
					r->add_child(this);
					set_global_transform(t);
					set_owner(r->get_owner());
				};
			};
		};
}

void TriggerPortal::_init() {}

/*******************************************************************************
TRIGGER OCCLUDER
Used in occlusion culling. Created in Trenchbroom by painting an Occluder Texture
on one face and Skip textures on all others.
*******************************************************************************/
#include "TriggerCull.h"

void TriggerOccluder::_register_methods()
{
	register_property("properties", &TriggerOccluder::set_properties, &TriggerOccluder::get_properties, {});
	register_method("reparent", &TriggerOccluder::reparent);
}

void TriggerOccluder::set_properties(Dictionary p)
{
	properties = p;
	if (!Engine::get_singleton()->is_editor_hint())
		return;
	for (int i = 0; i < get_child_count(); i++)
	{
		if (get_child(i)->get_class() == "MeshInstance")
		{
			MeshInstance* m = cast_to<MeshInstance>(get_child(i));
			Ref<MeshDataTool> mdt = Ref<MeshDataTool>(MeshDataTool::_new());
			mdt->create_from_surface(m->get_mesh(), 0);
			Vector3 n = mdt->get_face_normal(0);
			Vector3 axis = n.cross(Vector3::FORWARD);
			if (axis.length() < 1.0f)
				axis = n.cross(Vector3::UP);
			if (axis.length() < 1.0f)
				axis = n.cross(Vector3::RIGHT);
			float depth = 0.0f;
			// Save the mesh basis before the next part
			Basis mesh_basis = m->get_global_transform().basis;
			// Orient the occluder to the face normal
			Basis b = get_transform().basis;
			if (n != b.y && -n != b.y)
				look_at(to_global(n), get_global_transform().basis.y);
			else if (n != b.z && -n != b.z)
				look_at(to_global(n), get_global_transform().basis.z);
			else if (n != b.x && -n != b.x)
				look_at(to_global(n), get_global_transform().basis.x);
			// Reorient the mesh
			m->set_global_transform(Transform(mesh_basis, m->get_global_translation()));
			// Get the vertex points and convert them from Vector3 to Vector2
			PoolVector2Array pts;
			for (int j = 0; j < mdt->get_vertex_count(); j++)
			{
				Vector3 v = mdt->get_vertex(j);
				v = to_local(v + get_global_translation());
				depth = v.z;
				pts.append(Vector2(v.x, v.y));
			};
			Ref<OccluderShapePolygon> shape = Ref<OccluderShapePolygon>(OccluderShapePolygon::_new());
			shape->set_polygon_points(pts);
			if (properties.has("two_way"))
				shape->set_two_way((bool)properties["two_way"]);
			set_shape(shape);
			translate(Vector3(0.0f, 0.0f, depth));
		};
		get_child(i)->queue_free();
	};
	if (properties.has("moveparent"))
		call_deferred("reparent");
}

Dictionary TriggerOccluder::get_properties() { return properties; }

void TriggerOccluder::reparent()
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
		};
}

void TriggerOccluder::_init() {}
