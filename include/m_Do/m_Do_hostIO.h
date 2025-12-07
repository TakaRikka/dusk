#ifndef M_DO_M_DO_HOSTIO_H
#define M_DO_M_DO_HOSTIO_H

#include "JSystem/JHostIO/JORReflexible.h"
#include "JSystem/JHostIO/JORMContext.h"
#include <string.h>

class mDoHIO_child_c {
public:
    mDoHIO_child_c() {
        unk18 = 0;
        mPt = NULL;
    }

    ~mDoHIO_child_c();

    void setPt(JORReflexible* i_pt) { mPt = i_pt; }
    void setName(const char* i_name) { strncpy(mName, i_name, sizeof(mName)); }
    JORReflexible* getPt() { return mPt; }
    const char* getName() { return mName; }

    /* 0x00 */ char mName[24];
    /* 0x18 */ u8 unk18;
    /* 0x1C */ JORReflexible* mPt;
};

class mDoHIO_entry_c : public JORReflexible {
public:
#if DEBUG
    mDoHIO_entry_c();
    void entryHIO(const char* i_name);
    void removeHIO();

    virtual ~mDoHIO_entry_c();

    /* 0x4 */ s8 mNo;
    /* 0x5 */ u8 mCount;
#else
    virtual ~mDoHIO_entry_c() {}
#endif
};

class mDoHIO_subRoot_c : public JORReflexible {
public:
    mDoHIO_subRoot_c() {}
    void updateChild(s8);
    void deleteChild(s8);
    s8 createChild(const char*, JORReflexible*);

    virtual void genMessage(JORMContext*);
    virtual ~mDoHIO_subRoot_c();

    /* 0x4 */ mDoHIO_child_c mChildren[80];
};

class mDoHIO_root_c : public JORReflexible {
public:
    mDoHIO_root_c() {}
    void update();
    void updateChild(s8);
    void deleteChild(s8);
    int createChild(const char*, JORReflexible*);

    virtual void genMessage(JORMContext*);
    virtual ~mDoHIO_root_c();

    /* 0x4 */ mDoHIO_subRoot_c mSub;
};

extern mDoHIO_root_c mDoHIO_root;

void mDoHIO_updateChild(s8 i_no);
void mDoHIO_deleteChild(s8 i_no);
s8 mDoHIO_createChild(const char*, JORReflexible*);

#if DEBUG
#define mDoHIO_CREATE_CHILD(i_name, i_node) mDoHIO_createChild(i_name, i_node)
#define mDoHIO_DELETE_CHILD(i_no)           mDoHIO_deleteChild(i_no)
#else
#define mDoHIO_CREATE_CHILD(i_name, i_node) (-1)
#define mDoHIO_DELETE_CHILD(i_no)           (void)0
#endif

#endif /* M_DO_M_DO_HOSTIO_H */
