#include "ImGui.hpp"
#include "resource-manager.hpp"


static const char* ImGui_ImplSdlGL3_GetClipboardText(void*)
{
    return SDL_GetClipboardText();
}

static void ImGui_ImplSdlGL3_SetClipboardText(void*, const char* text)
{
    SDL_SetClipboardText(text);
}

bool User_Interface::ImGui_ImplSdlGL3_CreateDeviceObjects()
{

}

bool User_Interface::ImGui_ImplSdlGL3_Init(SDL_Window* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    io.SetClipboardTextFn = ImGui_ImplSdlGL3_SetClipboardText;
    io.GetClipboardTextFn = ImGui_ImplSdlGL3_GetClipboardText;
    io.ClipboardUserData  = nullptr;

    return true;
}

