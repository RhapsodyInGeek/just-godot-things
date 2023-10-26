// HEADER
bool check_lock(Node* actor_ent, Node* lock_ent);
static uint32_t set_lock(uint8_t lock_flags, bool is_button = false);

// BODY
bool GameManager::check_lock(Node* actor_ent, Node* lock_ent) {
	uint32_t lock = lock_ent->get("locked");
	if (lock > 0) {
		if (actor_ent->is_in_group("PLAYER")) {
			if (cast_to<Player>(actor_ent)->items & lock)
				return false;
		}
		String msg = lock_ent->get("locked_message");
		if (msg != "")
			message(msg);
		else {
			if (lock > 1) {
				if (lock & IT_BLUEKEY)
					message("$hint_bluekey");
				else if (lock & IT_KEY4)
					message("$hint_obsidiankey");
				else if (lock & IT_KEY3)
					message("$hint_emeraldkey");
				else if (lock & IT_KEY2)
					message("$hint_goldkey");
				else if (lock & IT_KEY1)
					message("$hint_silverkey");
				else
					message("$hint_open_elsewhere");
				return true;
			}
			message("$hint_open_elsewhere");
		}
		return true;
	}
	return false;
}

uint32_t GameManager::set_lock(uint8_t lock_flags, bool is_button) {
	// Triggered locks override keys
	if (!is_button && (lock_flags & FL_TRIGGER_LOCK))
		return (uint32_t)1;
	// Multiple keys can be required for one lock
	uint32_t lock = 0;
	if (lock_flags & FL_SILVER_LOCK)
		lock |= IT_KEY1;
	if (lock_flags & FL_GOLD_LOCK)
		lock |= IT_KEY2;
	if (lock_flags & FL_EMERALD_LOCK)
		lock |= IT_KEY3;
	if (lock_flags & FL_OBSIDIAN_LOCK)
		lock |= IT_KEY4;
	if (lock_flags & FL_BLUE_LOCK)
		lock |= IT_BLUEKEY;
	return lock;
}