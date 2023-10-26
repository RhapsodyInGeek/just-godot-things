# These functions get called by a few of the func entities, particularly FuncButton and FuncMove.
# I typically put these functions in a GameManager autoload, typically called `GAME`.
# Entities will call `GAME.set_lock(lock_flags)` on their `lock` variable.
# When an entity with a lock has "use" called on it, it uses `GAME.check_lock(activator, self)` to see if it stays locked.

static func set_lock(lock_flags: int, is_button: bool = false)->int:
	# Triggered locks override keys
	if !is_button and (lock_flags & LockFlags.TRIGGER_LOCK):
		return 1
	# Multiple keys can be required for one lock
	var lock: int = 0
	if (lock_flags & LockFlags.SILVER_LOCK):
		lock |= IT_KEY1
	if (lock_flags & LockFlags.GOLD_LOCK):
		lock |= IT_KEY2
	if (lock_flags & LockFlags.EMERALD_LOCK):
		lock |= IT_KEY3
	if (lock_flags & LockFlags.OBSIDIAN_LOCK):
		lock |= IT_KEY4
	if (lock_flags & LockFlags.BLUE_LOCK):
		lock |= IT_BLUEKEY
	return lock;

func check_lock(unlocker_ent: Node, lock_ent: Node)->bool:
	var lock: int = lock_ent.get("locked")
	if lock > 0:
		if unlocker_ent.is_in_group("PLAYER"):
			if (unlocker_ent.items & lock):
				return false;
		var msg: String = lock_ent.get("locked_message")
		if msg != "":
			message(msg)
		else:
			if lock > 1:
				if (lock & IT_BLUEKEY):
					message("$hint_bluekey")
				elif (lock & IT_KEY4):
					message("$hint_obsidiankey")
				elif (lock & IT_KEY3):
					message("$hint_emeraldkey")
				elif (lock & IT_KEY2):
					message("$hint_goldkey")
				elif (lock & IT_KEY1):
					message("$hint_silverkey")
				else:
					message("$hint_open_elsewhere")
				return true;
			message("$hint_open_elsewhere")
		return true
	return false