cmake_minimum_required(VERSION 3.25)

project(
    MeuJogo
    VERSION 0.1.0
    LANGUAGES CXX
)

# C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Exporta compile_commands.json (VSCode, clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# SDL3
include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.18
)

FetchContent_MakeAvailable(SDL3)

# Arquivos do projeto
file(
    GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(roguelike
    main.cpp
    ${SOURCES}
)

# Pasta include/
target_include_directories(
    roguelike
    PRIVATE
        ./include
)

# Warnings
if(MSVC)
    target_compile_options(
        roguelike
        PRIVATE
            /W4
            /permissive-
    )
else()
    target_compile_options(
        roguelike
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wshadow
    )
endif()

# Link SDL3
target_link_libraries(
    roguelike
    PRIVATE
        SDL3::SDL3
)

# Copiar assets para a pasta do executável
add_custom_command(
    TARGET roguelike
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:roguelike>/assets
)