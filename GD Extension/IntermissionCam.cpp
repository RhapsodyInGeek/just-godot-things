#include "GameDefs.h"
/*****************************************************
INTERMISSION CAMERA
When the stage ends, one of these is randomly chosen
as the new viewport camera and looks around.
******************************************************/
void IntermissionCam::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_properties", "properties"), &IntermissionCam::set_properties);
	ClassDB::bind_method(D_METHOD("get_properties"), &IntermissionCam::get_properties);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");
}

void IntermissionCam::set_properties(Dictionary p){
	properties = p;
	if (!Engine::get_singleton()->is_editor_hint())
		return;
	set_rotation_degrees(GameManager::demangler(properties));
}

Dictionary IntermissionCam::get_properties() { return properties; }

IntermissionCam::IntermissionCam() {}
IntermissionCam::~IntermissionCam() {}

void IntermissionCam::_ready() {
	if (!Engine::get_singleton()->is_editor_hint()) {
		GAME = get_node<GameManager>(GAME_PATH);
		cam = get_node<Camera3D>("cam");
		cam->add_to_group("INTERMISSION_CAM");
	}
}

void IntermissionCam::_process(float delta) {
	if (!Engine::get_singleton()->is_editor_hint() && cam->is_current()) {
		float t = GAME->time;
		cam->set_rotation_degrees(cam->get_rotation_degrees() + Vector3(sinf(t * 0.5f) * delta, cosf(t * 0.25f) * 2.0f * delta, 0.0f));
	}
}