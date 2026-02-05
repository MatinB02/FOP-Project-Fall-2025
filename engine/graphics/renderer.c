#include "renderer.h"
#include <stdio.h>
#include <math.h>
#include <SDL_image.h>

#include "core/constants.h"
#include "logic/referee.h"
#include "entities/team.h"
#include "entities/ball.h"

// for scoreboard
static void draw_filled_rect(SDL_Renderer* r, int x, int y, int w, int h, SDL_Color color) {
    SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
    SDL_Rect rect = { x, y, w, h };
    SDL_RenderFillRect(r, &rect);
}

static void draw_hollow_circle(SDL_Renderer* r, int cx, int cy, int radius) {
    for (float angle = 0; angle < 360; angle += 1.0f) {
        float rad = DEG2RAD(angle);
        int x = cx + (int)(cos(rad) * radius);
        int y = cy + (int)(sin(rad) * radius);
        SDL_RenderDrawPoint(r, x, y);
    }
}

// goals net texture
static void draw_net_texture(SDL_Renderer* r, SDL_Rect box) {
    SDL_SetRenderDrawColor(r, 200, 200, 200, 150); // Light grey, slightly transparent looking
    int spacing = 5; // Density of the net

    // Vertical strings
    for (int x = box.x; x <= box.x + box.w; x += spacing)
        SDL_RenderDrawLine(r, x, box.y, x, box.y + box.h);

    // Horizontal strings
    for (int y = box.y; y <= box.y + box.h; y += spacing)
        SDL_RenderDrawLine(r, box.x, y, box.x + box.w, y);
}

static void draw_pitch_markings(SDL_Renderer* r) {
    // Draw Grass Stripes
    int stripe_h = SCREEN_HEIGHT / GRASS_STRIPE_COUNT;
    for (int i = 0; i < GRASS_STRIPE_COUNT; i++) {
        if (i % 2 == 0) SDL_SetRenderDrawColor(r, 0, 145, 0, 255);
        else            SDL_SetRenderDrawColor(r, 0, 120, 0, 255);
        SDL_Rect stripe = {0, i * stripe_h, SCREEN_WIDTH, stripe_h};
        SDL_RenderFillRect(r, &stripe);
    }

    // GOAL PARAMETERS
    int goal_top_y = CENTER_Y - (GOAL_HEIGHT / 2);

    SDL_Rect left_goal = { PITCH_X - GOAL_WIDTH, goal_top_y, GOAL_WIDTH, GOAL_HEIGHT };
    SDL_Rect right_goal = { PITCH_X + PITCH_W, goal_top_y, GOAL_WIDTH, GOAL_HEIGHT };

    // Draw Net Texture
    draw_net_texture(r, left_goal);
    draw_net_texture(r, right_goal);

    // Draw White Lines
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255); 

    // Goal outlines
    SDL_RenderDrawRect(r, &left_goal);
    SDL_RenderDrawRect(r, &right_goal);

    // "Erase" the Goal Mouths. Barely recognizable
    // Use the grass color to overwrite the white line facing the field
    SDL_SetRenderDrawColor(r, 0, 145, 0, 255); 
    // SDL_RenderDrawLine(r, PITCH_X, goal_top_y + 1, PITCH_X, goal_top_y + GOAL_HEIGHT - 1);
    // SDL_RenderDrawLine(r, PITCH_X + PITCH_W, goal_top_y + 1, PITCH_X + PITCH_W, goal_top_y + GOAL_HEIGHT - 1);

    // Draw Main Pitch Lines
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255); 

    // Out Lines
    SDL_Rect field_rect = { PITCH_X, PITCH_Y, PITCH_W, PITCH_H };
    SDL_RenderDrawRect(r, &field_rect);

    // Center Line & Circle
    SDL_RenderDrawLine(r, CENTER_X, PITCH_Y, CENTER_X, PITCH_Y + PITCH_H);
    draw_hollow_circle(r, CENTER_X, CENTER_Y, 90);

    // Penalty Areas
    int box_w = 80;
    int box_h = 200;
    int box_top = CENTER_Y - (box_h / 2);
    SDL_Rect left_box = { PITCH_X, box_top, box_w, box_h };
    SDL_RenderDrawRect(r, &left_box);
    SDL_Rect right_box = { PITCH_X + PITCH_W - box_w, box_top, box_w, box_h };
    SDL_RenderDrawRect(r, &right_box);
}

