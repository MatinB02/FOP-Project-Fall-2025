#include "scene.h"
#include "game/possession.h"
#include <entities/ball.h>
#include <entities/team.h>
#include <logic/coach.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Initializes the game scene, including teams, players, and the ball.
 * @param scene Pointer to the Scene to initialize.
 */
void init_scene(struct Scene *scene) {
    scene->remaining_time = 120.0f; // 2 minutes game
    scene->wait_time = 0.0f;
    scene->first_team = make_team_ptr();
    scene->second_team = make_team_ptr();

    // create players
    for (int i = 0; i < PLAYER_COUNT; i++) {
        scene->first_team->players[i] = make_player_ptr((float)(50 + i * 50), 300, 1, i);
        scene->second_team->players[i] = make_player_ptr((float)(700 - i * 40), 300, 2, i);
    }

    // initialize ball
    scene->ball->position.x = CENTER_X + (rand() % 2) * 2 - 1;  // gives -1 or +1, randomly selecting starter team
    scene->ball->position.y = CENTER_Y;
    set_piece_goal(scene);
    scene->state = STATE_RESTARTING;
}

/**
 * @brief Updates the states of both teams in the scene.
 * @param scene Pointer to the Scene to update.
 */
void update_and_verify_scene_states(struct Scene *scene, const float dt) {
    update_team(scene, scene->first_team);
    update_team(scene, scene->second_team);
    update_ball_possessor(scene);
    
    for (int i = 0; i < PLAYER_COUNT; i++) {
        struct Player* p1 = scene->first_team->players[i];
        struct Player* p2 = scene->second_team->players[i];

        // move players
        p1->position.x += p1->velocity.x * dt;
        p1->position.y += p1->velocity.y * dt;
        p2->position.x += p2->velocity.x * dt;
        p2->position.y += p2->velocity.y * dt;
        // make sure no one walks off the pitch
        if (p1->position.x < p1->radius) p1->position.x = p1->radius;
        if (p1->position.x > SCREEN_WIDTH - p1->radius) p1->position.x = SCREEN_WIDTH - p1->radius;
        if (p1->position.y < p1->radius) p1->position.y = p1->radius;
        if (p1->position.y > SCREEN_HEIGHT - p1->radius) p1->position.y = SCREEN_HEIGHT - p1->radius;
        if (p2->position.x < p2->radius) p2->position.x = p2->radius;
        if (p2->position.x > SCREEN_WIDTH - p2->radius) p2->position.x = SCREEN_WIDTH - p2->radius;
        if (p2->position.y < p2->radius) p2->position.y = p2->radius;
        if (p2->position.y > SCREEN_HEIGHT - p2->radius) p2->position.y = SCREEN_HEIGHT - p2->radius;
    }

    struct Ball* ball = scene->ball;
    if (ball->possessor != NULL)
        ball->last_team = ball->possessor->team;
    ball->position.x += ball->velocity.x * dt;
    ball->position.y += ball->velocity.y * dt;
    ball->velocity.x *= FRICTION;
    ball->velocity.y *= FRICTION;
    // finally the ball stops
    if (hypotf(ball->velocity.x, ball->velocity.y) < 10.0f) {
        ball->velocity.x = 0;
        ball->velocity.y = 0;
    }
    // ball bounces off the window edges 
    if (ball->position.x - ball->radius < 0) {
        ball->position.x = ball->radius;
        ball->velocity.x = -ball->velocity.x;
    }
    if (ball->position.x + ball->radius > SCREEN_WIDTH) {
        ball->position.x = SCREEN_WIDTH - ball->radius;
        ball->velocity.x = -ball->velocity.x;
    }
    if (ball->position.y - ball->radius < 0) {
        ball->position.y = ball->radius;
        ball->velocity.y = -ball->velocity.y;
    }
    if (ball->position.y + ball->radius > SCREEN_HEIGHT) {
        ball->position.y = SCREEN_HEIGHT - ball->radius;
        ball->velocity.y = -ball->velocity.y;
    }
}

/**
 * @brief Stops ball and players movements.
 */
void stop_movements(struct Scene* scene) {
    // Stop the ball
    scene->ball->velocity.x = 0.0f;
    scene->ball->velocity.y = 0.0f;

    // Stop all players from both teams
    for (int i = 0; i < PLAYER_COUNT; i++) {
        if (scene->first_team->players[i]) {
            scene->first_team->players[i]->velocity.x = 0.0f;
            scene->first_team->players[i]->velocity.y = 0.0f;
        }
        if (scene->second_team->players[i]) {
            scene->second_team->players[i]->velocity.x = 0.0f;
            scene->second_team->players[i]->velocity.y = 0.0f;
        }
    }
}

/**
 * @brief Places ball and players after out or corner.
 */
