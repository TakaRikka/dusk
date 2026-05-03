#pragma once
#include <string>

namespace dusk::android {

void setupMethods();

void takeUriPermissions(const std::string& uri);

bool checkUriPermissions(const std::string& uri);


}
