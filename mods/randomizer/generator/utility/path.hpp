#pragma once

#include <filesystem>

#ifdef QT_GUI
    #include <QString>
#endif

using fspath = std::filesystem::path;

namespace Utility {
    fspath get_data_path();
    fspath get_app_save_path();
    fspath get_logs_path();
    fspath get_temp_dir();

    // On Windows, fspath.string() will throw an exception if the character requires some kind of Unicode/non-ANSI representation
    // using .u8string() fixes this, but a lot of the randomizer still expects std::string which is not implicitly convertible
    // std::string should still properly store a UTF-8 string, so this wrapper does that conversion
    inline std::string toUtf8String(const fspath& path) {
        const std::u8string& pathStr = path.u8string();
        return std::string(pathStr.begin(), pathStr.end());
    }

    #ifdef QT_GUI
        // Wrapper for path -> QString
        // Use a wide string type to cover Windows where paths are UTF-16 encoded (and hopefully still be fine on other platforms)
        // Also use the "generic" version with '/' separators because the Windows '\' breaks some paths
        inline QString toQString(const fspath& path) { return QString::fromStdU32String(path.generic_u32string()); }
        inline fspath fromQString(const QString& path) { return path.toStdU32String(); }
    #endif
}
