/**
 * @file scene.h
 * @brief The Game World container.
 * * The Scene struct holds everything: the players, the ball, the clock, and 
 * the current match state (Running, Goal, etc.).
 */

#ifndef ENGINE_GRAPHICS_SCENE_H
#define ENGINE_GRAPHICS_SCENE_H

#include <entities/field.h>

/**
 * @enum GameState
 * @brief Controls the global "Flow" of the match.
 */
typedef enum {
    STATE_RUNNING,      /**< Normal gameplay where AI and physics are active. */
    STATE_GOAL,         /**< Momentary pause after a score. */
    STATE_OUT,          /**< Ball left the pitch; waiting for reset. */
    STATE_TIMEOUT,      /**< Match finished. */
    STATE_RESTARTING    /**< Transition state while players move to kickoff positions. */
} GameState;

typedef struct Scene {
    struct Team* first_team;
    struct Team* second_team;
    struct Ball* ball;
    Field field;
    GameState state;
    float wait_time;        /**< Secondary timer for "celebration" or "reset" delays. */
    float remaining_time;   /**< The main match countdown. */
} Scene;

void init_scene(Scene* scene);
void update_and_verify_scene_states(Scene* scene, const float dt);
void set_piece_out(Scene* scene);
void set_piece_goal(Scene* scene);

#endif /* ENGINE_GRAPHICS_SCENE_H */
