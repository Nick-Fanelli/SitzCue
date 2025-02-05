#include "ImGuiLayer.h"

#include "Application.h"
#include "StatusBar.h"
#include "MenuBar.h"
#include "Popup.h"
#include "ImGuiDefaults.h"

#include "CommandSystem/CommandStack.h"

using namespace SitzQ;

static MenuBar s_MenuBar;

ImGuiLayer::ImGuiLayer(Application* application) {
    m_Application = application;
}

void ImGuiLayer::OnCreate() {

    SITZCUE_PROFILE_FUNCTION();

    const char* glslVersion = "#version 410 core";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    auto& iniFilePath = PlatformDetection::GetIniFilePath();
    std::string iniFileData;
    FileUtils::ReadFile(iniFilePath, iniFileData);

    io.IniFilename = NULL;

    ImGui::LoadIniSettingsFromMemory(iniFileData.c_str(), iniFileData.size());

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImFontConfig config;
    config.OversampleH = 3;
    config.OversampleV = 3;

    // Default Font
    io.FontDefault = io.Fonts->AddFontFromFileTTF("resources/fonts/Inter-Regular.ttf", 17.0f, &config);

    // Font Awesome
    ImFontConfig faConfig;
    faConfig.MergeMode = true;
    faConfig.GlyphMinAdvanceX = 13.0f;
    static constexpr ImWchar iconRanges[] = { 0xe005, 0xf8ff, 0 };
    io.Fonts->AddFontFromFileTTF("resources/fonts/font-awesome/Font-Awesome-Solid-900.otf", 17.0f, &faConfig, iconRanges);

    // Bold Font
    s_BoldFont = io.Fonts->AddFontFromFileTTF("resources/fonts/Inter-Bold.ttf", 30.0f, &config);

    ImGuiStyle& style = ImGui::GetStyle();
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    io.ConfigMacOSXBehaviors = true;

    m_Application->GetDisplay().GetImGuiSize(&io.DisplaySize);

    ImGui_ImplGlfw_InitForOpenGL(m_Application->GetDisplay().GetWindowPtr(), true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    ApplyColorTheme();

    s_MenuBar = MenuBar(m_Application);

    m_SceneManager = SceneManager(m_Application);
    m_SceneManager.SetScene<LanderScene>();
}

static void DrawDockspace() {

    SITZCUE_PROFILE_FUNCTION();

    static bool isDockingEnabled = true;
    static constexpr bool optFullscreen = true;
    static constexpr ImGuiDockNodeFlags dockingFlags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    if(optFullscreen) {
        static ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - StatusBar::StatusBarHeight));
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if(dockingFlags * ImGuiDockNodeFlags_PassthruCentralNode)
        windowFlags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
    ImGui::Begin("Dockspace", &isDockingEnabled, windowFlags);

    s_MenuBar.OnUpdate();

    ImGui::PopStyleVar(optFullscreen ? 3 : 1);
    {
        // Draw Dockspace
        static ImGuiIO& io = ImGui::GetIO();
        ImGuiID dockspaceID = ImGui::GetID("Dockspace");
        ImGui::DockSpace(dockspaceID, { 0.0f, 0.0f }, dockingFlags);
    }
    ImGui::End();
}

void ImGuiLayer::Begin() {

    SITZCUE_PROFILE_FUNCTION();

    {
        SITZCUE_PROFILE_SCOPE("ImGui->NewFrames");

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    static ImGuiStyle& style = ImGui::GetStyle();
    static ImGuiIO& io = ImGui::GetIO();

    // Draw Dockspace
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 370.0f, style.WindowMinSize.y }); // Set minimum window size for dockspace windows
    DrawDockspace(); // Draw the dockspace environment
    ImGui::PopStyleVar(); // Pop dockspace window min size

    if(PlatformDetection::IsNativeCommandKey() && !io.WantTextInput) {
        if(!io.WantTextInput) {
            // Undo Pressed
            if(ImGui::IsKeyPressed(ImGuiKey_Z)) {
                CommandStack::Undo();
            } else if(ImGui::IsKeyPressed(ImGuiKey_Y)) { // Redo
                CommandStack::Redo();
            }
        }

    }

    m_SceneManager.OnUpdate();

    Popup::Update(); // Update Popup System
}

void ImGuiLayer::End() {

    SITZCUE_PROFILE_FUNCTION();

    static ImGuiIO& io = ImGui::GetIO();
    m_Application->GetDisplay().GetImGuiSize(&io.DisplaySize);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* previousContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(previousContext);
    }
}

static inline void SaveIniData() {
    size_t size;
    const char* rawData = ImGui::SaveIniSettingsToMemory(&size);

    FileUtils::ChangeContentTo(PlatformDetection::GetIniFilePath(), std::string(rawData));
}

void ImGuiLayer::OnDestroy() {
    
    SITZCUE_PROFILE_FUNCTION();

    m_SceneManager.OnDestroy();

    SaveIniData();
}

void ImGuiLayer::ApplyColorTheme() {

    SITZCUE_PROFILE_FUNCTION();

    static auto& colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_DockingPreview] = ImGuiDefaults::AccentColor;
    colors[ImGuiCol_DragDropTarget] = ImGuiDefaults::AccentColor;
    colors[ImGuiCol_NavHighlight] = ImGuiDefaults::AccentColor;
    colors[ImGuiCol_ResizeGripActive] = ImGuiDefaults::AccentColor;

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

    static auto& style = ImGui::GetStyle();
    style.FrameRounding = 5.0f;
    style.WindowRounding = 5.0f;
}