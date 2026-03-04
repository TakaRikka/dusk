#include <revolution.h>
#include <revolution/base/PPCArch.h>

void PMBegin(void) {
    PPCMtmmcr0(0);
    PPCMtmmcr1(0);
    PPCMtpmc1(0);
    PPCMtpmc2(0);
    PPCMtpmc3(0);
    PPCMtpmc4(0);
    PPCMtmmcr0(0x4F);
    PPCMtmmcr1(0x78800000);
}

void PMEnd(void) {
    PPCMtmmcr0(0);
    PPCMtmmcr1(0);
}

void PMCycles(void) {
    PPCMfpmc1();
}

void PML1FetchMisses(void) {
    PPCMfpmc2();
}

void PML1MissCycles(void) {
    PPCMfpmc3();
}

void PMInstructions(void) {
    PPCMfpmc4();
}
