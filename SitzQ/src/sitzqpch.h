#pragma once

// STD-LIB
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <algorithm>
#include <regex>
#include <memory>
#include <thread>
#include <utility>

#include <string>
#include <stack>
#include <array>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <random>

#include <filesystem>

// Internal
#include "PlatformDetection.h"
#include "Log.h"

#include "Debug/Assert.h"
#include "Debug/Profile.h"

// Vendor
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include "Utils/ImGuiUtils.h"