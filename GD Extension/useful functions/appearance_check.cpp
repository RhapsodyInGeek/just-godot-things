// HEADER
bool appearance_check(uint8_t appearance_flags);

// BODY
bool GameManager::appearance_check(uint8_t appearance_flags) {
    if (appearance_flags & FL_NOT_DEATHMATCH && gamemode == DEATHMATCH)
        return true;
    else if (appearance_flags & FL_NOT_TEAM_DEATHMATCH && gamemode == TEAM_DEATHMATCH)
        return true;
    else if (appearance_flags & FL_NOT_EASY && difficulty == EASY)
        return true;
    else if (appearance_flags & FL_NOT_NORMAL && difficulty == NORMAL)
        return true;
    else if (appearance_flags & FL_NOT_HARD && difficulty == HARD)
        return true;
    else if (appearance_flags & FL_NOT_NIGHTMARE && difficulty == NIGHTMARE)
        return true;
    return false;
}