#include "FileUtils.h"

using namespace SitzQ;

// ====================================================================================================================
// Validate
// ====================================================================================================================

bool FileUtils::Exists(const std::filesystem::path& path) {
    SITZCUE_PROFILE_FUNCTION();

    return std::filesystem::exists(path);
}

bool FileUtils::IsDirectory(const std::filesystem::path& path) {
    SITZCUE_PROFILE_FUNCTION();

    return std::filesystem::is_directory(path);
}

bool FileUtils::IsDirectoryEmpty(const std::filesystem::path& path) {

    SITZCUE_PROFILE_FUNCTION();

    if(std::filesystem::is_directory(path)) {
        return std::filesystem::is_empty(path);
    } else {
        Log::Warn("Filepath not a directory\n\tStatus: Returning True");
        return true;
    }
}

// ====================================================================================================================
// Query
// ====================================================================================================================

void FileUtils::GetLastWriteTime(const std::filesystem::path& filepath, std::filesystem::file_time_type& returnRef) {

    SITZCUE_PROFILE_FUNCTION();

    returnRef = std::filesystem::last_write_time(filepath);
}


// ====================================================================================================================
// Read
// ====================================================================================================================
void FileUtils::ReadFile(const std::filesystem::path& path, std::string& toString) {

    SITZCUE_PROFILE_FUNCTION();

    toString.clear();

    std::ifstream inputFileStream(path);

    if(inputFileStream.is_open()) {
        std::string line;

        while(std::getline(inputFileStream, line)) {
            toString += line + '\n';
        }

        inputFileStream.close();
    } else {
        Log::Error("Failed to open file");
    }
}


// ====================================================================================================================
// Move
// ====================================================================================================================

void FileUtils::MoveFile(const std::filesystem::path& originalPath, const std::filesystem::path& newPath) {

    SITZCUE_PROFILE_FUNCTION();

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

    SITZCUE_PROFILE_FUNCTION();

    FileUtils::CreateDirectory(filepath.parent_path());

    std::ofstream outFile(filepath);
    outFile.close();
}

void FileUtils::CreateDirectory(const std::filesystem::path& dirPath) {

    SITZCUE_PROFILE_FUNCTION();

    // if(!Exists(dirPath.parent_path())) {
    //     CreateDirectory(dirPath.parent_path());
    // }

    std::filesystem::create_directory(dirPath);
}

// ====================================================================================================================
// Modify
// ====================================================================================================================

void FileUtils::ChangeContentTo(const std::filesystem::path& filepath, const std::string& content) {

    SITZCUE_PROFILE_FUNCTION();

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

    SITZCUE_PROFILE_FUNCTION();

    if(std::filesystem::is_directory(path)) {
        for(const auto& entry : std::filesystem::directory_iterator(path)) {
            std::filesystem::remove_all(entry.path());
        }
    } else {
        Log::Error(std::string(path) + std::string(": is not a directory!"));
    }

}