static void render_text(SDL_Renderer* r, TTF_Font* font, const char* text, int x, int y, SDL_Color color) {
    if (!font || !text) return;

    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        SDL_Log("TTF_RenderText_Solid failed: %s", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(r, surface);
    if (!texture) {
        SDL_Log("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
    }

    // Set the destination rectangle for drawing
    SDL_Rect dst = { x, y, surface->w, surface->h };
    
    // Draw the texture and clean up
    if (texture) {
        SDL_RenderCopy(r, texture, NULL, &dst);
        SDL_DestroyTexture(texture);
    }
    
    SDL_FreeSurface(surface);
}

static void draw_circle(SDL_Renderer* r, int cx, int cy, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            const int dx = w - radius;
            const int dy = h - radius;
            if (dx * dx + dy * dy <= radius * radius) {
                SDL_RenderDrawPoint(r, cx + dx, cy + dy);
            }
        }
    }
}

/**
 * @brief Initializes the SDL window and renderer.
 * @param r Pointer to Renderer struct to initialize.
 * @return 0 on success.
 */
int renderer_init(struct Renderer* r) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        exit(1);
    }

    // --- TTF Initialization ---
    if (TTF_Init() == -1) {
        SDL_Log("TTF_Init failed: %s", TTF_GetError());
        SDL_Quit();
        exit(1);
    }
    
    extern unsigned char DejaVuSans_ttf[];
    extern unsigned int DejaVuSans_ttf_len;

    SDL_RWops* rw = SDL_RWFromConstMem(DejaVuSans_ttf, DejaVuSans_ttf_len);
    r->font = TTF_OpenFontRW(rw, 1, 24);

    if (!r->font) {
        SDL_Log("TTF_OpenFont failed: %s", TTF_GetError());
    }

    // Initialize SDL_image for PNG
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
        SDL_Log("IMG_Init failed: %s", IMG_GetError());

    r->window = SDL_CreateWindow(
        "Soccer Engine",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, 0
    );

    if (!r->window) {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    r->sdl_renderer = SDL_CreateRenderer(r->window, -1, SDL_RENDERER_ACCELERATED);
    if (!r->sdl_renderer) {
        SDL_Log("Renderer creation failed: %s", SDL_GetError());
        SDL_DestroyWindow(r->window);
        SDL_Quit();
        exit(1);
    }

    // --- Dynamically compute path relative to executable ---
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat-truncation"

    char exe_path[512];
    const char* base_path = SDL_GetBasePath();  // Returns the directory of the executable
    if (base_path) {
        snprintf(exe_path, sizeof(exe_path), "%sassets/icons/", base_path);
        SDL_free((void*)base_path); // Free the string returned by SDL_GetBasePath
    } else {
        // Fallback if SDL_GetBasePath fails
        snprintf(exe_path, sizeof(exe_path), "assets/icons/");
    }

    // Load window icon
    char icon_file[512];
    snprintf(icon_file, sizeof(icon_file), "%sapp_icon.png", exe_path);


    SDL_Surface* icon_surface = IMG_Load(icon_file);
    if (icon_surface) {
        SDL_SetWindowIcon(r->window, icon_surface);
        SDL_FreeSurface(icon_surface);
    } else {
        SDL_Log("Failed to load icon '%s': %s", icon_file, IMG_GetError());
    }

    // Load player textures
    for (int i = 0; i < PLAYER_COUNT; i++) {
        char filename[512];
        char player_char = 'a' + i;

        snprintf(filename, sizeof(filename), "%sred_%c.png", exe_path, player_char);
        r->red_icons[i] = IMG_LoadTexture(r->sdl_renderer, filename);
        if (!r->red_icons[i]) {
            SDL_Log("Failed to load RED player texture %s: %s", filename, IMG_GetError());
        }

        snprintf(filename, sizeof(filename), "%sblue_%c.png", exe_path, player_char);
        r->blue_icons[i] = IMG_LoadTexture(r->sdl_renderer, filename);
        if (!r->blue_icons[i]) {
            SDL_Log("Failed to load BLUE player texture %s: %s", filename, IMG_GetError());
        }
    }
    #pragma GCC diagnostic pop
    
    return 0;
}


