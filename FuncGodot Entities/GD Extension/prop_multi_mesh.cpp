/*************************************************************************
PROP MULTIMESH

Dynamic multimesh prop generator. Prop models are auto generated based upon
a customizable mesh resource path.

Target should be a world entity with a globalname, eg: func_geo, func_move.
**************************************************************************/
#include "prop_multi_mesh.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include "../util/mesh_face.h"
#include <godot_cpp/templates/rb_map.hpp>

void PropMultiMesh::_bind_methods() {
	// populate_multi_mesh
	ClassDB::bind_method(D_METHOD("set_populate_multi_mesh", "populate_multi_mesh"), &PropMultiMesh::set_populate_multi_mesh);
	ClassDB::bind_method(D_METHOD("get_populate_multi_mesh"), &PropMultiMesh::get_populate_multi_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "populate_multi_mesh"), "set_populate_multi_mesh", "get_populate_multi_mesh");
	// target
	ClassDB::bind_method(D_METHOD("set_target", "target"), &PropMultiMesh::set_target);
	ClassDB::bind_method(D_METHOD("get_target"), &PropMultiMesh::get_target);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "target"), "set_target", "get_target");
	// targetname
	/*ClassDB::bind_method(D_METHOD("set_targetname", "targetname"), &PropMultiMesh::set_targetname);
	ClassDB::bind_method(D_METHOD("get_targetname"), &PropMultiMesh::get_targetname);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "targetname"), "set_targetname", "get_targetname");*/
	// target_mesh
	ClassDB::bind_method(D_METHOD("set_target_mesh", "target_mesh"), &PropMultiMesh::set_target_mesh);
	ClassDB::bind_method(D_METHOD("get_target_mesh"), &PropMultiMesh::get_target_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "target_mesh"), "set_target_mesh", "get_target_mesh");
	// mesh_res_path
	ClassDB::bind_method(D_METHOD("set_mesh_res_path", "mesh_res_path"), &PropMultiMesh::set_mesh_res_path);
	ClassDB::bind_method(D_METHOD("get_mesh_res_path"), &PropMultiMesh::get_mesh_res_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "mesh_res_path"), "set_mesh_res_path", "get_mesh_res_path");
	// amount
	ClassDB::bind_method(D_METHOD("set_amount", "amount"), &PropMultiMesh::set_amount);
	ClassDB::bind_method(D_METHOD("get_amount"), &PropMultiMesh::get_amount);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "amount"), "set_amount", "get_amount");
	// random_rotation
	ClassDB::bind_method(D_METHOD("set_random_rotation", "random_rotation"), &PropMultiMesh::set_random_rotation);
	ClassDB::bind_method(D_METHOD("get_random_rotation"), &PropMultiMesh::get_random_rotation);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "random_rotation"), "set_random_rotation", "get_random_rotation");
	// random_tilt
	ClassDB::bind_method(D_METHOD("set_random_tilt", "random_tilt"), &PropMultiMesh::set_random_tilt);
	ClassDB::bind_method(D_METHOD("get_random_tilt"), &PropMultiMesh::get_random_tilt);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "random_tilt"), "set_random_tilt", "get_random_tilt");
	// base_scale
	ClassDB::bind_method(D_METHOD("set_base_scale", "base_scale"), &PropMultiMesh::set_base_scale);
	ClassDB::bind_method(D_METHOD("get_base_scale"), &PropMultiMesh::get_base_scale);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "base_scale"), "set_base_scale", "get_base_scale");
	// random_scale
	ClassDB::bind_method(D_METHOD("set_random_scale", "random_scale"), &PropMultiMesh::set_random_scale);
	ClassDB::bind_method(D_METHOD("get_random_scale"), &PropMultiMesh::get_random_scale);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "random_scale"), "set_random_scale", "get_random_scale");

	ClassDB::bind_method(D_METHOD("_func_godot_apply_properties", "props"), &PropMultiMesh::_func_godot_apply_properties);
	ClassDB::bind_method(D_METHOD("_func_godot_build_complete"), &PropMultiMesh::_func_godot_build_complete);
}

