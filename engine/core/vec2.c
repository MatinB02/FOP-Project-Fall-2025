#include "vec2.h"
#include <math.h>

void vec2_add(struct Vec2 *out, const struct Vec2 *a, const struct Vec2 *b) {
  out->x = a->x + b->x;
  out->y = a->y + b->y;
}

void vec2_sub(struct Vec2 *out, struct Vec2 *a, struct Vec2 *b) {
  out->x = a->x - b->x;
  out->y = a->y - b->y;
}

void mulVec2(struct Vec2 *out, struct Vec2 *a, struct Vec2 *b) {
  out->x = a->x * b->x;
  out->y = a->y * b->y;
}

float dotProduct(struct Vec2 *a, struct Vec2 *b) {
  return a->x * b->x + a->y * b->y;
}

float vec2Determinant(struct Vec2 *a, struct Vec2 *b) {
  return a->x * b->y - a->y * b->x;
}

float lengthVec2(struct Vec2 *a) { return sqrt(a->x * a->x + a->y * a->y); }

float vec2Rotation(struct Vec2 *a) { return atan2(a->y, a->x); }
