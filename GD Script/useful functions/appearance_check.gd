# Appearance Flags check. We check by inclusion of flags, not by omission.
# We don't free the entity in this script because we may need to run extra
# cleanup prior to freeing.

# This method gets added to a Game Manager autoload
func appearance_check(appearance_flags: int)->bool:
	if appearance_flags & AppearanceFlags.NOT_EASY and difficulty == Difficulties.EASY:
		return true
	if appearance_flags & AppearanceFlags.NOT_NORMAL and difficulty == Difficulties.NORMAL:
		return true
	if appearance_flags & AppearanceFlags.NOT_HARD and difficulty == Difficulties.HARD:
		return true
	if appearance_flags & AppearanceFlags.NOT_NIGHTMARE and difficulty == Difficulties.NIGHTMARE:
		return true
	return false

# Then we call this in the ready function of every entity that gets modified by the difficulty
func _ready() -> void:
    if Engine.is_editor_hint():
        return
    
    if GAME.appearance_check(appearance_flags):
        queue_free()
        return
    
    # Other entity onready code...