#include "Application.h"

#include "ImGuiLayer.h"

using namespace SitzCue;

static constexpr int s_DisplayWidth = 1600;
static constexpr int s_DisplayHeight = 900;

static ImGuiLayer s_ImGuiLayer;

void Application::StartApplication() {
    this->CreateWindow();

    s_ImGuiLayer = ImGuiLayer(this);

    this->LaunchApplicationLoop();
    this->CleanUp();
}

void Application::GetImGuiSize(ImVec2* outVec2) {
    int x, y;
    glfwGetWindowSize(m_Window, &x, &y);

    outVec2->x = (float) x;
    outVec2->y = (float) y;
}

void Application::CreateWindow() {
    if(!glfwInit()) {
        Log::Error("Failed to initialize GLFW!\n\tStatus: Returning");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    m_Window = glfwCreateWindow(s_DisplayWidth, s_DisplayHeight, "Sitz Cue", nullptr, nullptr);
    glfwSwapInterval(1); // Enable V-Sync

    int width, height;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    width = mode->width;
    height = mode->height;

    int newWidth = (int) ((float) (900 * width) / (float) height);
    int newHeight = (int) ((float) (1600 * height) / (float) width);

    glfwSetWindowSize(m_Window, newWidth, newHeight);

    if(m_Window == nullptr) {
        glfwTerminate();
        Log::Error("Could not create the GLFW Window");
        return;
    }

     int screenWidth, screenHeight;

    glfwGetFramebufferSize(m_Window, &screenWidth, &screenHeight);
    glfwMakeContextCurrent(m_Window);

    // Handle Input

    glfwWindowHint(GLFW_SAMPLES, 4);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "GLAD FAILED" << std::endl;
        Log::Error("Failed to initialize GLAD");
        return;
    }

    glViewport(0, 0, screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Log::Success("Created the GLFW Display!");

    glfwFocusWindow(m_Window);
}

void Application::LaunchApplicationLoop() {

    // Initialize
    s_ImGuiLayer.OnCreate();

    float endTime, startTime = (float) glfwGetTime();
    float deltaTime = -1.0f;
    float accumulativeDeltaTime = 0.0f;

    uint32_t frameCount = 0;

    while(!glfwWindowShouldClose(m_Window)) {

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(deltaTime >= 0) {
            s_ImGuiLayer.Begin();
            s_ImGuiLayer.End();
        }

        glfwSwapBuffers(m_Window);
        glfwPollEvents();

        endTime = (float) glfwGetTime();
        deltaTime = endTime - startTime;
        startTime = endTime;

        accumulativeDeltaTime += deltaTime;
        if(accumulativeDeltaTime >= 1.0f) {
            m_CurrentFps = frameCount;
            frameCount = 0;
            accumulativeDeltaTime = 0.0f;
        }

        frameCount++;
    }

    s_ImGuiLayer.OnDestroy();
}

void Application::CleanUp() {

    Log::Info("Closing Application");

    glfwDestroyWindow(m_Window);
    glfwTerminate();
}