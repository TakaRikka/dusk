#include <dolphin.h>
#include <dolphin/ax.h>
#include <dolphin/axfx.h>

#include "__axfx.h"

static f32 rsmpTab12khz[512] = {
    0.097503662109f,  0.802215576172f,  0.101593017578f,  -0.000976562500f, 0.093505859375f,
    0.802032470703f,  0.105804443359f,  -0.001037597656f, 0.089599609375f,  0.801696777344f,
    0.110107421875f,  -0.001159667969f, 0.085784912109f,  0.801177978516f,  0.114471435547f,
    -0.001281738281f, 0.082031250000f,  0.800476074219f,  0.118927001953f,  -0.001403808594f,
    0.078369140625f,  0.799621582031f,  0.123474121094f,  -0.001525878906f, 0.074798583984f,
    0.798614501953f,  0.128143310547f,  -0.001647949219f, 0.071350097656f,  0.797424316406f,
    0.132873535156f,  -0.001770019531f, 0.067962646484f,  0.796051025391f,  0.137695312500f,
    -0.001922607422f, 0.064697265625f,  0.794525146484f,  0.142608642578f,  -0.002044677734f,
    0.061492919922f,  0.792846679688f,  0.147613525391f,  -0.002197265625f, 0.058349609375f,
    0.790985107422f,  0.152709960938f,  -0.002319335938f, 0.055328369141f,  0.788940429688f,
    0.157897949219f,  -0.002471923828f, 0.052368164062f,  0.786743164062f,  0.163177490234f,
    -0.002655029297f, 0.049499511719f,  0.784423828125f,  0.168518066406f,  -0.002807617188f,
    0.046722412109f,  0.781890869141f,  0.173980712891f,  -0.002990722656f, 0.044006347656f,
    0.779205322266f,  0.179504394531f,  -0.003143310547f, 0.041412353516f,  0.776367187500f,
    0.185119628906f,  -0.003326416016f, 0.038879394531f,  0.773376464844f,  0.190826416016f,
    -0.003509521484f, 0.036407470703f,  0.770233154297f,  0.196594238281f,  -0.003692626953f,
    0.034027099609f,  0.766937255859f,  0.202484130859f,  -0.003875732422f, 0.031738281250f,
    0.763488769531f,  0.208435058594f,  -0.004058837891f, 0.029510498047f,  0.759857177734f,
    0.214447021484f,  -0.004272460938f, 0.027374267578f,  0.756103515625f,  0.220550537109f,
    -0.004455566406f, 0.025299072266f,  0.752197265625f,  0.226745605469f,  -0.004669189453f,
    0.023315429688f,  0.748168945312f,  0.233001708984f,  -0.004852294922f, 0.021392822266f,
    0.743988037109f,  0.239318847656f,  -0.005065917969f, 0.019561767578f,  0.739654541016f,
    0.245727539062f,  -0.005310058594f, 0.017791748047f,  0.735198974609f,  0.252197265625f,
    -0.005523681641f, 0.016052246094f,  0.730590820312f,  0.258728027344f,  -0.005706787109f,
    0.014404296875f,  0.725860595703f,  0.265350341797f,  -0.005920410156f, 0.012817382812f,
    0.721008300781f,  0.272033691406f,  -0.006164550781f, 0.011322021484f,  0.716003417969f,
    0.278778076172f,  -0.006378173828f, 0.009887695312f,  0.710906982422f,  0.285583496094f,
    -0.006561279297f, 0.008514404297f,  0.705657958984f,  0.292449951172f,  -0.006774902344f,
    0.007202148438f,  0.700317382812f,  0.299346923828f,  -0.007019042969f, 0.005920410156f,
    0.694854736328f,  0.306335449219f,  -0.007232666016f, 0.004699707031f,  0.689270019531f,
    0.313385009766f,  -0.007415771484f, 0.003570556641f,  0.683563232422f,  0.320465087891f,
    -0.007629394531f, 0.002471923828f,  0.677734375000f,  0.327606201172f,  -0.007873535156f,
    0.001434326172f,  0.671844482422f,  0.334777832031f,  -0.008087158203f, 0.000457763672f,
    0.665832519531f,  0.341979980469f,  -0.008270263672f, -0.000488281250f, 0.659729003906f,
    0.349243164062f,  -0.008453369141f, -0.001342773438f, 0.653533935547f,  0.356567382812f,
    -0.008636474609f, -0.002166748047f, 0.647216796875f,  0.363891601562f,  -0.008850097656f,
    -0.002960205078f, 0.640838623047f,  0.371276855469f,  -0.009033203125f, -0.003692626953f,
    0.634338378906f,  0.378692626953f,  -0.009216308594f, -0.004364013672f, 0.627777099609f,
    0.386138916016f,  -0.009338378906f, -0.004974365234f, 0.621154785156f,  0.393615722656f,
    -0.009490966797f, -0.005584716797f, 0.614440917969f,  0.401092529297f,  -0.009643554688f,
    -0.006134033203f, 0.607635498047f,  0.408599853516f,  -0.009796142578f, -0.006652832031f,
    0.600769042969f,  0.416107177734f,  -0.009918212891f, -0.007141113281f, 0.593841552734f,
    0.423645019531f,  -0.010009765625f, -0.007568359375f, 0.586853027344f,  0.431213378906f,
    -0.010131835938f, -0.007965087891f, 0.579772949219f,  0.438751220703f,  -0.010223388672f,
    -0.008331298828f, 0.572662353516f,  0.446319580078f,  -0.010284423828f, -0.008666992188f,
    0.565521240234f,  0.453887939453f,  -0.010345458984f, -0.008972167969f, 0.558319091797f,
    0.461456298828f,  -0.010406494141f, -0.009216308594f, 0.551055908203f,  0.469024658203f,
    -0.010406494141f, -0.009460449219f, 0.543731689453f,  0.476593017578f,  -0.010406494141f,
    -0.009674072266f, 0.536407470703f,  0.484130859375f,  -0.010375976562f, -0.009857177734f,
    0.529022216797f,  0.491668701172f,  -0.010375976562f, -0.010009765625f, 0.521606445312f,
    0.499176025391f,  -0.010314941406f, -0.010131835938f, 0.514160156250f,  0.506683349609f,
    -0.010253906250f, -0.010253906250f, 0.506683349609f,  0.514160156250f,  -0.010131835938f,
    -0.010314941406f, 0.499176025391f,  0.521606445312f,  -0.010009765625f, -0.010375976562f,
    0.491668701172f,  0.529022216797f,  -0.009857177734f, -0.010375976562f, 0.484130859375f,
    0.536407470703f,  -0.009674072266f, -0.010406494141f, 0.476593017578f,  0.543731689453f,
    -0.009460449219f, -0.010406494141f, 0.469024658203f,  0.551055908203f,  -0.009216308594f,
    -0.010406494141f, 0.461456298828f,  0.558319091797f,  -0.008972167969f, -0.010345458984f,
    0.453887939453f,  0.565521240234f,  -0.008666992188f, -0.010284423828f, 0.446319580078f,
    0.572662353516f,  -0.008331298828f, -0.010223388672f, 0.438751220703f,  0.579772949219f,
    -0.007965087891f, -0.010131835938f, 0.431213378906f,  0.586853027344f,  -0.007568359375f,
    -0.010009765625f, 0.423645019531f,  0.593841552734f,  -0.007141113281f, -0.009918212891f,
    0.416107177734f,  0.600769042969f,  -0.006652832031f, -0.009796142578f, 0.408599853516f,
    0.607635498047f,  -0.006134033203f, -0.009643554688f, 0.401092529297f,  0.614440917969f,
    -0.005584716797f, -0.009490966797f, 0.393615722656f,  0.621154785156f,  -0.004974365234f,
    -0.009338378906f, 0.386138916016f,  0.627777099609f,  -0.004364013672f, -0.009216308594f,
    0.378692626953f,  0.634338378906f,  -0.003692626953f, -0.009033203125f, 0.371276855469f,
    0.640838623047f,  -0.002960205078f, -0.008850097656f, 0.363891601562f,  0.647216796875f,
    -0.002166748047f, -0.008636474609f, 0.356567382812f,  0.653533935547f,  -0.001342773438f,
    -0.008453369141f, 0.349243164062f,  0.659729003906f,  -0.000488281250f, -0.008270263672f,
    0.341979980469f,  0.665832519531f,  0.000457763672f,  -0.008087158203f, 0.334777832031f,
    0.671844482422f,  0.001434326172f,  -0.007873535156f, 0.327606201172f,  0.677734375000f,
    0.002471923828f,  -0.007629394531f, 0.320465087891f,  0.683563232422f,  0.003570556641f,
    -0.007415771484f, 0.313385009766f,  0.689270019531f,  0.004699707031f,  -0.007232666016f,
    0.306335449219f,  0.694854736328f,  0.005920410156f,  -0.007019042969f, 0.299346923828f,
    0.700317382812f,  0.007202148438f,  -0.006774902344f, 0.292449951172f,  0.705657958984f,
    0.008514404297f,  -0.006561279297f, 0.285583496094f,  0.710906982422f,  0.009887695312f,
    -0.006378173828f, 0.278778076172f,  0.716003417969f,  0.011322021484f,  -0.006164550781f,
    0.272033691406f,  0.721008300781f,  0.012817382812f,  -0.005920410156f, 0.265350341797f,
    0.725860595703f,  0.014404296875f,  -0.005706787109f, 0.258728027344f,  0.730590820312f,
    0.016052246094f,  -0.005523681641f, 0.252197265625f,  0.735198974609f,  0.017791748047f,
    -0.005310058594f, 0.245727539062f,  0.739654541016f,  0.019561767578f,  -0.005065917969f,
    0.239318847656f,  0.743988037109f,  0.021392822266f,  -0.004852294922f, 0.233001708984f,
    0.748168945312f,  0.023315429688f,  -0.004669189453f, 0.226745605469f,  0.752197265625f,
    0.025299072266f,  -0.004455566406f, 0.220550537109f,  0.756103515625f,  0.027374267578f,
    -0.004272460938f, 0.214447021484f,  0.759857177734f,  0.029510498047f,  -0.004058837891f,
    0.208435058594f,  0.763488769531f,  0.031738281250f,  -0.003875732422f, 0.202484130859f,
    0.766937255859f,  0.034027099609f,  -0.003692626953f, 0.196594238281f,  0.770233154297f,
    0.036407470703f,  -0.003509521484f, 0.190826416016f,  0.773376464844f,  0.038879394531f,
    -0.003326416016f, 0.185119628906f,  0.776367187500f,  0.041412353516f,  -0.003143310547f,
    0.179504394531f,  0.779205322266f,  0.044006347656f,  -0.002990722656f, 0.173980712891f,
    0.781890869141f,  0.046722412109f,  -0.002807617188f, 0.168518066406f,  0.784423828125f,
    0.049499511719f,  -0.002655029297f, 0.163177490234f,  0.786743164062f,  0.052368164062f,
    -0.002471923828f, 0.157897949219f,  0.788940429688f,  0.055328369141f,  -0.002319335938f,
    0.152709960938f,  0.790985107422f,  0.058349609375f,  -0.002197265625f, 0.147613525391f,
    0.792846679688f,  0.061492919922f,  -0.002044677734f, 0.142608642578f,  0.794525146484f,
    0.064697265625f,  -0.001922607422f, 0.137695312500f,  0.796051025391f,  0.067962646484f,
    -0.001770019531f, 0.132873535156f,  0.797424316406f,  0.071350097656f,  -0.001647949219f,
    0.128143310547f,  0.798614501953f,  0.074798583984f,  -0.001525878906f, 0.123474121094f,
    0.799621582031f,  0.078369140625f,  -0.001403808594f, 0.118927001953f,  0.800476074219f,
    0.082031250000f,  -0.001281738281f, 0.114471435547f,  0.801177978516f,  0.085784912109f,
    -0.001159667969f, 0.110107421875f,  0.801696777344f,  0.089599609375f,  -0.001037597656f,
    0.105804443359f,  0.802032470703f,  0.093505859375f,  -0.000976562500f, 0.101593017578f,
    0.802215576172f,  0.097503662109f,
};