void PropMultiMesh::set_populate_multi_mesh(bool populate) { _populate_multi_mesh(); } bool PropMultiMesh::get_populate_multi_mesh() { return false; }
void PropMultiMesh::set_target(String t) { target = t; } String PropMultiMesh::get_target() { return target; }
//void PropMultiMesh::set_targetname(String t) { targetname = t; } String PropMultiMesh::get_targetname() { return targetname; }
void PropMultiMesh::set_target_mesh(String t) { target_mesh = t; } String PropMultiMesh::get_target_mesh() { return target_mesh; }
void PropMultiMesh::set_mesh_res_path(String p) { mesh_res_path = p; } String PropMultiMesh::get_mesh_res_path() { return mesh_res_path; }
void PropMultiMesh::set_amount(int a) { amount = Math::max(a, 0); } int PropMultiMesh::get_amount() { return Math::max(amount, 0); }
void PropMultiMesh::set_random_rotation(float r) { random_rotation = abs(r); } float PropMultiMesh::get_random_rotation() { return abs(random_rotation); }
void PropMultiMesh::set_random_tilt(float t) { random_tilt = t; } float PropMultiMesh::get_random_tilt() { return random_tilt; }
void PropMultiMesh::set_base_scale(float s) { base_scale = Math::max(abs(s), 0.01f); } float PropMultiMesh::get_base_scale() { return Math::max(abs(base_scale), 0.01f); }
void PropMultiMesh::set_random_scale(float s) { random_scale = abs(s); } float PropMultiMesh::get_random_scale() { return abs(random_scale); }

void PropMultiMesh::_populate_multi_mesh() {
	set_multimesh(nullptr);

	// Find surface target
	MeshInstance3D* mesh_targ[2] = { nullptr, nullptr };

	for (int i = 0; i < 2; i++) {
		NodePath targ_path;
		if (i == 0)
			targ_path = String("entity_") + target;
		else
			targ_path = String("entity_") + target_mesh;

		if (get_parent()->has_node(targ_path)) {
			Node* temp = get_parent()->get_node<Node>(targ_path);
			if (temp->get_class() == "MeshInstance3D")
				mesh_targ[i] = cast_to<MeshInstance3D>(temp);
			else if (temp->get_child_count() > 0) {
				TypedArray<Node> temp_children = temp->get_children();
				for (int j = 0; j < temp_children.size(); j++) {
					if (cast_to<Node>(temp_children[j])->get_class() == "MeshInstance3D") {
						mesh_targ[i] = cast_to<MeshInstance3D>(temp_children[j]);
						break;
					}
				}
			}
		}
	}

	// No valid surface target! Find one within range
	if (!mesh_targ[0] || !mesh_targ[0]->get_mesh().is_valid()) {
		TypedArray<Node> world_ents = get_tree()->get_nodes_in_group("WORLD");
		for (int i = 0; i < world_ents.size(); i++) {
			if (cast_to<Node>(world_ents[i])->get_class() == "MeshInstance3D" 
				&& cast_to<MeshInstance3D>(world_ents[i])->get_global_position().distance_to(get_global_position()) < 8.0f) {
				mesh_targ[0] = cast_to<MeshInstance3D>(world_ents[i]);
				break;
			}
			TypedArray<Node> temp_children = cast_to<Node>(world_ents[i])->get_children();
			for (int j = 0; j < temp_children.size(); j++) {
				Node* child = cast_to<Node>(temp_children[j]);
				if (child->get_class() == "MeshInstance3D") {
					if (cast_to<MeshInstance3D>(child)->get_mesh().is_valid() 
						&& cast_to<MeshInstance3D>(child)->get_global_position().distance_to(get_global_position()) < 8.0f) {
						mesh_targ[0] = cast_to<MeshInstance3D>(child);
						break;
					}
				}
			}
			if (mesh_targ[0] != nullptr)
				break;
		}

		if (!mesh_targ[0]) {
			UtilityFunctions::printerr(String("No target found for PropMultiMesh ") + (String)get_name());
			return;
		}
	}

	// Cache the mesh
	Ref<Mesh> mesh;
	if (mesh_targ[1] != nullptr)
		mesh = mesh_targ[1]->get_mesh();
	if (mesh.is_null() && FileAccess::file_exists(mesh_res_path))
		mesh = ResourceLoader::get_singleton()->load(mesh_res_path);
	if (mesh.is_null()) {
		UtilityFunctions::printerr(String("No valid mesh set for PropMultiMesh ") + (String)get_name());
		return;
	}

	// Prepare target surface
	Transform3D geo_xform = this->get_global_transform().affine_inverse() * mesh_targ[0]->get_global_transform();
	PackedVector3Array verts = mesh_targ[0]->get_mesh()->get_faces();
	if (verts.size() == 0 || verts.size() % 3 != 0) {
		UtilityFunctions::printerr(String("PropMultiMesh ") + (String)get_name() + String(" target has invalid mesh"));
		return;
	}

	std::vector<MeshFace> faces;
	faces.resize(verts.size() / 3);
	int facecount = faces.size();
	for (int i = 0; i < facecount; i++) {
		faces[i] = MeshFace(verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]);
		for (int j = 0; j < 3; j++)
			faces[i].vertex[j] = geo_xform.xform(faces[i].vertex[j]);
	}	

	float area_accum = 0.0f;
	RBMap<float, int> triangle_area_map;
	for (int i = 0; i < facecount; i++) {
		float area = faces[i].get_area();
		if (area < CMP_EPSILON)
			continue;
		triangle_area_map[area_accum] = i;
		area_accum += area;
	}

	if (triangle_area_map.is_empty() || area_accum == 0) {
		UtilityFunctions::printerr(String("PropMultiMesh ") + (String)get_name() + String(" couldn't map area"));
		return;
	}

	// Generate MultiMesh resource
	Ref<MultiMesh> multimesh;
	multimesh.instantiate();
	multimesh->set_transform_format(MultiMesh::TRANSFORM_3D);
	multimesh->set_instance_count(amount);
	multimesh->set_mesh(mesh);
	set_multimesh(multimesh);

	Ref<RandomNumberGenerator> rng;
	rng.instantiate();

	// Place instances
	for (int i = 0; i < amount; i++) {
		float areapos = rng->randf_range(0.0f, area_accum);
		RBMap<float, int>::Iterator itr = triangle_area_map.find_closest(areapos);
		if (!itr)
			continue;
		int index = Math::wrapi(itr->value, 0, facecount);

		MeshFace &face = faces[index];

		Vector3 pos = face.get_random_point_inside();
		Vector3 normal = face.get_plane().normal;
		Vector3 op_axis = (face.vertex[0] - face.vertex[1]).normalized();

		Transform3D xform;
		xform.set_look_at(pos, pos + op_axis, normal);

		float rot = Math::deg_to_rad(random_rotation);
		float tilt = Math::deg_to_rad(random_tilt);

		Basis post_xform;
		post_xform.rotate(xform.basis.get_column(1), rng->randf_range(-rot, rot));
		post_xform.rotate(xform.basis.get_column(2), rng->randf_range(-tilt, tilt));
		post_xform.rotate(xform.basis.get_column(0), rng->randf_range(-tilt, tilt));

		xform.basis = post_xform * xform.basis;
		xform.basis *= fmaxf(abs(base_scale + rng->randf_range(-random_scale, random_scale)), 0.01f);

		multimesh->set_instance_transform(i, xform);
	}
}

