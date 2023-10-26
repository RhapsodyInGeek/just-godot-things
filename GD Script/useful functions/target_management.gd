# The pair of functions I use for my simple scripting management. I typically put these functions in 
# a GameManager autoload, typically called `GAME`.
# Entities will call `GAME.set_targetname(self, targetname)` to add themselves to target groups.
# When an entity wants to trigger a target, it calls `GAME.use_targets(self, targetname)`.

func use_targets(activator: Node, targets: String)->void:
	# Similar setup to Quake, except we can specify multiple targets and a custom target function.
	# Targetnames are really Godot Groups, so we can have multiple entities share a common "targetname" in Trenchbroom.
	# Additionally, we can comma delimit multiple targetnames to target multiple groups of entities. Neat.
	var target_nodes: Array[Node]
	var target_names: PackedStringArray = targets.split(",")
	for i in target_names.size():
		target_nodes = get_tree().get_nodes_in_group(target_names[i])
		for tn in target_nodes:
			# Be careful when specifying a function since we can't pass arguments to it (without hackarounds of course)
			if "targetfunc" in activator:
				var f: String = activator.targetfunc
				if f != "" and tn.has_method(f):
					tn.call(f)
					continue
			if tn.has_method("use"):
				tn.call("use", activator)
	# Because we can trigger multiple entities at once, we get the trigger message from the caller node only,
	# to prevent spam / missed messages / unpredictable behavior...
	if "message" in activator:
		message(activator.message)


# Set the targetnames for the entity. We can specify multiple targetnames using comma delimiting.
func set_targetname(ent: Node, targetname: String)->void:
	if ent != null and targetname != "":
		for t in targetname.split(","):
			if t != "":
				ent.add_to_group(t)