#include "team.h"
#include "ball.h"
#include "game/scene.h"
#include "logic/referee.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * @brief The Team Update Cycle.
 * * This function is the "brain" for an entire side. It performs two passes:
 * 1. Perception: Every player looks at the scene and decides if they should change state.
 * 2. Action: Every player executes the movement or shooting logic for their current state.
 */
void update_team(struct Scene* scene, struct Team* team) {
    struct Player **players = team->players;
    struct Ball* ball = scene->ball;

    // STEP 1: THINK
    for (int i = 0; i < PLAYER_COUNT; i++)
        if (players[i] && players[i]->change_state_logic) {
            players[i]->change_state_logic(players[i], scene);
            verify_state(players[i], scene);
        }


    // STEP 2: ACT
    for (int i = 0; i < PLAYER_COUNT; i++) {
        if (players[i]) {
            struct Player *player = players[i];
            switch (player->state) {
                case IDLE:
                    break;
                case MOVING:
                    player->movement_logic(player, scene);
                    verify_movement(player);            // Enforce speed limits
                    if (player == ball->possessor) {    // possessor moves the ball
                        ball->velocity.x = player->velocity.x;
                        ball->velocity.y = player->velocity.y;
                    }
                    break;
                case INTERCEPTING:
                    if (player == ball->possessor)      // if you have the ball and you INTERCEPT, you loose it
                        ball->possessor = NULL;
                    break;
                case SHOOTING:
                    player->shooting_logic(player, scene);
                    verify_shoot(ball, false);          // Enforce speed limits
                    ball->possessor = NULL;
                    break;
                default:
                    break;
            }
        }
    }
}

/**
 * @brief Creates a stack-allocated Team instance.
 * @param kit The kit color of the team.
 * @return Initialized Team structure.
 */
struct Team make_team() {
    struct Team t = {
        .score = 0
    };
    return t;
}

/**
 * @brief Creates a heap-allocated Team instance.
 * @param kit The kit color of the team.
 * @return Pointer to a newly allocated Team.
 */
struct Team * make_team_ptr() {
    struct Team* t_ptr = (struct Team*)malloc(sizeof(struct Team));
    if (!t_ptr)
        return NULL;
    *t_ptr = make_team();
    return t_ptr;
}