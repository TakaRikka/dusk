#pragma once

#if defined(__has_include)
#if __has_include(<tracy/Tracy.hpp>)
#include <tracy/Tracy.hpp>
#endif
#endif

#ifndef ZoneScoped
#define ZoneScoped
#define ZoneScopedN(name)
#endif