/**
 * @brief Cleans up SDL renderer and window.
 * @param r Pointer to Renderer struct.
 */
void renderer_destroy(struct Renderer* r) {
    if (r->font) TTF_CloseFont(r->font);
    TTF_Quit();
    IMG_Quit();

    for (int i = 0; i < PLAYER_COUNT; i++) {
        if (r->red_icons[i])
            SDL_DestroyTexture(r->red_icons[i]);
        if (r->blue_icons[i])
            SDL_DestroyTexture(r->blue_icons[i]);
    }
    if (r->sdl_renderer) SDL_DestroyRenderer(r->sdl_renderer);
    if (r->window) SDL_DestroyWindow(r->window);
    SDL_Quit();
}


/**
 * @brief Draws the full game scene: teams and ball->
 * @param r Pointer to Renderer.
 * @param scene Pointer to Scene to render.
 */
void renderer_draw_scene(struct Renderer* r, const Scene* scene) {

    draw_pitch_markings(r->sdl_renderer);

    for (int i = 0; i < PLAYER_COUNT; i++) {
        const Player *p1 = scene->first_team->players[i];
        const Player *p2 = scene->second_team->players[i];

        // Players icon rectangle (position + size)
        SDL_Rect dest_rect = {
            (int)p1->position.x - p1->radius,
            (int)p1->position.y - p1->radius,
            (int)p1->radius * 2,
            (int)p1->radius * 2
        };
        if (r->red_icons[i]) {
            SDL_RenderCopy(r->sdl_renderer, r->red_icons[i], NULL, &dest_rect);
        } else { // Fallback to circle if texture failed to load
            SDL_SetRenderDrawColor(r->sdl_renderer, 255, 0, 0, 255);
            draw_circle(r->sdl_renderer, (int)p1->position.x, (int)p1->position.y, (int)p1->radius);
        }
        dest_rect.x = (int)p2->position.x - p2->radius;
        dest_rect.y = (int)p2->position.y - p2->radius;
        
        if (r->blue_icons[i]) {
            SDL_RenderCopy(r->sdl_renderer, r->blue_icons[i], NULL, &dest_rect);
        } else { // Fallback
            SDL_SetRenderDrawColor(r->sdl_renderer, 0, 0, 255, 255);
            draw_circle(r->sdl_renderer, (int)p2->position.x, (int)p2->position.y, (int)p2->radius);
        }
    }

    SDL_SetRenderDrawColor(r->sdl_renderer, 255, 255, 255, 255);
    draw_circle(r->sdl_renderer, (int)scene->ball->position.x, (int)scene->ball->position.y, (int)scene->ball->radius);

    // DRAW SCOREBOARD
    int box_w = 150;
    int box_h = 50;
    int box_x = (SCREEN_WIDTH - box_w) / 2;
    int box_y = 5;

    // Background (semi-transparent black)
    SDL_SetRenderDrawBlendMode(r->sdl_renderer, SDL_BLENDMODE_BLEND);
    draw_filled_rect(r->sdl_renderer, box_x, box_y, box_w, box_h,
                     (SDL_Color){0, 0, 0, 200});

    // Border (white)
    SDL_SetRenderDrawColor(r->sdl_renderer, 255, 255, 255, 200);
    SDL_Rect border = { box_x, box_y, box_w, box_h };
    SDL_RenderDrawRect(r->sdl_renderer, &border);

    // Team scores
    int left_score  = scene->first_team->score;
    int right_score = scene->second_team->score;

    SDL_Color left_color  = (SDL_Color){255, 0, 0, 255};
    SDL_Color right_color = (SDL_Color){0, 128, 255, 255};

    char left_text[16];
    char right_text[16];

    sprintf(left_text, "%d", left_score);
    sprintf(right_text, "%d", right_score);

    render_text(r->sdl_renderer, r->font,
                left_text,
                box_x + 25, box_y + 10,
                left_color);

    render_text(r->sdl_renderer, r->font,
                right_text,
                box_x + box_w - 40, box_y + 10,
                right_color);

    render_text(r->sdl_renderer, r->font,
                "VS",
                box_x + box_w / 2 - 15, box_y + 10,
                (SDL_Color){255,255,255,255});

    SDL_RenderPresent(r->sdl_renderer);
}


