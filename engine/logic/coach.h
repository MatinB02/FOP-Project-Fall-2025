/**
 * @file coach.h
 * @brief The AI Factory for players.
 * * In this project, "Coach" refers to the system that assigns specific AI behaviors
 * to players. Instead of hardcoding logic inside the Player struct, the Coach 
 * provides function pointers that define how a specific player moves or shoots.
 */

#ifndef ENGINE_ENTITIES_COACH_H
#define ENGINE_ENTITIES_COACH_H

#include "entities/player.h"

/** * @typedef PlayerLogicFn
 * @brief A standard blueprint for all AI behavior functions.
 * Any function following this signature can be assigned to a player.
 */
typedef void (*PlayerLogicFn)(struct Player *self, const struct Scene *scene);

/**
 * @name Logic Factory Functions
 * @brief Use these to retrieve the specific function pointer for a player.
 * @param team 1 (Red) or 2 (Blue).
 * @param kit The player's ID number (0 to PLAYER_COUNT-1).
 */
///@{
PlayerLogicFn get_movement_logic(int team, int kit);
PlayerLogicFn get_shooting_logic(int team, int kit);
PlayerLogicFn get_change_state_logic(int team, int kit);
///@}

/**
 * @brief Returns the skill stats (Talents) for a specific player.
 */
struct Talents get_talents(int team, int kit);

/**
 * @brief Returns the position of the player at kick-off.
 */
struct Vec2 get_positions(int team, int kit);

#endif