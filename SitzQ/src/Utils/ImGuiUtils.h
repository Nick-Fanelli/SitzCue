#pragma once

#include <imgui.h>

namespace SitzQ {

    inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) {
        return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
    }

    inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) {
        return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
    }

    inline ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs) {
        return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y);
    }

    inline ImVec2 operator*(const ImVec2& lhs, float scalar) {
        return ImVec2(lhs.x * scalar, lhs.y * scalar);
    }

    inline ImVec2 operator*(float scalar, const ImVec2& rhs) {
        return rhs * scalar;
    }

    inline ImVec2 operator/(const ImVec2& lhs, float scalar) {
        return ImVec2(lhs.x / scalar, lhs.y / scalar);
    }
}