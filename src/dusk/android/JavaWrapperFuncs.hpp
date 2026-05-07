#pragma once
#include <string>

namespace dusk::android {

void takeUriPermissions(const std::string& uri);

bool checkUriPermissions(const std::string& uri);


}
