#include "FileUtils.h"

using namespace SitzCue;

// ====================================================================================================================
// Validate
// ====================================================================================================================

bool FileUtils::Exists(const std::filesystem::path& path) {
    return std::filesystem::exists(path);
}

bool FileUtils::IsDirectory(const std::filesystem::path& path) {
    return std::filesystem::is_directory(path);
}

bool FileUtils::IsDirectoryEmpty(const std::filesystem::path& path) {
    if(std::filesystem::is_directory(path)) {
        return std::filesystem::is_empty(path);
    } else {
        Log::Warn("Filepath not a directory\n\tStatus: Returning True");
        return true;
    }
}

// ====================================================================================================================
// Move
// ====================================================================================================================

void FileUtils::MoveFile(const std::filesystem::path& originalPath, const std::filesystem::path& newPath) {

    if(!std::filesystem::exists(originalPath)) {
        Log::Error(std::string(originalPath) + std::string("does not exist!"));
        return;
    }

    std::filesystem::rename(originalPath, newPath);
}

// ====================================================================================================================
// Copy
// ====================================================================================================================

// ====================================================================================================================
// Create
// ====================================================================================================================

void FileUtils::CreateFile(const std::filesystem::path& filepath) {
    std::ofstream outFile(filepath);
    outFile.close();
}

void FileUtils::CreateDirectory(const std::filesystem::path& dirPath) {
    std::filesystem::create_directory(dirPath);
}

// ====================================================================================================================
// Modify
// ====================================================================================================================

void FileUtils::ChangeContentTo(const std::filesystem::path& filepath, const std::string& content) {

    try {
        std::ofstream file(filepath);

        if(file.is_open()) {
            file << content;
            file.close();
            Log::Info("Updated file content"); // TODO: Trace and add filepath
        } else {
            Log::Error("Failed to open file"); // TODO: Add filepath
        }
    } catch(const std::filesystem::filesystem_error& ex) {
        std::cerr << "Error" << ex.what() << std::endl;
    }

}

// ====================================================================================================================
// Delete
// ====================================================================================================================
void FileUtils::DeleteAllDirectoryContents(const std::filesystem::path& path) {

    if(std::filesystem::is_directory(path)) {
        for(const auto& entry : std::filesystem::directory_iterator(path)) {
            std::filesystem::remove_all(entry.path());
        }
    } else {
        Log::Error(std::string(path) + std::string(": is not a directory!"));
    }

}