const static double i2fMagic = 4503601774854144.0;

// prototypes
static void do_src1(AXFX_CHORUS_SRCINFO* src);
static void do_src2(AXFX_CHORUS_SRCINFO* src);

asm static void do_src1(__REGISTER AXFX_CHORUS_SRCINFO* src) {
    nofralloc
	stwu r1, -64(r1)
	stmw r26, 40(r1)
	lwz r4,  AXFX_CHORUS_SRCINFO.posLo(src)
	lwz r5,  AXFX_CHORUS_SRCINFO.posHi(src)
	lwz r6,  AXFX_CHORUS_SRCINFO.pitchLo(src)
	lwz r8,  AXFX_CHORUS_SRCINFO.trigger(src)
	lwz r7,  AXFX_CHORUS_SRCINFO.target(src)
	lwz r31, AXFX_CHORUS_SRCINFO.smpBase(src)
	lwz r30, AXFX_CHORUS_SRCINFO.dest(src)
	lwz r9,  AXFX_CHORUS_SRCINFO.old(src)
	lis r10, 0x4330
	stw r10, 8(r1)
	stw r10, 16(r1)
	stw r10, 24(r1)
	stw r10, 32(r1)
	lis r10, i2fMagic@ha
	lfd f9, i2fMagic@l(r10)
	slwi r10, r5, 2
	lwz r11, 0(r9)
	lwz r29, 4(r9)
	lwz r28, 8(r9)
	lwzx r27, r31, r10
	xoris r11, r11, 0x8000
	xoris r29, r29, 0x8000
	stw r11, 12(r1)
	xoris r28, r28, 0x8000
	stw r29, 20(r1)
	xoris r27, r27, 0x8000
	stw r28, 28(r1)
	lfd f1, 8(r1)
	stw r27, 36(r1)
	lfd f2, 16(r1)
	fsubs f1, f1, f9
	lfd f3, 24(r1)
	fsubs f2, f2, f9
	lfd f4, 32(r1)
	fsubs f3, f3, f9
	fsubs f4, f4, f9
	li r26, -4
	lis r12, rsmpTab12khz@ha
	addi r12, r12, rsmpTab12khz@l
	li r9, 160
	mtctr r9
L_000000AC:
	rlwinm r10, r4, 7, 21, 27
	addc r4, r4, r6
	add r10, r10, r12
	mcrxr cr0
	lfs f5, 0(r10)
	beq L_000000F4
	lfs f6, 4(r10)
	fmuls f10, f1, f5
	lfs f7, 8(r10)
	fmadds f10, f2, f6, f10
	lfs f8, 12(r10)
	fmadds f10, f3, f7, f10
	addi r30, r30, 4
	fmadds f10, f4, f8, f10
	fctiwz f10, f10
	stfiwx f10, r26, r30
	bdnz L_000000AC
	b L_00000160
L_000000F4:
	addi r5, r5, 1
	lfs f6, 4(r10)
	fmuls f10, f1, f5
	cmpw r5, r8
	fmr f1, f2
	lfs f7, 8(r10)
	fmadds f10, f2, f6, f10
	fmr f2, f3
	lfs f8, 12(r10)
	fmadds f10, f3, f7, f10
	addi r30, r30, 4
	fmr f3, f4
	bne+ L_0000012C
	mr r5, r7
L_0000012C:
	fmadds f10, f4, f8, f10
	slwi r9, r5, 2
	bdz L_00000158
	lwzx r10, r9, r31
	fctiwz f10, f10
	xoris r10, r10, 0x8000
	stw r10, 12(r1)
	stfiwx f10, r26, r30
	lfd f4, 8(r1)
	fsubs f4, f4, f9
	b L_000000AC
L_00000158:
	fctiwz f10, f10
	stfiwx f10, r26, r30
L_00000160:
	lwz r9, AXFX_CHORUS_SRCINFO.old(src)
	fctiwz f1, f1
	fctiwz f2, f2
	fctiwz f3, f3
	stfiwx f1, r0, r9
	addi r10, r9, 4
	stfiwx f2, r0, r10
	addi r10, r9, 8
	stfiwx f3, r0, r10
	stw r4, AXFX_CHORUS_SRCINFO.posLo(src)
	stw r5, AXFX_CHORUS_SRCINFO.posHi(src)
	lmw r26, 40(r1)
	addi r1, r1, 64
    blr
}

