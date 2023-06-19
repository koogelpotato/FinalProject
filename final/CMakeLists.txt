cmake_minimum_required(VERSION 3.26)
project(project_engine)

add_subdirectory(imgui_pack)

add_library(image_engine
    resource-manager.hpp
    shader.hpp
    sprite-renderer.hpp
    texture.hpp
    glad/glad.c
    glad/glad.h
    KHR/khrplatform.h
    resource-manager.cpp
    shader.cpp
    sprite-renderer.cpp
    texture.cpp
    game.cpp
    game-object.cpp
    game-object.hpp
    sound.cpp 
    sound.hpp
    game-level.cpp
    game-level.hpp
    ball-object.cpp
    ball-object.hpp
)

add_library(loader
    stb_image.h
    stb_image.cpp
)

target_link_libraries(image_engine PRIVATE loader)

find_package(SDL3 REQUIRED)

find_package(OpenGL REQUIRED)

target_include_directories(image_engine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/glm)

target_link_libraries(image_engine PRIVATE SDL3::SDL3-shared OpenGL::GL)
target_link_libraries(image_engine PRIVATE imgui_pack)

add_executable(image main.cpp)

target_link_libraries(image PRIVATE image_engine)

file(COPY resources/textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)
file(COPY resources/sounds DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)
file(COPY resources/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)
file(COPY resources/levels DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)
