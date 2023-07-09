#include "ApplicationCache.h"

#include <yaml-cpp/yaml.h>

#include "Utils/FileUtils.h"

using namespace SitzQ;

void ApplicationCache::Push(const std::string& key, const std::string& value) {
    s_CacheValues[key] = value;
}

std::optional<std::string> ApplicationCache::Get(const std::string& key) {
    if(s_CacheValues.find(key) != s_CacheValues.end()) {
        return { s_CacheValues[key] };
    } else {
        return {};
    }
}

bool ApplicationCache::Remove(const std::string& key) {
    size_t removedElements = s_CacheValues.erase(key);

    return removedElements > 0;
}

// Pull the application cache files data into memory
void ApplicationCache::Pull() {
    s_CacheValues.clear(); // Clear the current cached values

    // Stream in the file
    std::ifstream in(PlatformDetection::GetApplicationCacheFilePath());
    std::stringstream stream;
    stream << in.rdbuf();
    in.close();

    // Convert the YAML file to an unordered map of std::string, std::string to be read later
    YAML::Node node = YAML::Load(stream.str());
    YAML::convert<std::unordered_map<std::string, std::string>>::decode(node, s_CacheValues);
}

void ApplicationCache::Commit() {

    Log::Trace("Committing Application Cache");

    YAML::Node node = YAML::convert<std::unordered_map<std::string, std::string>>::encode(s_CacheValues);

    std::ofstream file(PlatformDetection::GetApplicationCacheFilePath());

    if(file.is_open()) {
        file << node;
        file.close();
    }
}