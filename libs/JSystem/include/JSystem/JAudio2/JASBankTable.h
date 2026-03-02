#ifndef JASBANKTABLE_H
#define JASBANKTABLE_H

#include "JSystem/JAudio2/JASBankList.h"
#include "JSystem/JAudio2/JASGadget.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
template<size_t N>
class JASBankTable : public JASBankList {
public:
    JASBank* getBank(u32 param_0) { return mArray.get(param_0); }
    virtual JASBank* getBank(u32 param_0) const { return mArray.get(param_0); }
    void registBank(u32 param_0, JASBank* param_1) { mArray.set(param_0, param_1); }

private:
    JASPtrArray<JASBank, N> mArray;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASDefaultBankTable : public JASBankTable<256>, public JASGlobalInstance<JASDefaultBankTable> {
public:
    JASDefaultBankTable() : JASGlobalInstance(true) {}
};

#endif /* JASBANKTABLE_H */
