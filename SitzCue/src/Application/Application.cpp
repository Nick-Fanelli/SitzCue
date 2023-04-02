#include "Application.h"

#include "ImGuiLayer.h"

using namespace SitzCue;

static constexpr int s_DisplayWidth = 1600;
static constexpr int s_DisplayHeight = 900;

static ImGuiLayer s_ImGuiLayer;

void Application::StartApplication() {
    s_ImGuiLayer = ImGuiLayer(this);

    m_Display.CreateDisplay();

    s_ImGuiLayer.OnCreate();

    m_Display.StartApplicationLoop([&](float deltaTime) {
        this->OnUpdate(deltaTime);
    });

    s_ImGuiLayer.OnDestroy();
}

void Application::OnUpdate(float deltaTime) {
    s_ImGuiLayer.Begin();
    s_ImGuiLayer.End();
}