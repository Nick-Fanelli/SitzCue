#include "Window.h"

#include "CueListWindow.h"
#include "CuePropertiesWindow.h"

using namespace SitzCue;

void Window::OnUpdate() {}

void WindowManager::OnCreate() {

    SITZCUE_PROFILE_FUNCTION();

    m_StatusBarPtr = new StatusBar();
    m_CueListWindowPtr = new CueListWindow();
    m_CuePropertiesWindowPtr = new CuePropertiesWindow(m_CueListWindowPtr);
}

void WindowManager::OnUpdate() {

    SITZCUE_PROFILE_FUNCTION();

    m_StatusBarPtr->OnUpdate();
    m_CueListWindowPtr->OnUpdate();
    m_CuePropertiesWindowPtr->OnUpdate();
}

void WindowManager::OnDestroy() {

    SITZCUE_PROFILE_FUNCTION();

    delete m_StatusBarPtr;
    delete m_CueListWindowPtr;
    delete m_CuePropertiesWindowPtr;
}