asm static void do_src2(__REGISTER AXFX_CHORUS_SRCINFO* src) {
    nofralloc
	stwu r1, -64(r1)
	stmw r26, 40(r1)
	lwz r4,  AXFX_CHORUS_SRCINFO.posLo(src)
	lwz r5,  AXFX_CHORUS_SRCINFO.posHi(src)
	lwz r6,  AXFX_CHORUS_SRCINFO.pitchLo(src)
	lwz r8,  AXFX_CHORUS_SRCINFO.trigger(src)
	lwz r7,  AXFX_CHORUS_SRCINFO.target(src)
	lwz r31, AXFX_CHORUS_SRCINFO.smpBase(src)
	lwz r30, AXFX_CHORUS_SRCINFO.dest(src)
	lwz r9,  AXFX_CHORUS_SRCINFO.old(src)
	lis r10, 0x4330
	stw r10, 8(r1)
	stw r10, 16(r1)
	stw r10, 24(r1)
	stw r10, 32(r1)
	lis r10, i2fMagic@ha
	lfd f9, i2fMagic@l(r10)
	slwi r10, r5, 2
	lwz r11, 0(r9)
	lwz r29, 4(r9)
	lwz r28, 8(r9)
	lwzx r27, r31, r10
	xoris r11, r11, 0x8000
	xoris r29, r29, 0x8000
	stw r11, 12(r1)
	xoris r28, r28, 0x8000
	stw r29, 20(r1)
	xoris r27, r27, 0x8000
	stw r28, 28(r1)
	lfd f1, 8(r1)
	stw r27, 36(r1)
	lfd f2, 16(r1)
	fsubs f1, f1, f9
	lfd f3, 24(r1)
	fsubs f2, f2, f9
	lfd f4, 32(r1)
	fsubs f3, f3, f9
	fsubs f4, f4, f9
	li r26, -4
	lis r12, rsmpTab12khz@ha
	addi r12, r12, rsmpTab12khz@l
	li r9, 160
	mtctr r9
L_00000244:
	rlwinm r10, r4, 7, 21, 27
	addc r4, r4, r6
	add r10, r10, r12
	mcrxr cr0
	addi r5, r5, 1
	lfs f5, 0(r10)
	beq L_000002C0
	lfs f6, 4(r10)
	fmuls f10, f1, f5
	cmpw r5, r8
	fmr f1, f2
	lfs f7, 8(r10)
	fmadds f10, f2, f6, f10
	fmr f2, f3
	lfs f8, 12(r10)
	fmadds f10, f3, f7, f10
	addi r30, r30, 4
	fmr f3, f4
	bne+ L_00000294
	mr r5, r7
L_00000294:
	fmadds f10, f4, f8, f10
	slwi r9, r5, 2
	bdz L_00000344
	lwzx r10, r9, r31
	fctiwz f10, f10
	xoris r10, r10, 0x8000
	stw r10, 12(r1)
	stfiwx f10, r26, r30
	lfd f4, 8(r1)
	fsubs f4, f4, f9
	b L_00000244
L_000002C0:
	cmpw r5, r8
	lfs f6, 4(r10)
	bne+ L_000002D0
	mr r5, r7
L_000002D0:
	slwi r11, r5, 2
	addi r5, r5, 1
	lwzx r29, r11, r31
	fmuls f10, f1, f5
	cmpw r5, r8
	xoris r29, r29, 0x8000
	fmr f1, f3
	lfs f7, 8(r10)
	stw r29, 12(r1)
	fmadds f10, f2, f6, f10
	lfs f8, 12(r10)
	fmadds f10, f3, f7, f10
	lfd f3, 8(r1)
	fmr f2, f4
	addi r30, r30, 4
	fsubs f3, f3, f9
	bne+ L_00000318
	mr r5, r7
L_00000318:
	fmadds f10, f4, f8, f10
	slwi r9, r5, 2
	bdz L_00000344
	lwzx r10, r9, r31
	fctiwz f10, f10
	xoris r10, r10, 0x8000
	stw r10, 12(r1)
	stfiwx f10, r26, r30
	lfd f4, 8(r1)
	fsubs f4, f4, f9
	b L_00000244
L_00000344:
	fctiwz f10, f10
	stfiwx f10, r26, r30
	lwz r9, AXFX_CHORUS_SRCINFO.old(src)
	fctiwz f1, f1
	fctiwz f2, f2
	fctiwz f3, f3
	stfiwx f1, r0, r9
	addi r10, r9, 4
	stfiwx f2, r0, r10
	addi r10, r9, 8
	stfiwx f3, r0, r10
	stw r4, AXFX_CHORUS_SRCINFO.posLo(src)
	stw r5, AXFX_CHORUS_SRCINFO.posHi(src)
	lmw r26, 40(r1)
	addi r1, r1, 64
	blr
}

