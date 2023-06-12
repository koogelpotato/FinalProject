#include "glad/glad.h"
#include "glm/glm.hpp"
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
SDL_Window* window;
SDL_GLContext gl_context;

bool shouldCloseWindow = false;

void HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
        if (event.type == SDL_EVENT_QUIT)
        {
            shouldCloseWindow = true;
        }
    }
}

int main(int argc, char* argv[])
{
    const int sdl_init = SDL_Init(SDL_INIT_VIDEO);
    if (sdl_init != 0)
    {
        std::cerr << "error SDL_Init: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    window = SDL_CreateWindow("window", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        std::cerr << "error SDL_CreateWindow: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    gl_context = SDL_GL_CreateContext(window);
    if (gl_context == nullptr)
    {
        std::cerr << "error SDL_GL_CreateContext: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (!gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "ERROR: Failed to load glad functions" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);
    SDL_ShowWindow(window);
    glGetError();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    bool showFigure = false;

    Resource_Manager::load_shader("/home/koogel/Final Project/final/imgui-shader.vs", "/home/koogel/Final Project/final/imgui-shader.frag", "UI");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH),
                                      static_cast<float>(SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f);

    Test.init_game();
    glGetError();

    bool is_running = true;
    unsigned int current_frame = SDL_GetPerformanceCounter();
    unsigned int last_frame = 0;
    double delta_time = 0;

    while (is_running)
    {
        last_frame = current_frame;
        current_frame = SDL_GetPerformanceCounter();

        delta_time = (double)((current_frame - last_frame) * 1000 / (double)SDL_GetPerformanceFrequency());

        HandleEvents(); // Handle SDL events

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImVec2 mainWindowSize = ImVec2(1000, 600);
        ImGui::SetNextWindowSize(mainWindowSize);
        ImGui::Begin("TestGui");

        if (ImGui::Button("Show Figure"))
        {
            showFigure = true;
        }
        if (showFigure)
        {
            showFigure = false;
            SDL_Window* figureWindow = SDL_CreateWindow("figure_window", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
            SDL_GL_MakeCurrent(figureWindow, gl_context);
            SDL_GL_SetSwapInterval(1);

            bool shouldCloseFigureWindow = false;
            while (!shouldCloseFigureWindow && !shouldCloseWindow)
            {
                HandleEvents(); // Handle SDL events

                glClearColor(0.2, 0.3, 0.3, 1.0);
                glClear(GL_COLOR_BUFFER_BIT);


                SDL_GL_SwapWindow(figureWindow);
            }
            SDL_DestroyWindow(figureWindow);
            SDL_GL_MakeCurrent(window, gl_context);
        }
        ImGui::End();

        ImGui::Render();

        Test.proccess_input(delta_time);
        Test.update(delta_time);

        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Test.render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    Resource_Manager::clear();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}