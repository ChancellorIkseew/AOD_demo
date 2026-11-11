#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace io::folders {
    namespace fs = std::filesystem;
    extern const fs::path RES;
    extern const fs::path SAVES;
    extern const fs::path SCREENSHOTS;
    extern const fs::path SHADERS;
    extern const fs::path CONTENT;
    extern const fs::path MATERIALS;

    using Contents = std::vector<std::string>;
    enum class ContentsType : uint8_t { folder, file };

    std::string trimExtensions(const std::string& filename);
    bool isPathValid(const fs::path& path) noexcept;
    bool folderExists(const fs::path& path);
    bool fileExists(const fs::path& path);
    bool createOrCheckFolder(const fs::path& path);
    Contents getContents(const fs::path& path, const ContentsType type);
}
