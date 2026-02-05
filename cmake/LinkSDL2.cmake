# include(FetchContent)

# macro(SetupSDL TARGET ACCESS)
#     set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

#     # Shared SDL2 settings
#     set(SDL_TEST OFF CACHE BOOL "" FORCE)
#     set(SDL_SHARED ON CACHE BOOL "" FORCE)
#     set(SDL_STATIC OFF CACHE BOOL "" FORCE)
#     set(SDL2IMAGE_SHARED ON CACHE BOOL "" FORCE)
#     set(SDL2TTF_SHARED ON CACHE BOOL "" FORCE)
#     set(SDL2_DISABLE_INSTALL ON CACHE BOOL "" FORCE)

#     # Standard SDL2 Image settings
#     set(SDL2IMAGE_VENDORED ON CACHE BOOL "" FORCE)
#     set(SDL2IMAGE_SAMPLES OFF CACHE BOOL "" FORCE)
#     set(SDL2IMAGE_TESTS OFF CACHE BOOL "" FORCE)
#     set(SDL2IMAGE_AVIF OFF CACHE BOOL "" FORCE)
#     set(SDL2IMAGE_TIF OFF CACHE BOOL "" FORCE)
#     set(SDL2IMAGE_WEBP OFF CACHE BOOL "" FORCE)
#     set(SDL2IMAGE_JXL OFF CACHE BOOL "" FORCE)
#     set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
#     set(ENABLE_TESTS OFF CACHE BOOL "" FORCE)
#     set(DAV1D_ENABLE_TESTS OFF CACHE BOOL "" FORCE)

#     # Standard SDL2 TTF settings
#     set(SDL2TTF_SAMPLES OFF CACHE BOOL "" FORCE)
#     set(SDL2TTF_VENDORED ON CACHE BOOL "" FORCE)
#     set(SDL2TTF_HARFBUZZ OFF CACHE BOOL "" FORCE)
#     set(FT_DISABLE_TESTS ON CACHE BOOL "" FORCE)
#     set(FT_WITH_ZLIB OFF CACHE BOOL "" FORCE)

#     FetchContent_Declare(
#         sdl2
#         GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
#         GIT_TAG release-2.32.10
#         GIT_SHALLOW TRUE
#     )
#     FetchContent_Declare(
#         sdl2image
#         GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
#         GIT_TAG release-2.8.2
#         GIT_SHALLOW TRUE
#     )
#     FetchContent_Declare(
#         sdl2_ttf
#         GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
#         GIT_TAG release-2.24.0
#         GIT_SHALLOW TRUE
#     )

#     FetchContent_MakeAvailable(sdl2 sdl2image sdl2_ttf)

#     # Link to the actual targets created by the source
#     target_link_libraries(
#         ${TARGET}
#         ${ACCESS}
#         SDL2::SDL2
#         SDL2_image::SDL2_image
#         SDL2_ttf::SDL2_ttf
#     )
# endmacro()


# first, checks to see if they're already installed:
include(FetchContent)

macro(SetupSDL TARGET ACCESS)

    # ---------------------------------
    # 1. Try system-installed SDL first
    # ---------------------------------
    find_package(SDL2 QUIET CONFIG)
    find_package(SDL2_image QUIET CONFIG)
    find_package(SDL2_ttf QUIET CONFIG)

    if (SDL2_FOUND AND SDL2_image_FOUND AND SDL2_ttf_FOUND)
        message(STATUS "Using system-installed SDL2 libraries")

    else()
        message(STATUS "System SDL2 not found, fetching with FetchContent")

        # -------- SDL base --------
        set(SDL_TEST OFF CACHE BOOL "" FORCE)
        set(SDL_SHARED ON CACHE BOOL "" FORCE)
        set(SDL_STATIC OFF CACHE BOOL "" FORCE)
        set(SDL2_DISABLE_INSTALL ON CACHE BOOL "" FORCE)

        # -------- SDL_image --------
        set(SDL2IMAGE_SHARED ON CACHE BOOL "" FORCE)
        set(SDL2IMAGE_VENDORED ON CACHE BOOL "" FORCE)
        set(SDL2IMAGE_TESTS OFF CACHE BOOL "" FORCE)
        set(SDL2IMAGE_SAMPLES OFF CACHE BOOL "" FORCE)

        # -------- SDL_ttf --------
        set(SDL2TTF_SHARED ON CACHE BOOL "" FORCE)
        set(SDL2TTF_VENDORED ON CACHE BOOL "" FORCE)
        set(SDL2TTF_SAMPLES OFF CACHE BOOL "" FORCE)
        set(SDL2TTF_HARFBUZZ OFF CACHE BOOL "" FORCE)

        FetchContent_Declare(
            sdl2
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG release-2.32.10
            GIT_SHALLOW TRUE
        )

        FetchContent_Declare(
            sdl2image
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
            GIT_TAG release-2.8.2
            GIT_SHALLOW TRUE
        )

        FetchContent_Declare(
            sdl2_ttf
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
            GIT_TAG release-2.24.0
            GIT_SHALLOW TRUE
        )

        FetchContent_MakeAvailable(sdl2 sdl2image sdl2_ttf)
    endif()

    # ---------------------------------
    # 2. Link (same targets either way)
    # ---------------------------------
    target_link_libraries(
        ${TARGET}
        ${ACCESS}
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
    )

endmacro()
