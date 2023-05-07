#pragma once

#include "sitzcuepch.h"

namespace SitzQ {

    struct UUID {

    public:
        UUID() = default;
        UUID(uint32_t uuid) : m_UUID(uuid) {}

        operator uint32_t() const { return m_UUID; }

        bool operator==(UUID& other) const { return other.m_UUID == m_UUID; }

    private:

        uint32_t m_UUID;

    };
}