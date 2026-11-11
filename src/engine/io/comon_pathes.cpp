#include "folders.hpp"

namespace fs = std::filesystem;
const fs::path io::folders::RES         = "res";
const fs::path io::folders::SAVES       = "saves";
const fs::path io::folders::SCREENSHOTS = "screenshots";
const fs::path io::folders::SHADERS     = RES / "shaders";
const fs::path io::folders::CONTENT     = RES / "content";
const fs::path io::folders::MATERIALS   = CONTENT / "materials";
