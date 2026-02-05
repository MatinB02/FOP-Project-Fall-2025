/**
 * @file referee.h
 * @brief Game logic validation and rule enforcement.
 * * The Referee module is separate from the Physics engine. While Physics moves
 * objects, the Referee checks if those movements resulted in a Goal, an Out,
 * or an illegal player state (like running too fast).
 */

#ifndef ENGINE_RULES_REFEREE_H
#define ENGINE_RULES_REFEREE_H

#include "game/scene.h"
#include "entities/player.h"
#include <stdbool.h>

/**
 * @enum RefereeCode
 * @brief Signals sent by the referee to the game engine.
 */
enum RefereeCode {
    PLAY_ON = 0, /**< Everything is fine; keep running. */
    GOAL = 1,    /**< Ball entered a net. */
    OUT = 2      /**< Ball left the field boundaries. */
};

/**
 * @brief The main rule-checker called every frame.
 * Checks for goals, out-of-bounds, and ball possession updates.
 */
int referee(struct Scene* scene);

/**
 * @brief Validates that a player's skills are within the allowed budget.
 * Prevents "Super-Players" that break the game balance.
 */
void verify_talents(struct Talents talents);

/**
 * @brief Corrects illegal player states (e.g., if a player tries to shoot 
 * without possessing the ball).
 */
void verify_state(struct Player *player, struct Scene *scene);

/**
 * @brief Enforces speed limits. If a player's velocity exceeds their Agility talent,
 * this function caps it at the maximum allowed.
 */
void verify_movement(struct Player *player);

/**
 * @brief Enforces physics limits on the ball after a kick.
 * * This function ensures that the ball doesn't move faster than the 
 * player's 'Shooting' talent allows. Also checks player passes to its
 * own half at restart kick-off.
 */
void verify_shoot(struct Ball *ball, bool kickoff);

#endif