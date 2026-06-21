cmake_minimum_required(VERSION 3.25)

project(
    Roguelike
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

add_executable(${PROJECT_NAME}
    main.cpp
    ${SOURCES}
)

# Pasta include/
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        ./include
)

# Warnings
if(MSVC)
    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE
            /W4
            /permissive-
    )
else()
    target_compile_options(
        ${PROJECT_NAME}
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
    ${PROJECT_NAME}
    PRIVATE
        SDL3::SDL3
)

# Copiar assets para a pasta do executável
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS
)