int AXFXChorusInit(AXFX_CHORUS* c) {
    s32* left;
    s32* right;
    s32* sur;
    u32 i;
    BOOL old;

	ASSERTMSGLINE(1074, c->baseDelay >= 5 && c->baseDelay <= 15 && c->variation >= 0 && c->variation <= 5 && c->period >= 500 && c->period <= 10000, "The value of specified parameter is out of range.");

	if (c->baseDelay < 5 || c->baseDelay > 15 || c->variation < 0 || c->variation > 5 || c->period < 500 || c->period > 10000) {
		return 0;
	}

    old = OSDisableInterrupts();
	c->work.lastLeft[0] = __AXFXAlloc(0x1680);
	ASSERTMSGLINE(0x442, c->work.lastLeft[0], "Can't allocate the memory.");

    if (c->work.lastLeft[0] != NULL) {
        c->work.lastRight[0] = (void*)(c->work.lastLeft[0] + 0x1E0);
        c->work.lastSur[0] = (void*)(c->work.lastRight[0] + 0x1E0);

        for (i = 1; i < 3; i++) {
            c->work.lastLeft[i] = (void*)&c->work.lastLeft[0][i * 0xA0];
            c->work.lastRight[i] = (void*)&c->work.lastRight[0][i * 0xA0];
            c->work.lastSur[i] = (void*)&c->work.lastSur[0][i * 0xA0];
        }

        left = c->work.lastLeft[0];
        right = c->work.lastRight[0];
        sur = c->work.lastSur[0];

        for (i = 0; i < 0x140; i++) {
            *left++ = 0;
            *right++ = 0;
            *sur++ = 0;
        }

        c->work.currentLast = 1;
        c->work.oldLeft[0] = c->work.oldLeft[1] = c->work.oldLeft[2] = c->work.oldLeft[3] = 0;
        c->work.oldRight[0] = c->work.oldRight[1] = c->work.oldRight[2] = c->work.oldRight[3] = 0;
        c->work.oldSur[0] = c->work.oldSur[1] = c->work.oldSur[2] = c->work.oldSur[3] = 0;
        c->work.src.trigger = 0x1E0;
        c->work.src.target = 0;
        OSRestoreInterrupts(old);
        return AXFXChorusSettings(c);
    }

    OSRestoreInterrupts(old);
    return 0;
}

