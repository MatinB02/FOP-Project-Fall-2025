/**
 * @file team.h
 * @brief Defines the Team entity and related helper functions.
 */

#ifndef ENGINE_ENTITIES_TEAM_H
#define ENGINE_ENTITIES_TEAM_H

#include "player.h"
#include <core/constants.h>

struct Scene;  /**< Forward declaration of Scene for update functions. */

/**
 * @struct Team
 * @brief Represents a soccer team with players and score.
 */
struct Team {
    unsigned int score;
    struct Player *players[PLAYER_COUNT];
};

/**
 * @brief Creates a new Team (stack-allocated).
 * @param kit The kit color for the team.
 * @return Initialized Team structure.
 */
struct Team make_team();

/**
 * @brief Creates a new Team (heap-allocated).
 * @param kit The kit color for the team.
 * @return Pointer to a newly allocated Team structure.
 */
struct Team *make_team_ptr();

/**
 * @brief Updates the state of all players in the team within the given scene.
 * @param scene Pointer to the game scene.
 * @param team Pointer to the team to update.
 */
void update_team(struct Scene* scene, struct Team* team);

#endif
