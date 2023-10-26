// HEADER
static Ref<AudioStream> update_sound(String path);

// BODY
Ref<AudioStream> GameManager::update_sound(String path) {
    String snd_path = "sounds/" + path;
    Ref<AudioStream> s;
    if (snd_path.rfind(".wav") > -1 || snd_path.rfind(".ogg") > -1) {
        Ref<FileAccess> file = nullptr;
        if (file->file_exists("user://" + snd_path)) // For modding
            s = ResourceLoader::get_singleton()->load("user://" + snd_path);
        else if (file->file_exists("res://" + snd_path) || file->file_exists("res://" + snd_path + ".import"))
            s = ResourceLoader::get_singleton()->load("res://" + snd_path);
    }
    return s;
}