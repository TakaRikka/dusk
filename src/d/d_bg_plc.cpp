#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_bg_plc.h"
#include "JSystem/JUtility/JUTAssert.h"

dBgPlc::dBgPlc() {
    m_base = NULL;
}

dBgPlc::~dBgPlc() {}

void dBgPlc::setBase(void* pbase) {
    m_base = (sBgPlc*)pbase;
}

sBgPc* dBgPlc::getCode(int index, sBgPc** ppcode) const {
    JUT_ASSERT(45, m_base != NULL);
    JUT_ASSERT(46, m_base->m_code_size == ZELDA_CODE_SIZE);
    JUT_ASSERT(47, index >= 0 && index< m_base->m_num);
    sBgPc* code = m_base->m_code;
    code += index;

    *ppcode = code;
    return code;
}

u32 dBgPlc::getGrpCode(int index) const {
    JUT_ASSERT(62, m_base != NULL);
    JUT_ASSERT(63, m_base->m_code_size == ZELDA_CODE_SIZE);
    JUT_ASSERT(64, 0 <= index && index < m_base->m_num);
    sBgPc* code = m_base->m_code;
    code += index;

    return code->code4;
}