/**
 * @brief Main logic dispatcher.
 * * This function orchestrates the three phases of a frame:
 * 1. Time Management (Is the game over?)
 * 2. Scene Update (Physics & Movement)
 * 3. Referee Check (Rules & Fouls)
 */
void update_scene(Scene* scene, const float dt) {
    // ----------------------------- PHASE 1: state controll -----------------------------
    // --- State: RESTARTING (The short Delay before calling player to kick-off) ---
    if (scene->state == STATE_RESTARTING) {
        scene->wait_time -= dt;
        if (scene->wait_time <= 0) {
            scene->state = STATE_RUNNING;
            printf("the player should now kick-off / throw-in ... \n");
            struct Ball* ball = scene->ball;
            struct Player* player = ball->possessor;
            player->shooting_logic(player, scene);
            verify_shoot(ball, true);
            scene->ball->possessor = NULL;
        }
        return; // Don't process physics yet
    }

    // --- State: OUT ---
    if (scene->state == STATE_OUT) {
        scene->wait_time -= dt;
        if (scene->wait_time < 0) {
            scene->wait_time = 2.0f;    // wait 2 more seconds before calling the player to throw in
            set_piece_out(scene);       // Position players/ball
            scene->state = STATE_RESTARTING;
        }
        return;
    }

    // --- State: GOAL ---
    if (scene->state == STATE_GOAL) {
        scene->wait_time -= dt;
        if (scene->wait_time < 0) {
            scene->wait_time = 2.0f;    // wait 2 more seconds before calling the player to kick off
            set_piece_goal(scene);      // Position players/ball
            scene->state = STATE_RESTARTING;
        }
        return;
    }

    if (scene->state != STATE_RUNNING) return; // scene->state == STATE_TIMEOUT
    scene->remaining_time -= dt;
    // --- State: TIMEOUT ---
    if (scene->remaining_time < 0.0f) {
        printf("Game Time has ended ...\n");
        scene->state = STATE_TIMEOUT;
        return;
    }

    // ----------------------------- PHASE 2: update the scene -----------------------------
    update_and_verify_scene_states(scene, dt);

    // ----------------------------- PHASE 3: call the referee -----------------------------
    // after screen update, call the referee to check all the rules
    // --- referee check ---
    switch (referee(scene)) {
        case GOAL:
            scene->state = STATE_GOAL;
            scene->wait_time = 5.0f; // 5 second delay before kick-off
            printf("Goal scored!\n");
            printf("first team score: %d\n", scene->first_team->score);
            printf("second team score: %d\n", scene->second_team->score);
            break;
        case OUT:
            scene->state = STATE_OUT;
            scene->wait_time = 2.0f; // 2 second delay before set-piece
            printf("Ball out of bounds!\n");
            break;
        default:
            break;  // no event, game continues
    }
}