int AXFXChorusShutdown(AXFX_CHORUS* c) {
    BOOL old;

    old = OSDisableInterrupts();
    __AXFXFree(c->work.lastLeft[0]);
    OSRestoreInterrupts(old);
    return 1;
}

int AXFXChorusSettings(AXFX_CHORUS* c) {
    BOOL old;

	ASSERTMSGLINE(1159, c->baseDelay >= 5 && c->baseDelay <= 15 && c->variation >= 0 && c->variation <= 5 && c->period >= 500 && c->period <= 10000, "The value of specified parameter is out of range.");
	if (c->baseDelay < 5 || c->baseDelay > 15 || c->variation < 0 || c->variation > 5 || c->period < 500 || c->period > 10000) {
		return 0;
	}

    old = OSDisableInterrupts();
    c->work.currentPosHi = 0x140 - ((c->baseDelay - 5) << 5);
    c->work.currentPosLo = 0;
    c->work.currentPosHi = (c->work.currentPosHi + ((c->work.currentLast - 1) * 0xA0/1)) % 480;
    c->work.pitchOffsetPeriod = ((c->period / 5) + 1) & ~(1);
    c->work.pitchOffsetPeriodCount = c->work.pitchOffsetPeriod >> 1;
    c->work.pitchOffset = (c->variation << 0x10) / (c->work.pitchOffsetPeriod * 5);
    OSRestoreInterrupts(old);
    return 1;
}

