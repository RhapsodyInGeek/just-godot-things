// HEADER
void use_targets(Node* activator, String targets);
void set_targetname(Node* ent, String targetname);

// BODY
void GameManager::use_targets(Node* activator, String targets) {
	/* Similar setup to Quake, except we can specify multiple targets and a custom target function.
	Targetnames are really Godot Groups, so we can have multiple entities share a common "targetname" in Trenchbroom.
	Additionally, we can comma delimit multiple targetnames to target multiple groups of entities. Neat. */
	Node* tn;
	Array target_nodes;
	PackedStringArray target_names = targets.split(",");
	for (int i = 0; i < target_names.size(); i++) {
		target_nodes = get_tree()->get_nodes_in_group(target_names[i]);
		for (int j = 0; j < target_nodes.size(); j++) {
			tn = cast_to<Node>(target_nodes[j]);
			// Be careful when specifying a function since we can't pass arguments to it (without hackarounds of course)
			if (activator->has_method("get_targetfunc")) {
				String f = activator->call("get_targetfunc");
				if (f != "" && tn->has_method(f)) {
					tn->call(f);
					continue;
				}
			}
			if (tn->has_method("use"))
				tn->call("use", activator);
		}
	}
	/* Because we can trigger multiple entities at once, we get the trigger message from the activator node only,
	to prevent spam / missed messages / unpredictable behavior... */
	if (activator->has_method("get_message")) {
		message(activator->call("get_message"));
	}
}

// Set the targetnames for the entity. We can specify multiple targetnames using comma delimiting.
void GameManager::set_targetname(Node* ent, String targetname) {
	if (ent != nullptr && targetname != "") {
		PackedStringArray t = targetname.split(",");
		for (int i = 0; i < t.size(); i++)
			if (String(t[i]) != "")
				ent->add_to_group(t[i]);
	}
}