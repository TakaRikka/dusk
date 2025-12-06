#ifndef JORHOSTINFO_H
#define JORHOSTINFO_H

#include <dolphin/dolphin.h>

#define HOSTINFO_REQ_COMPUTER_NAME 0
#define HOSTINFO_REQ_USERNAME      1
#define HOSTINFO_REQ_LOCAL_TIME    2

class JORHostInfo {
public:
    /* 0x0 */ s32 m_result;

    void setResult(s32 result) { m_result = result; }
    s32 getResult() { return m_result; }

    JORHostInfo() { m_result = 0; }
    virtual ~JORHostInfo() {}
};

class JORHostInfo_String : public JORHostInfo {
public:
    virtual ~JORHostInfo_String() {}

    JORHostInfo_String() {
        m_stringBuffer = NULL;
        m_bufferSize = 0;
    }

    void setStringBuffer(char* buffer) { m_stringBuffer = buffer; }
    void setBufferSize(u32 size) { m_bufferSize = size; }
    char* getString() { return m_stringBuffer; }
    u32 getBufferSize() { return m_bufferSize; }

    /* 0x8 */ char* m_stringBuffer;
    /* 0xC */ u32 m_bufferSize;
};

class JORHostInfo_CalendarTime : public JORHostInfo {
public:
    JORHostInfo_CalendarTime() { m_calendarTimeBuffer = NULL; }
    virtual ~JORHostInfo_CalendarTime() {}

    void setCalendarTimeBuffer(OSCalendarTime* pCalendarTime) { m_calendarTimeBuffer = pCalendarTime; }
    OSCalendarTime* getCalendarTime() { return m_calendarTimeBuffer; }

    /* 0x8 */ OSCalendarTime* m_calendarTimeBuffer;
};

BOOL JORGetYearDays(int year, int mon);

#endif /* JORHOSTINFO_H */
