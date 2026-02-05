/**
 * @file constants.h
 * @brief Global physical and environmental constants.
 * * This file acts as the "rulebook" for the physics and layout of the game.
 * Modifying these values will change how fast players move, how the ball
 * slides, and the size of the playing field.
 */

#ifndef ENGINE_CORE_CONSTANTS_H
#define ENGINE_CORE_CONSTANTS_H

// --- Mathematical Constants ---
#define PI 3.14159265358979323846
#define DEG2RAD(degrees) ((degrees) * (PI / 180.0f))
#define SEED 20 

// --- Entity Physics ---
#define BALL_RADIUS 10.0f
#define PLAYER_RADIUS 16.0f
#define PLAYER_COUNT 6

#define MAX_TALENT_PER_PLAYER 20
#define MAX_TALENT_PER_SKILL 10
#define MAX_PLAYER_VELOCITY 100.0f
#define MAX_BALL_VELOCITY 150.0f

/** * @brief Ball friction coefficient. 
 * Multiplied by velocity every frame; 1.0 is no friction, 0.0 is an immediate stop.
 */
#define FRICTION 0.98f

// --- Pitch & UI Layout ---
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700

#define PITCH_MARGIN     40.0f
#define PITCH_Y          (PITCH_MARGIN * 2)
#define PITCH_X          (PITCH_MARGIN)
#define PITCH_W          (SCREEN_WIDTH - (PITCH_MARGIN * 2))
#define PITCH_H          (SCREEN_HEIGHT - PITCH_Y - PITCH_MARGIN)

#define CENTER_Y         ((PITCH_H / 2) + PITCH_Y)
#define CENTER_X         (SCREEN_WIDTH / 2)

#define GOAL_WIDTH 35
#define GOAL_HEIGHT 120
#define GRASS_STRIPE_COUNT 10

#endif