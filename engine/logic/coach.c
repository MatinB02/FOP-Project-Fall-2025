#include "coach.h"
#include "core/constants.h"
#include "entities/ball.h"
#include "entities/team.h"
#include "game/scene.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// Set to false to let the other team use their own logic (if you implement it)
// Set to true to test your logic on both teams
bool coach_both_teams = true;

/* -------------------------------------------------------------------------
 * Logic Functions
 *  TODO 1: You must implement the following functions in Phase 2.
 *        Each player in each team has its own functions.
 *        You can add new functions, but are NOT ALLOWED to remove
 *        the existing functions or change their structure.
 * ------------------------------------------------------------------------- 
 * ⚠️ STUDENT RULES FOR PHASE 2:
 * You are restricted to modifying ONLY specific variables in each function:
 *
 * 1. MOVEMENT FUNCTIONS (movement_logic_X_Y):
 * Allowed: player->velocity
 * Goal:    Determine the direction and speed of movement.
 *
 * 2. SHOOTING FUNCTIONS (shooting_logic_X_Y):
 * Allowed: ball->velocity
 * Goal:    Determine the direction and power of the kick/pass.
 *
 * 3. CHANGE STATE FUNCTIONS (change_state_logic_X_Y):
 * Allowed: player->state
 * Goal:    Switch between IDLE, MOVING, SHOOTING, or INTERCEPTING.
 *
 * NOTE: Directly modifying any other attributes will be flagged as a violation.
 * Thank you for your attention to this matter!
 * ------------------------------------------------------------------------- */

/* Team 1 movement logic */
void movement_logic_1_0(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_1_1(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_1_2(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_1_3(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_1_4(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_1_5(struct Player *self, const struct Scene *scene) { (void)scene; }

/* Team 2 movement logic */
void movement_logic_2_0(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_2_1(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_2_2(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_2_3(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_2_4(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_2_5(struct Player *self, const struct Scene *scene) { (void)scene; }

/* Team 1 shooting logic */
void shooting_logic_1_0(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_1_1(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_1_2(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_1_3(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_1_4(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_1_5(struct Player *self, const struct Scene *scene) { (void)scene; }

/* Team 2 shooting logic */
void shooting_logic_2_0(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_2_1(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_2_2(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_2_3(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_2_4(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_2_5(struct Player *self, const struct Scene *scene) { (void)scene; }

/* Team 1 change_state logic */
void change_state_logic_1_0(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_1_1(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_1_2(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_1_3(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_1_4(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_1_5(struct Player *self, const struct Scene *scene) { (void)scene; }

/* Team 2 change_state logic */
void change_state_logic_2_0(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_2_1(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_2_2(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_2_3(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_2_4(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_2_5(struct Player *self, const struct Scene *scene) { (void)scene; }

/* -------------------------------------------------------------------------
 * Lookup tables for factory
 * ------------------------------------------------------------------------- */
static PlayerLogicFn team1_movement[6] = {
    movement_logic_1_0, movement_logic_1_1, movement_logic_1_2,
    movement_logic_1_3, movement_logic_1_4, movement_logic_1_5
};

static PlayerLogicFn team2_movement[6] = {
    movement_logic_2_0, movement_logic_2_1, movement_logic_2_2,
    movement_logic_2_3, movement_logic_2_4, movement_logic_2_5
};

static PlayerLogicFn team1_shooting[6] = {
    shooting_logic_1_0, shooting_logic_1_1, shooting_logic_1_2,
    shooting_logic_1_3, shooting_logic_1_4, shooting_logic_1_5
};

static PlayerLogicFn team2_shooting[6] = {
    shooting_logic_2_0, shooting_logic_2_1, shooting_logic_2_2,
    shooting_logic_2_3, shooting_logic_2_4, shooting_logic_2_5
};

static PlayerLogicFn team1_change_state[6] = {
    change_state_logic_1_0, change_state_logic_1_1, change_state_logic_1_2,
    change_state_logic_1_3, change_state_logic_1_4, change_state_logic_1_5
};

static PlayerLogicFn team2_change_state[6] = {
    change_state_logic_2_0, change_state_logic_2_1, change_state_logic_2_2,
    change_state_logic_2_3, change_state_logic_2_4, change_state_logic_2_5
};

/* -------------------------------------------------------------------------
 * Factory functions
 * ------------------------------------------------------------------------- */
PlayerLogicFn get_movement_logic(int team, int kit) {
    if (coach_both_teams) return team1_movement[kit];
    return (team == 1) ? team1_movement[kit] : team2_movement[kit];
}

PlayerLogicFn get_shooting_logic(int team, int kit) {
    if (coach_both_teams) return team1_shooting[kit];
    return (team == 1) ? team1_shooting[kit] : team2_shooting[kit];
}

PlayerLogicFn get_change_state_logic(int team, int kit) {
    if (coach_both_teams) return team1_change_state[kit];
    return (team == 1) ? team1_change_state[kit] : team2_change_state[kit];
}

/* -------------------------------------------------------------------------
 * TALENTS
 *  TODO 2: Replace these default values with your desired skill points.
 * ------------------------------------------------------------------------- */
/* Team 1 */
static struct Talents team1_talents[6] = {
    {5, 5, 5, 5},
    {5, 5, 5, 5},
    {5, 5, 5, 5},
    {5, 5, 5, 5},
    {5, 5, 5, 5},
    {5, 5, 5, 5},
};

/* Team 2 */
static struct Talents team2_talents[6] = {
    {5, 5, 5, 5},
    {5, 5, 5, 5},
    {5, 5, 5, 5},
    {5, 5, 5, 5},
    {5, 5, 5, 5},
    {5, 5, 5, 5},
};

struct Talents get_talents(int team, int kit) {
    if (coach_both_teams) return team1_talents[kit];
    return (team == 1) ? team1_talents[kit] : team2_talents[kit];
}


/* -------------------------------------------------------------------------
 * Positioning
 *  TODO 3: Decide players positions at kick-off.
 *        Players must stay on their half, outside the center circle.
 *        Keep in mind that the kick-off team's first player will automatically
 *             be placed at the center of the pitch.
 * ------------------------------------------------------------------------- */
/* Team 1 */
static struct Vec2 team1_positions[6] = {
    {300, CENTER_Y},
    {250, CENTER_Y-150},
    {200, CENTER_Y-75},
    {150, CENTER_Y},
    {200, CENTER_Y+75},
    {250, CENTER_Y+150},
};

/* Team 2 */
static struct Vec2 team2_positions[6] = {
    {750, CENTER_Y},
    {800, CENTER_Y-150},
    {850, CENTER_Y-75},
    {900, CENTER_Y},
    {850, CENTER_Y+75},
    {800, CENTER_Y+150},
};

struct Vec2 get_positions(int team, int kit) {
    return (team == 1) ? team1_positions[kit] : team2_positions[kit];
}
