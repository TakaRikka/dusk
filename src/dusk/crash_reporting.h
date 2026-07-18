#pragma once

namespace dusk::crash_reporting {

enum class Consent {
    Unavailable,
    Unknown,
    Given,
    Revoked,
};

void initialize();
void shutdown();
Consent get_consent();
void set_consent(bool enabled);

}  // namespace dusk::crash_reporting
