#include "folders.hpp"
//
#include "engine/debug/logger.hpp"

namespace fs = std::filesystem;
constexpr std::string_view FS_EXCEPTION = " std::filesystem exception : ";
static debug::Logger logger("io_folders");

std::string io::folders::trimExtensions(const std::string& filename) {
    return filename.substr(0, filename.find_first_of('.'));
}
bool io::folders::isPathValid(const fs::path& path) noexcept {
    return !path.empty() && path.string().find_first_of(" \t\n\r\f\v\"*?<>|") == std::string::npos;
}
bool io::folders::folderExists(const std::filesystem::path& path) {
    return isPathValid(path) && fs::exists(path) && fs::is_directory(path);
}
bool io::folders::fileExists(const std::filesystem::path& path) {
    return isPathValid(path) && fs::exists(path) && fs::is_regular_file(path);
}
bool io::folders::createOrCheckFolder(const std::filesystem::path& path) {
    if (!isPathValid(path)) {
        logger.error("Failed to create directory. Invalid symbols in path. Path: {}", path.string());
        return false;
    }
    try {
        if (fs::create_directory(path))
            logger.info("Created directory: {}", path.string());
        else
            logger.info("Directory already exists: {}", path.string());
        return true;
    }
    catch (const fs::filesystem_error& exception) {
        logger.error("Failed to create directory: {}{}{}", path.string(), FS_EXCEPTION, exception.what());
        return false;
    }
}
io::folders::Contents io::folders::getContents(const std::filesystem::path& path, const ContentsType type) {
    Contents contents;
    if (!folderExists(path)) {
        logger.error("Directory does not exist. Path: {}", path.string());
        return contents;
    }
    for (const auto& entry : fs::directory_iterator(path)) {
        if (type == ContentsType::folder && entry.is_directory() ||
            type == ContentsType::file && entry.is_regular_file())
            contents.push_back(entry.path().filename().string());
    }
    logger.info("Read directory: {}", path.string());
    return contents;
}
