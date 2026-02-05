cmake_minimum_required(VERSION 3.20)
project(soccerengine LANGUAGES C)

# --- Compiler settings ---
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# --- Dependencies ---
include(FetchContent)
include(cmake/LinkSDL2.cmake)

# --- Disable SDL2_ttf internal tests ---
set(SDL2_TTF_DISABLE_TESTS ON CACHE BOOL "" FORCE)
set(SDL2_IMAGE_DISABLE_TESTS ON CACHE BOOL "Disable SDL2_image internal tests")
set(SDL2IMAGE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# --- Source files ---
# 1. Glob the engine files (scan only the engine folder)
file(GLOB_RECURSE ENGINE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/engine/*.c")

# 2. Define your main source explicitly (no scanning needed)
set(MAIN_SRC "${CMAKE_CURRENT_SOURCE_DIR}/main.c")

# 3. Combine them
set(SRC_FILES ${ENGINE_SRC} ${MAIN_SRC})

# file(
#     GLOB_RECURSE SRC_FILES
#     "${CMAKE_CURRENT_SOURCE_DIR}/engine/*.c"
#     "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
# )

file(
    COPY ${CMAKE_SOURCE_DIR}/engine/assets/icons/
    DESTINATION ${CMAKE_BINARY_DIR}/bin/assets/icons/
)

# --- Executable ---
add_executable(soccerengine ${SRC_FILES})

# --- Link SDL2 ---
setupsdl(soccerengine PRIVATE)

# --- Embed font directly ---
set(GENERATED_FONT_C ${CMAKE_BINARY_DIR}/bin/embedded_font.c)

add_custom_command(
    OUTPUT ${GENERATED_FONT_C}
    COMMAND
        xxd -n DejaVuSans_ttf -i
        ${CMAKE_SOURCE_DIR}/engine/assets/fonts/DejaVuSans.ttf >
        ${GENERATED_FONT_C}
    DEPENDS ${CMAKE_SOURCE_DIR}/engine/assets/fonts/DejaVuSans.ttf
)

add_library(embedded_font STATIC ${GENERATED_FONT_C})

set_target_properties(
    embedded_font
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# --- Link libraries ---
target_link_libraries(
    soccerengine
    PRIVATE embedded_font SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf
)

if(NOT WIN32)
    target_link_libraries(soccerengine PRIVATE m)
endif()

# --- Include directories ---
target_include_directories(
    soccerengine
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/engine
)

# --- Compiler warnings ---
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(soccerengine PRIVATE -Wall -Wextra -Wpedantic)
endif()

# --- Output directory ---
set_target_properties(
    soccerengine
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
