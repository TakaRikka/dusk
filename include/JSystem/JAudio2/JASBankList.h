#ifndef JASBANKLIST_H
#define JASBANKLIST_H

#include <dolphin/types.h>

class JASBank;

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASBankList {
public:
    virtual const JASBank* getBank(u32 param_0) const = 0;
};

#endif /* JASBANKLIST_H */
