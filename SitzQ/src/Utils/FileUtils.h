#pragma once

#include "sitzcuepch.h"

namespace SitzQ::FileUtils {

    // Validate
    bool Exists(const std::filesystem::path& path);
    bool IsDirectory(const std::filesystem::path& path);
    bool IsDirectoryEmpty(const std::filesystem::path& path);

    // Move
    void MoveFile(const std::filesystem::path& originalPath, const std::filesystem::path& newPath);

    // Copy

    // Create
    void CreateFile(const std::filesystem::path& filepath);
    void CreateDirectory(const std::filesystem::path& dirPath);

    // Modify
    void ChangeContentTo(const std::filesystem::path& filepath, const std::string& content);

    // Delete
    void DeleteAllDirectoryContents(const std::filesystem::path& path);

}