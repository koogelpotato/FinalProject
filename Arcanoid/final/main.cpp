#include "glad/glad.h"
#include "glm/glm.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_audio.h>
//#include "imgui_pack/imgui.h"
//#include "imgui_pack/imgui_impl_opengl3.h"
//#include "imgui_pack/imgui_impl_sdl3.h"

#include "game.hpp"
#include "resource-manager.hpp"

#include <iostream>

const unsigned int SCREEN_WIDTH = 600;
const unsigned int SCREEN_HEIGHT = 600;


SDL_Window* window;
SDL_GLContext gl_context;


void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar* message, const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
}

void render_main_menu();

int main(int argc, char* argv[])
{
    const int sdl_init_video = SDL_Init(SDL_INIT_VIDEO);
    if (sdl_init_video != 0)
    {
        std::cerr << "error SDL_INIT_VIDEO: " << SDL_GetError() << std::endl;
        return -1;
    }

    const int  sdl_init_audio = SDL_Init(SDL_INIT_AUDIO);
    if(sdl_init_audio != 0)
    {
        std::cerr << "error SDL_INIT_AUDIO: " << SDL_GetError() << std::endl;
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

    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO();
    //(void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    //ImGui::StyleColorsDark();

    //ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    //ImGui_ImplOpenGL3_Init("#version 300 es");


    //Resource_Manager::load_shader("resources/shaders/imgui-shader.vs", "resources/shaders/imgui-shader.frag", "UI");
    //glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH),
                                      //static_cast<float>(SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f);
                                      
    Game Test(SCREEN_WIDTH, SCREEN_HEIGHT);

    Test.init_game();
    glGetError();

    bool is_running = true;
    bool show_ui = true;
    unsigned int current_frame = SDL_GetPerformanceCounter();
    unsigned int last_frame = 0;
    double delta_time = 0;


    SDL_Event event;

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
    while (is_running)
    {
        last_frame = current_frame;
        current_frame = SDL_GetPerformanceCounter();

        delta_time = (double)((current_frame - last_frame) * 1000 / (double)SDL_GetPerformanceFrequency());
        
        //ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplSDL3_NewFrame();
        //ImGui::NewFrame();

        //ImVec2 mainWindowSize = ImVec2(SCREEN_WIDTH/4, SCREEN_HEIGHT/4);
        //ImVec2 uiPos = ImVec2(SCREEN_WIDTH/4,SCREEN_HEIGHT/4);
        //ImGui::SetNextWindowSize(mainWindowSize);
        //ImGui::SetWindowPos(uiPos);
        //ImGui::Begin("Break Out!");

        //if (ImGui::Button("Start"))
        //{
         //   show_ui = false;
        //}
        //ImGui::End();
        
        while(SDL_PollEvent(&event))
        {
             switch(event.type)
        {
        case SDL_EVENT_QUIT:
        {
            is_running = false;
            break;
        }
        case SDL_EVENT_KEY_DOWN:
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_SPACE:
                case SDLK_a:
                case SDLK_d:
                case SDLK_w:
                case SDLK_s:
                    Test.proccess_input(event, true);
            }
        
        break;
        }
        case SDL_EVENT_KEY_UP:
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_SPACE:
                case SDLK_a:
                case SDLK_d:
                case SDLK_w:
                case SDLK_s:
                    Test.proccess_input(event, false);
            }
        
        break;
        }
        }
        }

        Test.update_player_input(delta_time);
        Test.update(delta_time);

        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Test.render();

        //if(show_ui)
       // {
        //    ImGui::Render();
        //    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //}

        SDL_GL_SwapWindow(window);
    }

        //ImGui_ImplOpenGL3_Shutdown();
        //ImGui_ImplSDL3_Shutdown();
        //ImGui::DestroyContext();
    
        Resource_Manager::clear();

        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
}
