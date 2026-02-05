#include "possession.h"
#include "entities/team.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

/**
 * @brief Internal helper to check if a player's circular hitbox overlaps the ball's.
 * @return 1 if colliding, 0 otherwise.
 */
static int is_colliding(const struct Player* p, const struct Ball* b) {
    // Standard Circle-to-Circle collision math: (dist^2 <= combined_radius^2)
    float dx = p->position.x - b->position.x;
    float dy = p->position.y - b->position.y;
    float dist_sq = dx * dx + dy * dy;
    float radius_sum = p->radius + b->radius;
    return dist_sq <= radius_sum * radius_sum;
}

/**
 * @brief Resolves a tackle attempt on the ball by a player.
 *
 * If the ball is unpossessed, the player automatically gains control.
 * Otherwise, the function compares the defender's defence skill against
 * the current possessor's dribbling skill and uses a weighted random roll
 * to determine if the tackle is successful.
 */
void tackle(struct Player* player, struct Ball* ball) {
    if (!ball->possessor) {
        ball->possessor = player;
        ball->velocity.x = player->velocity.x;
        ball->velocity.y = player->velocity.y;
        return;
    }

    int defence_score = player->talents.defence;
    int dribble_score = ball->possessor->talents.dribbling;
    int sum = defence_score + dribble_score;

    srand((unsigned int)time(NULL));
    int random_roll = rand() % sum;

    if (random_roll < defence_score) {
        ball->possessor = player;
        ball->velocity.x = player->velocity.x;
        ball->velocity.y = player->velocity.y;
    }
}

/**
 * @brief Updates which player currently possesses the ball.
 *
 * Iterates through all players on both teams and checks if any player
 * in the INTERCEPTING state is colliding with the ball.  
 * If so, calls `tackle()` to potentially transfer possession.
 */
void update_ball_possessor(struct Scene* scene) {
    struct Ball* ball = scene->ball;

    for (int i = 0; i < PLAYER_COUNT; i++) {
        struct Player* p1 = scene->first_team->players[i];
        struct Player* p2 = scene->second_team->players[i];

        if (p1 && p1->state == INTERCEPTING && is_colliding(p1, ball))
            tackle(p1, ball);

        if (p2 && p1->state == INTERCEPTING && is_colliding(p2, ball))
            tackle(p2, ball);
    }
}