void AXFXChorusCallback(AXFX_BUFFERUPDATE* bufferUpdate, AXFX_CHORUS* chorus) {
    s32* leftD;
    s32* rightD;
    s32* surD;
    s32* leftS;
    s32* rightS;
    s32* surS;
    u32 i;
    u8 nextCurrentLast;

    nextCurrentLast = (chorus->work.currentLast + 1) % 3;
    leftD = chorus->work.lastLeft[nextCurrentLast];
    rightD = chorus->work.lastRight[nextCurrentLast];
    surD = chorus->work.lastSur[nextCurrentLast];
    leftS = bufferUpdate->left;
    rightS = bufferUpdate->right;
    surS = bufferUpdate->surround;

    for (i = 0; i < 0xA0; i++) {
        *leftD++ = *leftS++;
        *rightD++ = *rightS++;
        *surD++ = *surS++;
    }

    chorus->work.src.pitchHi = (chorus->work.pitchOffset >> 0x10) + 1;
    chorus->work.src.pitchLo = (chorus->work.pitchOffset & 0xFFFF) << 0x10;

    if (--chorus->work.pitchOffsetPeriodCount == 0) {
        chorus->work.pitchOffsetPeriodCount = chorus->work.pitchOffsetPeriod;
        chorus->work.pitchOffset = -chorus->work.pitchOffset;
    }

    for (i = 0; i < 3; i++) {
        chorus->work.src.posHi = chorus->work.currentPosHi;
        chorus->work.src.posLo = chorus->work.currentPosLo;
        switch (i) {                  
        case 0:
            chorus->work.src.smpBase = chorus->work.lastLeft[0];
            chorus->work.src.dest = bufferUpdate->left;
            chorus->work.src.old = &chorus->work.oldLeft[0];
            break;
        case 1:
            chorus->work.src.smpBase = chorus->work.lastRight[0];
            chorus->work.src.dest = bufferUpdate->right;
            chorus->work.src.old = &chorus->work.oldRight[0];
            break;
        case 2:
            chorus->work.src.smpBase = chorus->work.lastSur[0];
            chorus->work.src.dest = bufferUpdate->surround;
            chorus->work.src.old = &chorus->work.oldSur[0];
            break;
        }
        switch(chorus->work.src.pitchHi) {
            case 0:
                do_src1(&chorus->work.src);
                break;
            case 1:
                do_src2(&chorus->work.src);
                break;
        }
    }

    chorus->work.currentPosHi = (chorus->work.src.posHi % 480);
    chorus->work.currentPosLo = chorus->work.src.posLo;
    chorus->work.currentLast = nextCurrentLast;
}
