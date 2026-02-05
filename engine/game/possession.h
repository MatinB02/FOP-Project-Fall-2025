/**
 * @file possession.h
 * @brief Logic for ball ownership and tackling.
 * * This module defines how players interact with the ball. It handles
 * the physics of "picking up" the ball and the RNG logic for "stealing" it.
 */

#ifndef ENGINE_RULES_POSSESSION_H
#define ENGINE_RULES_POSSESSION_H

#include "game/scene.h"
#include "entities/ball.h"
#include "entities/player.h"

/**
 * @brief Resolves a contest for the ball between a player and the current possessor.
 * * This uses a "Weighted Random" roll based on player talents. 
 * If (Player's Defence) > (Possessor's Dribbling), the ball likely changes hands.
 */
void tackle(struct Player* player, struct Ball* ball);

/**
 * @brief Scans the field to see if any free player has touched the ball.
 * * Typically called every frame to bridge the gap between "Physics" and "Possession."
 */
void update_ball_possessor(struct Scene* scene);

#endif