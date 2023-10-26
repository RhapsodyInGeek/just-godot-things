/*******************************************************************************
PLAYER_START
Handles spawning in the Player entity. Why not just spawn in the player directly?
Future proofing in case we want to do anything with multiplayer, multiple start positions,
and also to take care of resetting / setting inventory.
*******************************************************************************/
class PlayerStart : public Node3D {
    GDCLASS(PlayerStart, Node3D)
protected:
    static void _bind_methods();
public:
    Dictionary properties = {};
    enum {
        FL_SINGLEPLAYER = (1 << 0), FL_COOP = (1 << 1), FL_DEATHMATCH = (1 << 2), FL_TEAM_DEATHMATCH = (1 << 3),
        FL_RESET_HEALTH = (1 << 4), FL_RESET_WEAPONS = (1 << 5)
    };
    uint8_t appearance_flags = 0;
    uint8_t flags = 1;
    uint8_t index = 0;
    bool can_spawn = false;
    PlayerStart();
    ~PlayerStart();
    void set_properties(Dictionary new_properties); Dictionary get_properties();
    void set_appearance_flags(int f); int get_appearance_flags();
    void set_flags(int f); int get_flags();
    void set_index(int i); int get_index();
    void spawn_player();
    void _ready();
};