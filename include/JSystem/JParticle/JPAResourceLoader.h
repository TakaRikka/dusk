#ifndef JPARESOURCELOADER_H
#define JPARESOURCELOADER_H

#include "dolphin/types.h"

class JPAResourceManager;

class JPAResourceLoader {
public:
    JPAResourceLoader(u8 const*, JPAResourceManager*);
    void load_jpc(u8 const*, JPAResourceManager*);
};

#endif /* JPARESOURCELOADER_H */
