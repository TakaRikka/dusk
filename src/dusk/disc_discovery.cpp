#include "dusk/disc_discovery.hpp"

#include "dusk/data.hpp"

#include <borealis/file_select.hpp>
#include <borealis/log.hpp>

#include <system_error>

namespace dusk::disc_discovery {
namespace {

constexpr borealis::Log DiscoveryLog{"dusk::disc_discovery"};

void collect(
    const std::filesystem::path& dir, std::string_view origin, std::vector<Candidate>& out) {
    std::error_code ec;
    if (dir.empty() || !std::filesystem::is_directory(dir, ec)) {
        return;
    }
    for (std::filesystem::directory_iterator it{dir, ec}, end; !ec && it != end; it.increment(ec)) {
        std::error_code fileEc;
        if (it->is_regular_file(fileEc)) {
            out.push_back(Candidate{it->path(), origin});
        }
    }
}

}  // namespace

bool needed() noexcept {
    return !borealis::file_select::capabilities().canOpenFile;
}

std::vector<Candidate> scan() {
    const std::filesystem::path bundlePath = data::base_path_relative("disc");
    const std::filesystem::path dataPath = data::configured_data_path();
    const std::filesystem::path dataDiscsPath = dataPath / "discs";
    DiscoveryLog.info("disc discovery: scanning bundle '{}', data '{}', data/discs '{}'",
        bundlePath.string(), dataPath.string(), dataDiscsPath.string());

    std::vector<Candidate> found;
    collect(bundlePath, "bundle", found);
    collect(dataDiscsPath, "data", found);
    collect(dataPath, "data", found);
    auto selected = select_candidates(std::move(found));
    DiscoveryLog.info("disc discovery: {} candidate(s)", selected.size());
    for (const auto& c : selected) {
        DiscoveryLog.info("  [{}] {}", c.origin, c.path.string());
    }
    return selected;
}

}  // namespace dusk::disc_discovery
