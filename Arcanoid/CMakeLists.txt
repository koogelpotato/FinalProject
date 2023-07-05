cmake_minimum_required(VERSION 3.26)
project(project_engine)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/submodules/SDL
                 ${CMAKE_BINARY_DIR}/final/SDL)

#add_subdirectory(final/imgui_pack)
add_subdirectory(final/glad)

add_library(image_engine SHARED
    final/resource-manager.hpp
    final/shader.hpp
    final/sprite-renderer.hpp
    final/texture.hpp
    final/glad/glad.c
    final/glad/glad.h
    final/KHR/khrplatform.h
    final/resource-manager.cpp
    final/shader.cpp
    final/sprite-renderer.cpp
    final/texture.cpp
    final/game.cpp
    final/game-object.cpp
    final/game-object.hpp
    final/game-level.cpp
    final/game-level.hpp
    final/ball-object.cpp
    final/ball-object.hpp
    final/sound-manager.cpp
    final/sound-manager.hpp
)
add_library(loader SHARED
    final/stb_image.h
    final/stb_image.cpp
)

target_link_libraries(image_engine PRIVATE loader)

# find_package(SDL3 REQUIRED)

target_include_directories(image_engine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/final/glm)

target_link_libraries(image_engine PUBLIC SDL3::SDL3-shared)
#target_link_libraries(image_engine PRIVATE imgui_pack)
target_link_libraries(image_engine PRIVATE glad)

#add_executable(image final/main.cpp)

add_library(image SHARED final/main.cpp)

target_compile_features(image PRIVATE cxx_std_17)

target_link_libraries(image PUBLIC image_engine)

file(COPY final/resources/textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/final/resources)
file(COPY final/resources/sounds DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/final/resources)
file(COPY final/resources/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/final/resources)
file(COPY final/resources/levels DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/final/resources)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")

	target_link_libraries(
	glad
	android
	log
	EGL
	GLESv3)

endif ()

