#pragma once

#include "secret_storage_core.hpp"

#include <memory>

namespace dusk::mods::svc {

std::unique_ptr<SecretStorageBackend> make_android_secret_storage_backend();

}  // namespace dusk::mods::svc
