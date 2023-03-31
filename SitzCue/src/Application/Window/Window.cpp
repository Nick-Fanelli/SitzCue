#include "Window.h"

#include "CueListWindow.h"
#include "CuePropertiesWindow.h"

using namespace SitzCue;

void Window::OnUpdate() {}

void WindowManager::OnCreate() {
    m_StatusBarPtr = new StatusBar();
    m_CueListWindowPtr = new CueListWindow();
    m_CuePropertiesWindowPtr = new CuePropertiesWindow(m_CueListWindowPtr);
}

void WindowManager::OnUpdate() {
    m_StatusBarPtr->OnUpdate();
    m_CueListWindowPtr->OnUpdate();
    m_CuePropertiesWindowPtr->OnUpdate();
}

void WindowManager::OnDestroy() {
    delete m_StatusBarPtr;
    delete m_CueListWindowPtr;
    delete m_CuePropertiesWindowPtr;
}