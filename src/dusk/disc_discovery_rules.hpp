#pragma once

// Pure rules for locating disc images without a file dialog (tvOS). No Dusklight dependencies,
// so tests/disc_discovery can build this standalone on the host.

#include <algorithm>
#include <array>
#include <cctype>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace dusk::disc_discovery {

inline constexpr std::array<std::string_view, 9> kDiscExtensions{
    "iso", "gcm", "ciso", "gcz", "nfs", "rvz", "wbfs", "wia", "tgc"};

struct Candidate {
    std::filesystem::path path;
    std::string_view origin;  // "bundle" (inside the app bundle) or "data" (app data directory)
};

inline bool is_disc_filename(const std::filesystem::path& path) noexcept {
    std::string ext = path.extension().string();
    if (ext.size() < 2 || ext.front() != '.') {
        return false;
    }
    ext.erase(0, 1);
    std::transform(ext.begin(), ext.end(), ext.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return std::find(kDiscExtensions.begin(), kDiscExtensions.end(), ext) != kDiscExtensions.end();
}

inline int origin_rank(std::string_view origin) noexcept {
    if (origin == "bundle") {
        return 0;
    }
    if (origin == "data") {
        return 1;
    }
    return 2;
}

// Keeps disc files, removes duplicate paths (first occurrence wins), then orders by origin rank
// (bundle before data) and filename, independent of the input order.
inline std::vector<Candidate> select_candidates(std::vector<Candidate> found) {
    std::vector<Candidate> out;
    out.reserve(found.size());
    for (auto& c : found) {
        if (!is_disc_filename(c.path)) {
            continue;
        }
        const bool dup = std::any_of(out.begin(), out.end(),
            [&](const Candidate& o) { return o.path == c.path; });
        if (!dup) {
            out.push_back(std::move(c));
        }
    }
    std::stable_sort(out.begin(), out.end(), [](const Candidate& a, const Candidate& b) {
        const int ra = origin_rank(a.origin);
        const int rb = origin_rank(b.origin);
        if (ra != rb) {
            return ra < rb;
        }
        return a.path.filename().string() < b.path.filename().string();
    });
    return out;
}

}  // namespace dusk::disc_discovery
