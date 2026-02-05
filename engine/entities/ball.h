/**
 * @file ball.h
 * @brief The Ball entity: tracks movement and ownership.
 */
#ifndef ENGINE_ENTITIES_BALL_H
#define ENGINE_ENTITIES_BALL_H

#include <core/vec2.h>
#include <core/constants.h>
#include <string.h>
#include <stdlib.h>

/**
 * @struct Ball
 * @brief Represents the soccer ball.
 * If possessor is not NULL, the ball's position is locked to that player.
 */
struct Ball {
    struct Vec2 position;
    struct Vec2 velocity;
    const float radius;
    struct Player* possessor;   /**< Pointer to the player currently holding the ball. NULL if free. */

    int last_team;      /**< Tracks who last touched the ball (1 or 2) for kickoff rules. */
};

/**
 * @brief Helper to initialize a ball on the stack.
 */
static inline struct Ball make_ball(float x, float y) {
    struct Ball b = {
        .position = {x, y},
        .velocity = {0, 0},
        .radius = BALL_RADIUS,
        .possessor  = NULL,
        .last_team = 0
    };
    return b;
}

/**
 * @brief Helper to allocate a ball on the heap.
 */
static inline struct Ball* make_ball_ptr(float x, float y) {
    struct Ball *b = malloc(sizeof(struct Ball));
    if (b) {
        struct Ball temp = make_ball(x, y);
        memcpy(b, &temp, sizeof(struct Ball));
    }
    return b;
}

#endif
