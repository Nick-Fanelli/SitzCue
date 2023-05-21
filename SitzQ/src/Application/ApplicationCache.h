#pragma once

#include "sitzqpch.h"

namespace SitzQ::ApplicationCache {

    namespace Keys {
        inline const std::string LastActiveProject = "LastActiveProject";
    }

    void Push(const std::string& key, const std::string& value);
    std::optional<std::string> Get(const std::string& key);
    bool Remove(const std::string& key);

    void Pull();
    void Commit();

    static inline std::unordered_map<std::string, std::string> s_CacheValues;
}