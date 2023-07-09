#include "Display.h"

#include "Application.h"

using namespace SitzQ;

static constexpr int s_DisplayWidth = 1600;
static constexpr int s_DisplayHeight = 900;

void Display::CreateDisplay() {

    SITZCUE_PROFILE_FUNCTION();

    Log::Trace("Initializing GLFW");
    if(!glfwInit()) { // Initialize GLFW
        Log::Error("Failed to initialize GLFW!\n\tStatus: Returning");
        return;
    }

    // Setup GLFW window hints to be compat with all OS
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Create the window to the window ptr
    m_Window = glfwCreateWindow(s_DisplayWidth, s_DisplayHeight, "SitzQ", nullptr, nullptr);

    if(m_Window == nullptr) { // Check to see if the window exists
        glfwTerminate();
        Log::Error("Could not create the GLFW Window");
        return;
    }

    glfwSwapInterval(true); // Enable V-Sync

    int width, height;

    // Set window size to 16x9 ratio

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    width = mode->width;
    height = mode->height;

    int newWidth = (int) ((float) (900 * width) / (float) height);
    int newHeight = (int) ((float) (1600 * height) / (float) width);

    glfwSetWindowSize(m_Window, newWidth, newHeight);

    int screenWidth, screenHeight;

    glfwGetFramebufferSize(m_Window, &screenWidth, &screenHeight);
    glfwMakeContextCurrent(m_Window);

    glfwWindowHint(GLFW_SAMPLES, 4);

    Log::Trace("Loading GLAD");
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) { // Initialize GLAD
        std::cout << "GLAD FAILED" << std::endl;
        Log::Error("Failed to initialize GLAD");
        return;
    }

    // Define the viewport
    glViewport(0, 0, screenWidth, screenHeight);

    // glEnable(GL_DEPTH_TEST);

    // glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwFocusWindow(m_Window);
}

void Display::GetImGuiSize(ImVec2* outVec2) {
    int x, y;
    glfwGetWindowSize(m_Window, &x, &y);

    outVec2->x = (float) x;
    outVec2->y = (float) y;
}

void Display::StartApplicationLoop(Application* application) {

    SITZCUE_PROFILE_FUNCTION();

    application->OnCreate();

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    float currentFrame = (float) glfwGetTime();

    while(!glfwWindowShouldClose(m_Window)) {

        SITZCUE_PROFILE_SCOPE("Update Loop");

        currentFrame = (float) glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        application->OnUpdate(deltaTime);

        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    application->OnDestroy();

    CleanUp();
}

void Display::CleanUp() {

    SITZCUE_PROFILE_FUNCTION();

    Log::Trace("Closing Application");

    glfwDestroyWindow(m_Window);
    glfwTerminate();

    Log::Info("GLFW Terminated");
}