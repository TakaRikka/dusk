#include "path.hpp"
#include "file.hpp"

#if defined(QT_GUI)
    #if defined(__APPLE__)
        #include <QStandardPaths>
    #else
        #include <QCoreApplication>
    #endif
#endif

namespace Utility {
    fspath get_data_path() {
        #if defined(QT_GUI)
            #if defined(EMBED_DATA)
                return ":/";
            #else
                return fromQString(QCoreApplication::applicationDirPath()) / "data/";
            #endif
        #elif defined(DEVKITPRO)
            return "/vol/content/";
        #elif defined(APPLE)
            return "../../../data/";
        #else
            return "./data/";
        #endif
    }

    fspath get_app_save_path() {
        fspath path;
        #if defined(__APPLE__) && defined(QT_GUI)
            path = fromQString(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
        #elif defined(QT_GUI)
            path = fromQString(QCoreApplication::applicationDirPath());
        #elif defined(DEVKITPRO)
            return "/vol/save/";
        #else
            return "./";
        #endif
        
        if (!std::filesystem::is_directory(path))
        {
            randomizer::utility::file::create_directories(path);
        }

        return path;
    }

    fspath get_logs_path() {
        const fspath path = get_app_save_path() / "logs/";

        if (!std::filesystem::is_directory(path))
        {
            randomizer::utility::file::create_directories(path);
        }

        return path;
    }

    fspath get_temp_dir() {
        // could get the OS-provided temp folder with Qt but it might be harder to find and debug should we use it for anything
        const fspath path = get_app_save_path() / "temp/";

        if (!std::filesystem::is_directory(path))
        {
            randomizer::utility::file::create_directories(path);
        }

        return path;
    }
}
