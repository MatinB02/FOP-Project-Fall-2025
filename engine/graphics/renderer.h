/**
 * @file renderer.h
 * @brief The Visual Engine (SDL2 Wrapper).
 * * This module is responsible for taking the "Data" in a Scene and 
 * putting it on the screen. It doesn't care about game rules, only pixels.
 */
#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game/scene.h"
#include "core/constants.h"

/**
 * @struct Renderer
 * @brief Holds the window handle and hardware-accelerated drawing context.
 */
struct Renderer {
    SDL_Window* window;
    SDL_Renderer* sdl_renderer;
    TTF_Font* font;
    SDL_Texture* red_icons[PLAYER_COUNT];
    SDL_Texture* blue_icons[PLAYER_COUNT];
};

/**
 * @brief Clears the screen and draws every entity in the Scene.
 */
void renderer_draw_scene(struct Renderer* r, const struct Scene* scene);

/**
 * @brief The core "Update" function called by the Main Loop.
 * * @param dt Delta Time: the time (in seconds) passed since the last frame. 
 * This ensures the game runs at the same speed regardless of FPS.
 */
void update_scene(struct Scene* scene, float dt);


int renderer_init(struct Renderer* r);
void renderer_destroy(struct Renderer* r);

#endif
