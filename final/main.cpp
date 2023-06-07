#include "glad/glad.h"
#include <SDL3/SDL.h>
#include "imgui_pack/imgui.h"
#include "imgui_pack/imgui_impl_opengl3.h"
#include "imgui_pack/imgui_impl_sdl3.h"

#include "game.hpp"
#include "resource-manager.hpp"

#include <iostream>


const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

Game Test(SCREEN_WIDTH, SCREEN_HEIGHT);


//User_Interface* user_interface;
int main(int arc, char *argv[])
{
    const int sdl_init = SDL_Init(SDL_INIT_VIDEO);

    if (sdl_init != 0)
    {
        std::cerr << "error SDL_Init: " << SDL_GetError() << std::endl;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_Window* window = SDL_CreateWindow("window", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        std::cerr << "error SDL_CreateWindow: " << SDL_GetError() << std::endl;
    }

    int gl_major_ver       = 3;
    int gl_minor_ver       = 2;
    int gl_context_profile = SDL_GL_CONTEXT_PROFILE_ES;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, gl_context_profile);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_ver);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_ver);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    if (gl_context == nullptr)
    {
        gl_major_ver       = 3;
        gl_minor_ver       = 2;
        gl_context_profile = SDL_GL_CONTEXT_PROFILE_ES;

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                                gl_context_profile);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_ver);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_ver);
        gl_context = SDL_GL_CreateContext(window);
    }

    assert(gl_context != nullptr);

    int check_current_major_ver = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &gl_major_ver);
    assert(check_current_major_ver == 0);

    int check_current_minor_ver = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &gl_minor_ver);
    assert(check_current_minor_ver == 0);

    if (gl_major_ver < 3)
    {
        std::cerr << "gl_major_ver < 3" << std::endl;
    }

    std::clog << "Opengl " << gl_major_ver << "." << gl_minor_ver << std::endl;

    if(!gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "ERROR: Failed to load glad functionsss" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); 
    SDL_ShowWindow(window);
    glGetError();



    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    //ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    //ImGui_ImplOpenGL3_Init("#version 300 es");

    Test.init_game();
    glGetError();
    bool is_running = true;
    unsigned int current_frame = SDL_GetPerformanceCounter();
    unsigned int last_frame = 0;
    double delta_time = 0;
    

    while(is_running)
    {
        last_frame = current_frame;
        current_frame = SDL_GetPerformanceCounter();

        delta_time = (double)((current_frame - last_frame)*1000 / (double)SDL_GetPerformanceFrequency() );

        //ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplSDL3_NewFrame();
        //ImGui::NewFrame();

        Test.proccess_input(delta_time);
        Test.update(delta_time);

        //ImGui::Render();
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        Test.render();
        SDL_GL_SwapWindow(window);
    }

    Resource_Manager::clear();
    //ImGui_ImplOpenGL3_Shutdown();
    //ImGui_ImplSDL3_Shutdown();
    //ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return   0;
}

