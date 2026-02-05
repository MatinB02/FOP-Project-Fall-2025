/**
 * @file player.h
 * @brief Player entity and AI Behavior system.
 * * Each player operates as a "State Machine." Their current 'state' 
 * determines which logic function is executed during the game loop.
 */

#ifndef ENGINE_ENTITIES_PLAYER_H
#define ENGINE_ENTITIES_PLAYER_H

#include <core/vec2.h>

struct Scene; // Forward declaration: The player needs to know the world exists.

/**
 * @enum PlayerActionState
 * @brief The possible "modes" a player can be in.
 */
typedef enum PlayerActionState {
    IDLE,          /**< Stand still. */
    MOVING,        /**< Moving in a direction. */
    SHOOTING,      /**< Shoot or pass the ball. */
    INTERCEPTING   /**< Tackle the ball. */
} PlayerActionState;

/**
 * @struct Talents
 * @brief Skill point that influence success rates and speed.
 * An int in range 1 to MAX_TALENT_PER_SKILL.
 * defence, agility, dribbling, shooting.
 */
struct Talents {
    int defence;    /**< tackle success probability */
    int agility;    /**< how fast the player can move */
    int dribbling;  /**< opponent's tackle failure probability */
    int shooting;   /**< how fast the player can shoot or pass */
};

/**
 * @struct Player
 * @brief The main actor in the game.
 * * This struct uses "Function Pointers" (movement_logic, etc.). 
 * This allows different players to have different "brains" (AI) 
 * while using the same data structure.
 */
typedef struct Player {
    struct Vec2 position;
    struct Vec2 velocity;
    const float radius;
    
    PlayerActionState state;
    const struct Talents talents;
    const int team;   // 1: Red (Left), 2: Blue (Right)
    const int kit;    // Individual player ID (e.g., Jersey number)

    /** AI: Decides HOW to move. */
    void (*movement_logic)(struct Player *self, const struct Scene *scene);
    
    /** AI: Decides WHERE and HOW HARD to kick. */
    void (*shooting_logic)(struct Player *self, const struct Scene *scene);
    
    /** AI: Decides WHEN to switch states (e.g., from IDLE to MOVING). */
    void (*change_state_logic)(struct Player *self, const struct Scene *scene);
} Player;

// Allocation functions
struct Player make_player(float x, float y, int team, const int kit);
struct Player *make_player_ptr(float x, float y, int team, const int kit);

#endif /* ENGINE_ENTITIES_PLAYER_H */
