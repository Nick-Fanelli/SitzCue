#include "ImGuiLayer.h"

using namespace SitzCue;

static const ImVec4 BrandAccentColor = { 0.396078f, 0.803921f, 0.992156f, 1.0f };

ImGuiLayer::ImGuiLayer(Application* application) {
    m_Application = application;
}

void ImGuiLayer::OnCreate() {

    const char* glslVersion = "#version 410 core";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // TODO
    // io.IniFilename = saveFilelocation.c_str();
    // ImGui::LoadIniSettingsFromDisk(s_SaveFileLocation.c_str());

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImFontConfig config;
    config.OversampleH = 3;
    config.OversampleV = 3;

    // Default Font
    io.FontDefault = io.Fonts->AddFontFromFileTTF("resources/fonts/Inter-Regular.ttf", 16.0f, &config);

    // Font Awesome
    // ImFontConfig faConfig;
    // faConfig.MergeMode = true;
    // faConfig.GlyphMinAdvanceX = 13.0f;
    // static constexpr ImWchar iconRanges[] = { 0xe005, 0xf8ff, 0 };
    // io.Fonts->AddFontFromFileTTF("assets/fonts/font-awesome/Font-Awesome-Solid-900.otf", 16.0f, &faConfig, iconRanges);

    ImGuiStyle& style = ImGui::GetStyle();
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    m_Application->GetImGuiSize(&io.DisplaySize);

    ImGui_ImplGlfw_InitForOpenGL(m_Application->GetWindowPtr(), true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    ApplyColorTheme();
}

void ImGuiLayer::Begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Test");
    ImGui::Text("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum");
    ImGui::End();
}

void ImGuiLayer::End() {
    static ImGuiIO& io = ImGui::GetIO();
    m_Application->GetImGuiSize(&io.DisplaySize);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* previousContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(previousContext);
    }
}

void ImGuiLayer::OnDestroy() {
    // TODO: IMPLEMENT
    // ImGui::SaveIniSettingsToDisk(s_SaveFileLocation.c_str());
    ImGui::SaveIniSettingsToDisk("imgui.ini");
}

void ImGuiLayer::ApplyColorTheme() {
    static auto& colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_DockingPreview] = BrandAccentColor;
    colors[ImGuiCol_DragDropTarget] = BrandAccentColor;
    colors[ImGuiCol_NavHighlight] = BrandAccentColor;
    colors[ImGuiCol_ResizeGripActive] = BrandAccentColor;

    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    
    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}