#include "fmt/format.h"
#include "loader.hpp"

#include <span>

namespace dusk::mods {

ModBundleZip::ModBundleZip(std::vector<u8>&& data) : zip_data(std::move(data)) {
    if (!mz_zip_reader_init_mem(&res_zip, zip_data.data(), zip_data.size(), 0)) {
        const auto error = mz_zip_get_last_error(&res_zip);
        throw std::runtime_error(
            fmt::format("Opening zip failed: {}", mz_zip_get_error_string(error)));
    }
}

ModBundleZip::~ModBundleZip() {
    mz_zip_reader_end(&res_zip);
}

std::vector<u8> ModBundleZip::readFile(const std::string& fileName) {
    std::lock_guard lock{m_mutex};
    size_t size;
    const auto ptr = mz_zip_reader_extract_file_to_heap(&res_zip, fileName.c_str(), &size, 0);

    if (!ptr) {
        throw std::runtime_error(fmt::format("File does not exist: {}", fileName));
    }

    std::span data(static_cast<u8*>(ptr), size);
    std::vector vec(data.begin(), data.end());

    mz_free(ptr);

    return vec;
}

std::vector<std::string> ModBundleZip::getFileNames() {
    std::lock_guard lock{m_mutex};
    std::vector<std::string> results;

    for (mz_uint i = 0, n = mz_zip_reader_get_num_files(&res_zip); i < n; ++i) {
        mz_zip_archive_file_stat stat{};
        if (!mz_zip_reader_file_stat(&res_zip, i, &stat)) {
            continue;
        }
        if (mz_zip_reader_is_file_a_directory(&res_zip, i)) {
            continue;
        }

        results.emplace_back(stat.m_filename);
    }

    return results;
}

size_t ModBundleZip::getFileSize(const std::string& fileName) {
    std::lock_guard lock{m_mutex};
    const auto idx = mz_zip_reader_locate_file(&res_zip, fileName.c_str(), nullptr, 0);
    if (idx < 0) {
        throw std::runtime_error(fmt::format("Unable to locate file in zip: {}", fileName));
    }

    mz_zip_archive_file_stat stat{};
    mz_zip_reader_file_stat(&res_zip, idx, &stat);
    return stat.m_uncomp_size;
}

}  // namespace dusk::mods
