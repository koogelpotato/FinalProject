#include "imgui_pack/imgui.h"
#include <SDL3/SDL.h>

#include "shader.hpp"
#include "texture.hpp"

class User_Interface
{
    public:
    User_Interface(Shader shader, Texture texture);
    bool ImGui_ImplSdlGL3_Init(SDL_Window* window);
    void ImGui_ImplSdlGL3_NewFrame(SDL_Window* window);
    void ImGui_ImplSdlGL3_RenderDrawData(ImDrawData* draw_data);
    void ImGui_ImplSdlGL3_Shutdown();
    bool ImGui_ImplSdlGL3_ProcessEvent(const SDL_Event* event);

    //<helper functions and callable functions>
    private:
    
};