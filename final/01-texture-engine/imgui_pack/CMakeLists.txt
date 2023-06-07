cmake_minimum_required(VERSION 3.26)
project(imgui_pack)

set(IMGUI_SOURCES
    imconfig.h
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui.h
    imgui_impl_opengl2.cpp
    imgui_impl_opengl2.h
    imgui_impl_opengl3.cpp
    imgui_impl_opengl3.h
    imgui_impl_opengl3_loader.h
    imgui_impl_sdl3.cpp
    imgui_impl_sdl3.h
    imgui_impl_sdlrenderer3.cpp
    imgui_impl_sdlrenderer3.h
    imgui_internal.h
    imgui_tables.cpp
    imgui_widgets.cpp
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
)

add_library(imgui_pack ${IMGUI_SOURCES})

target_include_directories(imgui_pack PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})