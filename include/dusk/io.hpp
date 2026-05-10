#ifndef DUSK_IO_HPP
#define DUSK_IO_HPP

#include <vector>
#include <filesystem>

// I can't believe it's 2026 and neither SDL (no error codes) nor
// C++ (no error codes) have a file system API functional enough for me to use.
// Here you go, this one's inspired by C#. I only wrote the functions I need.


namespace dusk::io {

/**
 * \brief A simple file stream wrapping cstdio FILE*.
 *
 * Methods on this class throw appropriate C++ exceptions when an error occurs.
 */
class FileStream {
    FILE* file;

public:
    FileStream() noexcept;

    /**
     * \brief Take ownership of a FILE* handle.
     */
    explicit FileStream(FILE* file);
    FileStream(const FileStream& other) = delete;
    FileStream(FileStream&& other) noexcept;

    ~FileStream();

    /**
     * \brief Open a file for reading at the given path.
     */
    static FileStream OpenRead(const char* utf8Path);

    /**
     * \brief Open a file for reading at the given path.
     */
    static FileStream OpenRead(const std::filesystem::path& path);

    /**
     * \brief Create a file for writing.
     *
     * If there is an existing file, its contents are demolished.
     */
    static FileStream Create(const char* utf8Path);

    /**
     * \brief Create a file for writing.
     *
     * If there is an existing file, its contents are demolished.
     */
    static FileStream Create(const std::filesystem::path& path);

    /**
     * \brief Read the byte contents of a file directly into a vector.
     */
    static std::vector<u8> ReadAllBytes(const char* utf8Path);

    /**
     * \brief Read the byte contents of a file directly into a vector.
     */
    static std::vector<u8> ReadAllBytes(const std::filesystem::path& path);

    /**
     * \brief Read the byte contents of a file directly into a vector.
     */
    static void WriteAllText(const char* utf8Path, std::string_view text);

    /**
     * \brief Read the byte contents of a file directly into a vector.
     */
    static void WriteAllText(const std::filesystem::path& path, std::string_view text);

    /**
     * \brief Read the remaining contents of the file directly into a vector.
     */
    std::vector<u8> ReadFull();

    /**
     * Get direct access to the underlying FILE* handle.
     */
    [[nodiscard]] void* GetFileHandle() const noexcept {
        return file;
    }

    /**
     * Write data to the file.
     */
    void Write(const char* data, size_t dataLen);

    FILE* ToInner();
};

}


#endif  // DUSK_IO_HPP
