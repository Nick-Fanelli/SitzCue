#include "Application.h"

using namespace SitzCue;

void Application::StartApplication() {
    m_Display.CreateDisplay();
    m_Display.StartApplicationLoop(this);
}

void Application::OnCreate() {
    m_ImGuiLayer = ImGuiLayer(this);
    m_ImGuiLayer.OnCreate();
}

void Application::OnUpdate(float deltaTime) {
    m_ImGuiLayer.Begin();
    m_ImGuiLayer.End();
}

void Application::OnDestroy() {
    m_ImGuiLayer.OnDestroy();
}