#include "update_check.hpp"

#include "dusk/http/http.hpp"
#include "fmt/format.h"
#include "nlohmann/json.hpp"
#include "version.h"

#include <charconv>
#include <optional>
#include <utility>

namespace dusk::update_check {
namespace {

using json = nlohmann::json;

constexpr std::string_view GitHubApiVersion = "2026-03-10";

struct Version {
    int major = 0;
    int minor = 0;
    int patch = 0;

    friend auto operator<=>(const Version&, const Version&) = default;
};

std::string json_string(const json& value, const char* key) {
    const auto iter = value.find(key);
    if (iter == value.end() || !iter->is_string()) {
        return {};
    }
    return iter->get<std::string>();
}

std::optional<int> parse_component(std::string_view& value) {
    if (value.empty() || value.front() < '0' || value.front() > '9') {
        return std::nullopt;
    }

    int parsed = 0;
    const char* begin = value.data();
    const char* end = value.data() + value.size();
    const auto [ptr, ec] = std::from_chars(begin, end, parsed);
    if (ec != std::errc()) {
        return std::nullopt;
    }

    value.remove_prefix(static_cast<size_t>(ptr - begin));
    return parsed;
}

bool consume(std::string_view& value, char expected) {
    if (value.empty() || value.front() != expected) {
        return false;
    }
    value.remove_prefix(1);
    return true;
}

std::optional<Version> parse_version(std::string_view value) {
    if (!value.empty() && value.front() == 'v') {
        value.remove_prefix(1);
    }

    Version version;
    auto major = parse_component(value);
    if (!major || !consume(value, '.')) {
        return std::nullopt;
    }
    auto minor = parse_component(value);
    if (!minor || !consume(value, '.')) {
        return std::nullopt;
    }
    auto patch = parse_component(value);
    if (!patch) {
        return std::nullopt;
    }
    if (!value.empty() && value.front() != '-' && value.front() != '+') {
        return std::nullopt;
    }

    version.major = *major;
    version.minor = *minor;
    version.patch = *patch;
    return version;
}

Release parse_release(const json& value) {
    Release release{
        .tagName = json_string(value, "tag_name"),
        .name = json_string(value, "name"),
        .htmlUrl = json_string(value, "html_url"),
        .body = json_string(value, "body"),
    };

    const auto assets = value.find("assets");
    if (assets != value.end() && assets->is_array()) {
        for (const auto& asset : *assets) {
            if (!asset.is_object()) {
                continue;
            }
            release.assets.push_back({
                .name = json_string(asset, "name"),
                .browserDownloadUrl = json_string(asset, "browser_download_url"),
                .digest = json_string(asset, "digest"),
            });
        }
    }

    return release;
}

std::string release_url(std::string_view owner, std::string_view repo) {
    return fmt::format("https://api.github.com/repos/{}/{}/releases/latest", owner, repo);
}

std::string user_agent() {
    return fmt::format("Dusk/{}", DUSK_WC_DESCRIBE);
}

}  // namespace

Result check_latest_github_release(std::string_view owner, std::string_view repo) {
    if (!http::available()) {
        return {
            .status = Status::Disabled,
            .message = "No HTTP backend is available",
        };
    }
    if (owner.empty() || repo.empty()) {
        return {
            .status = Status::Failed,
            .message = "GitHub owner and repo are required",
        };
    }

    http::Request request{
        .url = release_url(owner, repo),
        .headers =
            {
                {.name = "User-Agent", .value = user_agent()},
                {.name = "Accept", .value = "application/vnd.github+json"},
                {.name = "X-GitHub-Api-Version", .value = std::string(GitHubApiVersion)},
            },
    };

    http::Result result = http::get(request);
    if (result.error != http::Error::None) {
        return {
            .status = Status::Failed,
            .message = result.message,
        };
    }
    if (result.response.statusCode != 200) {
        return {
            .status = Status::Failed,
            .message = fmt::format("GitHub returned HTTP {}", result.response.statusCode),
        };
    }

    Release latest;
    try {
        latest = parse_release(json::parse(result.response.body));
    } catch (const std::exception& e) {
        return {
            .status = Status::Failed,
            .message = fmt::format("Failed to parse GitHub release JSON: {}", e.what()),
        };
    }

    const std::optional<Version> latestVersion = parse_version(latest.tagName);
    const std::optional<Version> currentVersion = parse_version(DUSK_WC_DESCRIBE);
    if (!latestVersion) {
        return {
            .status = Status::Failed,
            .message = fmt::format("Failed to parse release tag '{}'", latest.tagName),
            .latest = std::move(latest),
        };
    }
    if (!currentVersion) {
        return {
            .status = Status::Failed,
            .message = fmt::format("Failed to parse Dusk version '{}'", DUSK_WC_DESCRIBE),
            .latest = std::move(latest),
        };
    }

    const bool updateAvailable = *latestVersion > *currentVersion;
    return {
        .status = updateAvailable ? Status::UpdateAvailable : Status::UpToDate,
        .message = updateAvailable ? "Update available" : "Dusk is up to date",
        .latest = std::move(latest),
    };
}

}  // namespace dusk::update_check
