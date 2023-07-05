set(GLAD_SOURCES 
    glad.c
    glad.h)

add_library(glad ${GLAD_SOURCES})

target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
