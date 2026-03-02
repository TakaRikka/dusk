#ifndef JASREPORT_H
#define JASREPORT_H

class JKRHeap;

void JASReportInit(JKRHeap*, int);
int JASReportGetLineMax();
int JASReportCopyBuffer(char *, int);
void JASReport(const char* message, ...);

#endif /* JASREPORT_H */