void PropMultiMesh::_func_godot_apply_properties(Dictionary props) {
	if (props.has("target"))
		target = (String)props["target"];
	if (props.has("target_mesh"))
		target_mesh = (String)props["target_mesh"];
	//if (props.has("targetname"))
		//targetname = (String)props["targetname"];
	if (props.has("mesh_res_path"))
		mesh_res_path = (String)props["mesh_res_path"];
	if (props.has("amount"))
		amount = (int)props["amount"];
	if (props.has("random_rotation"))
		random_rotation = fminf(abs((float)props["random_rotation"]), 180.0f);
	if (props.has("random_tilt"))
		random_tilt = fminf(abs((float)props["random_tilt"]), 180.0f);
	if (props.has("base_scale"))
		base_scale = fmaxf(abs((float)props["base_scale"]), 0.01f);
	if (props.has("random_scale"))
		random_scale = abs((float)props["random_scale"]);
	if (props.has("cast_shadow")) {
		int shadow = (int)props["cast_shadow"];
		set_cast_shadows_setting((GeometryInstance3D::ShadowCastingSetting)shadow);
	}

	_populate_multi_mesh();
	
}

void PropMultiMesh::_func_godot_build_complete() {}

PropMultiMesh::PropMultiMesh() { add_to_group("prop_multi_mesh"); }
PropMultiMesh::~PropMultiMesh() {}