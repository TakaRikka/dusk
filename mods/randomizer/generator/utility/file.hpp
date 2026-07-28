#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "../utility/path.hpp"

#ifdef DEVKITPRO
    #include <sys/stat.h>
    #include <dirent.h>
#endif

namespace randomizer::utility::file
{
    //std::filesystem is partially broken on Wii U, these are cross-platform replacements

    inline std::ostream& seek(std::ostream& stream, const std::streamoff& off, const std::ios::seekdir& way = std::ios::beg) {
        //#ifdef DEVKITPRO
        //Wii U crashes if you seek past eof, most other platforms extend the file
        //Handle writing the extra padding manually

        switch(way) {
            case std::ios::cur:
            {
                const std::streamoff& cur = stream.tellp();
                if(off > 0) {
                    stream.seekp(0, std::ios::end);
                    if(stream.tellp() < (cur + off)) {
                        const std::string buffer((cur + off) - stream.tellp(), '\0');
                        stream.write(&buffer[0], buffer.size());
                    }
                }
                else if ((cur + off) < 0) {
                    //can't seek before start of file, seek to beginning as failsafe
                    return stream.seekp(0, std::ios::beg);
                }
                return stream.seekp(cur + off, std::ios::beg);
            }
            case std::ios::end:
            //BUG: seek to std::ios::end doesn't seem to work on MLC, find workaround? (relevant uses are currently replaced)
            {
                stream.seekp(0, std::ios::end);
                if(off > 0) {
                    const std::string buffer(off, '\0');
                    stream.write(&buffer[0], buffer.size());
                }
                else if((-off) > stream.tellp()) {
                    //Can't seek before start of file, seek to beginning as failsafe
                    return stream.seekp(0, std::ios::beg);
                }
                return stream.seekp(off, std::ios::end);
            }
            case std::ios::beg:
                [[fallthrough]];
            default:
            {
                if(off < 0) {
                    //can't seek before start of file, seek to beginning as failsafe
                    stream.seekp(0, std::ios::beg);
                }
                stream.seekp(0, std::ios::end);
                if(stream.tellp() < off) {
                    const std::string buffer(off - stream.tellp(), '\0');
                    stream.write(&buffer[0], buffer.size());
                }
                return stream.seekp(off, std::ios::beg);
            }
        }
        //#else
        //    return stream.seekp(off, way);
        //#endif
    }

    //from https://github.com/emiyl/dumpling/blob/5dc5131243385050e45339779e75a2eaad31f1e4/source/app/filesystem.cpp#L177
    bool isRoot(const fspath& fsPath);

    //from https://github.com/emiyl/dumpling/blob/5dc5131243385050e45339779e75a2eaad31f1e4/source/app/filesystem.cpp#L193
    inline bool dirExists(const fspath& fsPath) {
        #ifdef DEVKITPRO
            static struct stat existStat;
            if (isRoot(fsPath)) return true;
            if (lstat(fsPath.string().c_str(), &existStat) == 0 && S_ISDIR(existStat.st_mode)) return true;
            return false;
        #else
            return std::filesystem::is_directory(fsPath);
        #endif
    }

    inline bool create_directories(const fspath& fsPath) {
        #ifdef DEVKITPRO
            std::string temp = fsPath.string();
            if(temp.back() == '/') temp.pop_back();
            for(size_t i = 0; i < temp.size(); i++) {
                if(temp[i] == '/') {
                    const std::string& sub = temp.substr(0, i);
                    if (!dirExists(sub)) {
                        mkdir(sub.c_str(), ACCESSPERMS);
                    }
                }
            }
            mkdir(temp.c_str(), ACCESSPERMS);
        #else
            std::filesystem::create_directories(fsPath);
        #endif

        return true;
    }

    bool copy_file(const fspath& from, const fspath& to);

    bool copy(const fspath& from, const fspath& to);

    int GetContents(const fspath& filename, std::string& fileContents, bool resourceFile = false);

    int GetContents(const fspath& filename, std::stringstream& fileContents);

    void Verify(const fspath& filename);
}
