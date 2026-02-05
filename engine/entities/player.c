#include "player.h"
#include "logic/coach.h"
#include <core/constants.h>
#include <logic/referee.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Creates a stack-allocated Player instance.
 * @param x Initial x-coordinate.
 * @param y Initial y-coordinate.
 * @param talents Player's skill attributes.
 * @return Initialized Player structure.
 */
struct Player make_player(const float x, const float y, const int team, const int kit) {
    struct Player p = {
        .position = {x, y},
        .velocity = {0, 0},
        .radius = PLAYER_RADIUS,
        .talents = get_talents(team, kit),
        .state = IDLE,
        .team = team,
        .kit = kit,

        .movement_logic     = get_movement_logic(team, kit),
        .shooting_logic     = get_shooting_logic(team, kit),
        .change_state_logic = get_change_state_logic(team, kit),
    };
    verify_talents(p.talents);
    return p;
}

/**
 * @brief Creates a heap-allocated Player instance.
 * @param x Initial x-coordinate.
 * @param y Initial y-coordinate.
 * @param talents Player's skill attributes.
 * @return Pointer to newly allocated Player, or NULL on allocation failure.
 */

struct Player *make_player_ptr(const float x, const float y, const int team, const int kit) {
    struct Player *p = malloc(sizeof(struct Player));
    if (!p) return NULL;

    // We create a temporary stack player
    struct Player temp = make_player(x, y, team, kit);

    // We use memcpy to copy the bytes from the temp player to the heap.
    // This ignores the 'const' qualifiers during the copy process.
    memcpy(p, &temp, sizeof(struct Player));

    return p;
}