void set_piece_out(struct Scene* scene) {

    stop_movements(scene);

    struct Ball* ball = scene->ball;
    if (ball->last_team == 0) {
        printf("it's not clear which team throw the ball out!\n");
        printf("let's assume it was the first team.\n");
        ball->last_team = 1;
    }
    int last_team = ball->last_team;

    // handle corners
    float x = ball->position.x;
    float y = ball->position.y;

    float left_line   = PITCH_X;
    float right_line  = (PITCH_X + PITCH_W);
    float top_line    = PITCH_Y;
    float bottom_line = (PITCH_Y + PITCH_H);

    bool past_left   = (x + BALL_RADIUS < left_line);
    bool past_right  = (x - BALL_RADIUS > right_line);
    bool past_top    = (y + BALL_RADIUS < top_line);
    bool past_bottom = (y - BALL_RADIUS > bottom_line);
    if (past_left) {
        if (last_team == 2) { // goal kick
            ball->position.y = CENTER_Y;
            ball->position.x = left_line + PITCH_MARGIN;
        } else { // corner
            ball->position.x = left_line;
            if (y > CENTER_Y) // put the ball at the higher corner
                ball->position.y = bottom_line;
            else // put the ball at the lower corner
                ball->position.y = top_line;
        }
    } else if (past_right) { // similar to past_left
        if (last_team == 1) {
            ball->position.y = CENTER_Y;
            ball->position.x = right_line - PITCH_MARGIN;
        } else {
            ball->position.x = right_line;
            if (y > CENTER_Y)
                ball->position.y = bottom_line;
            else
                ball->position.y = top_line;
        }
    } else if (past_bottom) {
        ball->position.y = bottom_line;
    } else if (past_top) {
        ball->position.y = top_line;
    }

    // Find the closest player to the ball to take the throw-in/kick-in
    struct Player* kicker = NULL;
    float min_dist = 999999.0f;
    struct Player* p = NULL;
    if (last_team == 2) {
        for(int i=0; i < PLAYER_COUNT; i++) {
            p = scene->first_team->players[i];
            float d = hypotf(p->position.x - ball->position.x, p->position.y - ball->position.y);
            if (d < min_dist) { min_dist = d; kicker = p; }
        }
    } else {
        for(int i=0; i < PLAYER_COUNT; i++) {
            p = scene->second_team->players[i];
            float d = hypotf(p->position.x - ball->position.x, p->position.y - ball->position.y);
            if (d < min_dist) { min_dist = d; kicker = p; }
        }
    }

    if (!kicker) {
        printf("couln't select a player throw-in the ball!\n");
        return;
    }
    ball->possessor = kicker;
    ball->last_team = kicker->team;
    // Position the player slightly "behind" the ball relative to the pitch center
    float dir_x = (CENTER_X) - ball->position.x;
    float dir_y = (CENTER_Y) - ball->position.y;
    float length = hypotf(dir_x, dir_y);
    
    // Normalize and push player 30 units away from center, behind the ball
    kicker->position.x = ball->position.x - (dir_x / length) * 15.0f;
    kicker->position.y = ball->position.y - (dir_y / length) * 15.0f;

    ball->position.x += (dir_x / length) * 5.0f;
    ball->position.y += (dir_y / length) * 5.0f;

    return;
}

/**
 * @brief Resets ball and players to kickoff positions after a goal.
 */
void set_piece_goal(struct Scene* scene) {
    // Freeze everyone
    stop_movements(scene);

    struct Ball* ball = scene->ball;
    // Identify who kicks off
    struct Team* kickoff_team = (ball->position.x < CENTER_X) ? scene->first_team : scene->second_team;
    struct Team* waiting_team = (ball->position.x > CENTER_X) ? scene->first_team : scene->second_team;

    ball->position.x = (float)CENTER_X;
    ball->position.y = (float)CENTER_Y;
    ball->possessor = NULL;

    const float circle_radius = 90.0f;
    const float padding = 20.0f; // Extra space to ensure they are outside the line

    // Position Kickoff Team
    for (int i = 0; i < PLAYER_COUNT; i++) {
        struct Player* p = kickoff_team->players[i];
        if (!p) continue;

        if (i == 0) {
            // The designated kicker: Place them just behind the ball
            // Direction depends on which side they are attacking
            float side_multiplier = (kickoff_team == scene->first_team) ? 1.0f : -1.0f;
            p->position.x = (float)CENTER_X + (side_multiplier * 15.0f);
            p->position.y = (float)CENTER_Y;
            ball->possessor = p;
            ball->last_team = p->team;
        } else {
            // Others stay on their half, outside the center circle
            Vec2 position = get_positions(p->team, p->kit);
            p->position.x = position.x;
            p->position.y = position.y;
        }
    }

    // Position Waiting Team
    for (int i = 0; i < PLAYER_COUNT; i++) {
        struct Player* p = waiting_team->players[i];
        if (!p) continue;

        Vec2 position = get_positions(p->team, p->kit);
        p->position.x = position.x;
        p->position.y = position.y;
    }

    printf("Team %d is about to kick-off\n", (kickoff_team == scene->first_team ? 1 : 2));
}
