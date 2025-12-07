#include "m_Do/m_Do_hostIO.h"
#include <dolphin/dolphin.h>

#if DEBUG

mDoHIO_root_c mDoHIO_root;

mDoHIO_root_c::~mDoHIO_root_c() {}

mDoHIO_subRoot_c::~mDoHIO_subRoot_c() {}

mDoHIO_child_c::~mDoHIO_child_c() {}

void mDoHIO_root_c::genMessage(JORMContext* i_context) {
    i_context->genNode("Node", &mSub, 0, 0);
}

void mDoHIO_subRoot_c::genMessage(JORMContext* i_context) {
    for (int i = 0; i < 80; i++) {
        JORReflexible* node = mChildren[i].getPt();

        if (node != NULL) {
            i_context->genNode(mChildren[i].getName(), node, 0, 0);
        }
    }
}

void mDoHIO_root_c::update() {
    JORMContext* context = attachJORMContext(5);
    context->invalidNode(this, 3);
    releaseJORMContext(context);
}

s8 mDoHIO_subRoot_c::createChild(const char* i_name, JORReflexible* i_node) {
    for (int i = 0; i < 80; i++) {
        if (mChildren[i].getPt() == i_node) {
            OSReport_Error("危険：既に登録されているホストIOをふたたび登録\nしようとしているのを発見しました。<%s>%08x\n削除処理が正しく呼ばれていない可能性があります。\n登録と削除は１：１で呼ぶように修正してください。\n", i_name, i_node);
            return -1;
        }
    }

    for (int i = 0; i < 80; i++) {
        if (mChildren[i].getPt() == NULL) {
            mChildren[i].setName(i_name);
            mChildren[i].setPt(i_node);
            return i + 1;
        }
    }

    OSReport_Error("ホストIOの空きエントリがありません。登録できませんでした。<%s>\n", i_name);
    return -1;
}

void mDoHIO_subRoot_c::deleteChild(s8 i_no) {
    i_no--;
    
    if (i_no >= 0) {
        if (mChildren[i_no].getPt() == NULL) {
            OSReport_Error("危険：削除済みホストIOをさらに削除しようとしています<%s>\n", mChildren[i_no].getName());
        } else {
            mChildren[i_no].setPt(NULL);
        }
    } else {
        OSReport_Error("mDoHIO_subRoot_c::deleteChild\n");
    }
}

void mDoHIO_subRoot_c::updateChild(s8 i_no) {
    i_no--;
    
    if (i_no >= 0) {
        JORMContext* context = attachJORMContext(5);
        context->invalidNode(mChildren[i_no].getPt(), 3);
        releaseJORMContext(context);
    }
}

mDoHIO_entry_c::mDoHIO_entry_c() {
    mNo = -1;
    mCount = 0;
}

mDoHIO_entry_c::~mDoHIO_entry_c() {
    if (mCount != 0) {
        OSReport_Error("~mDoHIO_entry_c mCount=%d mNo=%d\n", mCount, mNo);
        mDoHIO_deleteChild(mNo);
        mDoHIO_deleteChild(mNo);  
    }
}

void mDoHIO_deleteChild(s8 i_no) {
    mDoHIO_root.deleteChild(i_no);
}

void mDoHIO_root_c::deleteChild(s8 i_no) {
    mSub.deleteChild(i_no);
}

void mDoHIO_entry_c::entryHIO(const char* i_name) {
    if (mCount >= 127) {
        // "mDoHIO_entry_c::entryHIO too many calls\n"
        OSReport_Error("mDoHIO_entry_c::entryHIO 呼びすぎです\n");
        return;
    }
    
    if (mNo == -1 && i_name != NULL) {
        mNo = mDoHIO_createChild(i_name, this);
    }

    mCount++;
}

void mDoHIO_entry_c::removeHIO() {
    if (mCount != 0) {
        mCount--;

        if (mCount == 0 && mNo != -1) {
            mDoHIO_deleteChild(mNo);
            mNo =-1;
        }
    } else {
        OSReport_Error("mDoHIO_entry_c::removeHIO 呼びすぎです\n");
    }
}

void mDoHIO_updateChild(s8 i_no) {
    mDoHIO_root.updateChild(i_no);
}

void mDoHIO_root_c::updateChild(s8 i_no) {
    mSub.updateChild(i_no);
}

#endif
