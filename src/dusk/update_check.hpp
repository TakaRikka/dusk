#ifndef DUSK_UPDATE_CHECK_HPP
#define DUSK_UPDATE_CHECK_HPP

#include <string>
#include <string_view>
#include <vector>

namespace dusk::update_check {

enum class Status {
    Disabled,
    UpToDate,
    UpdateAvailable,
    Failed,
};

struct Asset {
    std::string name;
    std::string browserDownloadUrl;
    std::string digest;
};

struct Release {
    std::string tagName;
    std::string name;
    std::string htmlUrl;
    std::string body;
    std::vector<Asset> assets;
};

struct Result {
    Status status = Status::Failed;
    std::string message;
    Release latest;
};

Result check_latest_github_release(std::string_view owner, std::string_view repo);

}  // namespace dusk::update_check

#endif  // DUSK_UPDATE_CHECK_HPP
