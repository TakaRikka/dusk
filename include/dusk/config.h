#ifndef DUSK_CONFIG_H
#define DUSK_CONFIG_H

#include <aurora/aurora.h>

struct DuskConfig : public AuroraConfig {
    AuroraLogLevel logLevel;
};

extern DuskConfig duskConfig;
#endif
