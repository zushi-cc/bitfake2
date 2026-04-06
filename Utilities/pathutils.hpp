#pragma once
#include <filesystem>
#include <string>
namespace bitfake {
namespace pathutils {
inline std::string pathToString(const std::filesystem::path &p) {
#ifdef _WIN32
    // On Windows, path is UTF-16, convert to UTF-8 string
    return p.string();
#else
    return p.string(); // UNIX: already UTF-8
#endif
}
}}