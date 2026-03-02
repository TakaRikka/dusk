#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JHostIO/JORHostInfo.h"
#include "JSystem/JHostIO/JORServer.h"

// End of each month in standard year
static s32 YearDays[] = {
    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334,
};

// End of each month in leap year
static s32 LeapYearDays[] = {
    0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335,
};

static BOOL IsLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

BOOL JORGetYearDays(int year, int mon) {
    s32* days = IsLeapYear(year) ? LeapYearDays : YearDays;
    return days[mon];
}

bool JORGetHostInfo(u32 request_type, JORHostInfo* pHostinfo) {
    JORMContext* mctx = JORServer::getInstance()->attachMCTX(MCTX_MSG_GET_HOST_INFO);
    mctx->sendHostInfoRequest(request_type, pHostinfo);
    JORServer::getInstance()->releaseMCTX(mctx);

    while (pHostinfo->getResult() == 0) {
        JOR_MESSAGELOOP();
    }

    return pHostinfo->getResult() == 1;
}

void JORGetComputerName(char* buffer, u32 buffer_size) {
    JORHostInfo_String string;
    string.setStringBuffer(buffer);
    string.setBufferSize(buffer_size);

    JORGetHostInfo(HOSTINFO_REQ_COMPUTER_NAME, &string);
}

void JORGetUserName(char* buffer, u32 buffer_size) {
    JORHostInfo_String string;
    string.setStringBuffer(buffer);
    string.setBufferSize(buffer_size);

    JORGetHostInfo(HOSTINFO_REQ_USERNAME, &string);
}

void JORGetLocalTime(OSCalendarTime* pCalendarTime) {
    JORHostInfo_CalendarTime calendar_time;
    calendar_time.setCalendarTimeBuffer(pCalendarTime);

    JORGetHostInfo(HOSTINFO_REQ_LOCAL_TIME, &calendar_time);
}
