# ⚽ Soccer Engine

Welcome to the **Soccer Engine AI Challenge**! This is a lightweight, SDL2-based soccer simulation designed for students and developers to practice low-level C programming, game engine architecture, and autonomous agent logic.

## 📌 Project Overview

The engine simulates a 6v6 soccer match. The core physics, rendering, and entity management are provided. Your task is to act as the **Referee** (Phase 1) and the **Coach** (Phase 2).

### Tech Stack

* **Language:** C (C99)
* **Graphics:** SDL2, SDL_ttf, SDL_image
* **Physics:** Custom 2D vector-based kinematics

---

## 🚀 Phase 1: The Referee (Game Rules)

In this phase, you will focus on `engine/logic/referee.c`. Currently, the game is a "wild west" where players can move at light speed, and goals aren't properly recorded.

**Your Objectives:**

1. **Enforce Talent Limits:** Implement `verify_talents` to ensure no player exceeds the `MAX_TALENT_PER_PLAYER` sum (set to 5).
2. **Validate Movement:** Implement `verify_movement` to ensure players don't exceed the speed allowed by their Agility talent.
3. **Ball Possession Rules:** Validate that a player actually has the ball before they are allowed to enter the `SHOOTING` state.
4. **Set-Pieces:** Ensure that during a kickoff, the ball is passed backwards into the team's own half, as per the `verify_shoot` logic.

**Goal:** Familiarize yourself with the `Scene` structure, the `Player` entity, and how the game loop interacts with the logic layer.

---

## 🧠 Phase 2: The Coach (AI Strategy)

Once the rules are enforced, it’s time to make the players "smart." You will work primarily in `engine/logic/coach.c`.

**Your Objectives:**

* **Role-Based Logic:** Assign different movement patterns to Defenders, Midfielders, and Attackers based on their `kit` number.
* **Intercepting:** Write logic so that if a ball comes within a certain radius, players automatically switch to the `INTERCEPTING` state to lunge for it.
* **Teamwork (Passing):** Instead of just shooting at the goal, players should scan for teammates. If a teammate is closer to the goal and "open" (not covered by an opponent), the player should pass.
* **Defensive Positioning:** Defenders should stay between the ball and their own goal rather than just chasing the ball randomly.

**Goal:** Develop a rational AI agent that can win a match against a random-movement team without violating any of the referee's rules.

---

## 🛠️ Installation & Setup

### Prerequisites

* **GCC** or **Clang** compiler
* **SDL2** libraries (including `SDL_ttf` and `SDL_image`)

---

## 📂 Project Structure

* `engine/core/`: Constants and Vector Math (`vec2`).
* `engine/entities/`: Definitions for `Ball`, `Player`, and `Team`.
* `engine/logic/`: This is your workspace. Contains `referee.c` and `coach.c`.
* `engine/graphics/`: SDL2 Renderer and Scene management.

---


## 🤝 Contributing

This is an educational project. Feel free to fork it, implement your own AI strategies, and open a Pull Request if you find bugs in the core engine!

---

*Good luck, Coach! Bring home the trophy.* 🏆
