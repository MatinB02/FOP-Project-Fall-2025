/**
 * @file field.h
 * @brief Defines the soccer field dimensions.
 */

#ifndef ENGINE_ENTITIES_FIELD_H
#define ENGINE_ENTITIES_FIELD_H

/**
 * @struct Field
 * @brief Represents the size of the soccer field.
 */
typedef struct Field {
    const int width;
    const int height;
} Field;

#endif /* ENGINE_ENTITIES_FIELD_H */
