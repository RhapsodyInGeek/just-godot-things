# Add this to your Game Manager autoload and you can use this to load an audio file by string.
# Static so we can call this in the Editor, too.

static func update_sound(path: String)->AudioStream:
	var snd_path: String = "sounds/" + path
	var s: AudioStream = null
	if snd_path.rfind(".wav") > -1 or snd_path.rfind(".ogg") > -1:
		if FileAccess.file_exists("user://" + snd_path): # For modding
			s = load("user://" + snd_path)
		elif FileAccess.file_exists("res://" + snd_path) or FileAccess.file_exists("res://" + snd_path + ".import"):
			s = load("res://" + snd_path)
	return s;