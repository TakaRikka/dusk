#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio/functionvalue.h"
#include "JSystem/JGadget/search.h"
#include "JSystem/JUtility/JUTException.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JGadget/linklist.h"
#include "math.h"
#include "stdlib.h"

namespace JStudio {

namespace {

const ExtrapolateParameter gapfnExtrapolateParameter_[4] = {
    functionvalue::extrapolateParameter_raw,
    functionvalue::extrapolateParameter_repeat,
    functionvalue::extrapolateParameter_turn,
    functionvalue::extrapolateParameter_clamp,
};

}  // namespace

ExtrapolateParameter TFunctionValue::toFunction_outside(int idx) {
    ExtrapolateParameter fallback = NULL;
    ExtrapolateParameter result;

    result = JGadget::toValueFromIndex<ExtrapolateParameter>(idx, gapfnExtrapolateParameter_, 4,
                                                             fallback);

    if (result == NULL) {
        JUTWarn w;
        w << "unknown outside : " << idx;

        return gapfnExtrapolateParameter_[0];
    }

    return result;
}

TFunctionValue::TFunctionValue() {}
TFunctionValue::~TFunctionValue() {}

void TFunctionValueAttribute_refer::refer_initialize() {
    clear();
}

static f64 dummy_literal1() {
    return 0.0;
}

namespace functionvalue {

f64 interpolateValue_hermite(f64 c0, f64 c1, f64 x, f64 c2, f64 x2, f64 c3, f64 x3) {
    f64 a;
    f64 b;
    f64 c;
    f64 d;

    a = c0 - c1;
    b = a * (1.0 / (x2 - c1));               // (a - b) * 1.0 / (c - d)
    c = b - 1.0;                             // 1.0
    d = (3.0 + -2.0 * b) * (b * b);  // 3.0 - 2.0 * b
    f64 cab = (c * a * b);
    f64 coeffx3 = cab * x3;
    f64 cca = (c * c * a);
    f64 coeffc2 = cca * c2;
    return ((1.0 - d) * x + (d * c3)) + coeffc2 + coeffx3;
}


f64 interpolateValue_BSpline_uniform(f64 interpolationFactor, f64 point2, f64 point3, f64 point4, f64 point5) {
    f64 inverseInterpolationFactor = (1.0 - interpolationFactor);
    f64 inverseInterpolationFactorSquared = inverseInterpolationFactor * inverseInterpolationFactor;
    f64 inverseInterpolationFactorCubed = inverseInterpolationFactorSquared * inverseInterpolationFactor;

    f64 interpolationFactorSquared = interpolationFactor * interpolationFactor;
    f64 interpolationFactorCubed = interpolationFactorSquared * interpolationFactor;

    f64 coefficient1 = inverseInterpolationFactorCubed;

    f64 blendFactorForPoint3 = (1.0 / 2.0) * interpolationFactorCubed - interpolationFactorSquared + (2.0 / 3.0);

    f64 blendFactorForPoint4 =
        (1.0 / 2.0) * (interpolationFactor + interpolationFactorSquared - interpolationFactorCubed) + (1.0 / 6.0);

    f64 coefficient2 = interpolationFactorCubed;

    return ((coefficient1 * point2) + (coefficient2 * point5)) * (1.0 / 6.0) + (blendFactorForPoint3 * point3) +
           (blendFactorForPoint4 * point4);
}

f64 interpolateValue_BSpline_nonuniform(f64 interpolationFactor, const f64* controlPoints, const f64* knotVector) {
    f64 knot0              = knotVector[0];
    f64 knot1              = knotVector[1];
    f64 knot2              = knotVector[2];
    f64 knot3              = knotVector[3];
    f64 knot4              = knotVector[4];
    f64 knot5              = knotVector[5];
    f64 diff0              = interpolationFactor - knot0;
    f64 diff1              = interpolationFactor - knot1;
    f64 diff2              = interpolationFactor - knot2;
    f64 diff3              = knot3 - interpolationFactor;
    f64 diff4              = knot4 - interpolationFactor;
    f64 diff5              = knot5 - interpolationFactor;
    f64 inverseDeltaKnot32 = 1 / (knot3 - knot2);
    f64 blendFactor3       = (diff3 * inverseDeltaKnot32) / (knot3 - knot1);
    f64 blendFactor2       = (diff2 * inverseDeltaKnot32) / (knot4 - knot2);
    f64 blendFactor1       = (diff3 * blendFactor3) / (knot3 - knot0);
    f64 blendFactor4       = ((diff1 * blendFactor3) + (diff4 * blendFactor2)) / (knot4 - knot1);
    f64 blendFactor5       = (diff2 * blendFactor2) / (knot5 - knot2);
    f64 term1              = diff3 * blendFactor1;
    f64 term2              = (diff0 * blendFactor1) + (diff4 * blendFactor4);
    f64 term3              = (diff1 * blendFactor4) + (diff5 * blendFactor5);
    f64 term4              = diff2 * blendFactor5;

    return (term1 * controlPoints[0]) + (term2 * controlPoints[1]) + (term3 * controlPoints[2]) + (term4 * controlPoints[3]);
}

inline f64 interpolateValue_linear(f64 a1, f64 a2, f64 a3, f64 a4, f64 a5) {
    return a3 + ((a5 - a3) * (a1 - a2)) / (a4 - a2);
}

inline f64 interpolateValue_linear_1(f64 a1, f64 a2, f64 a3, f64 a4) {
    return a3 + (a4 - a3) * (a1 - a2);
}

inline f64 interpolateValue_plateau(f64 a1, f64 a2, f64 a3, f64 a4, f64 a5) {
    return interpolateValue_hermite(a1, a2, a3, 0.0, a4, a5, 0.0);
}

f64 extrapolateParameter_turn(f64 param_0, f64 param_1) {
    f64 dVar2 = 2.0 * param_1;
    f64 dVar1 = extrapolateParameter_repeat(param_0, dVar2);
    if (dVar1 >= param_1) {
        dVar1 = dVar2 - dVar1;
    }
    return dVar1;
}

}  // namespace functionvalue

void TFunctionValueAttribute_range::range_initialize() {
    fBegin_ = NAN;
    fEnd_ = fBegin_;
    fDifference_ = fBegin_;

    range_setProgress(TFunctionValue::PROG_INIT);
    range_setAdjust(TFunctionValue::ADJ_INIT);
    range_setOutside(TFunctionValue::OUT_INIT);
}

void TFunctionValueAttribute_range::range_prepare() {
    TFunctionValue::TEProgress progress = range_getProgress();

    switch (progress) {
    default:
        JUTWarn w;
        w << "unknown progress : " << progress;
    case 0:
        _20 = 0.0;
        _28 = 1.0;
        break;
    case 1:
        _20 = 0.0;
        _28 = -1.0;
        break;
    case 2:
        _20 = fBegin_;
        _28 = -1.0;
        break;
    case 3:
        _20 = fEnd_;
        _28 = -1.0;
        break;
    case 4:
        _20 = 0.5 * (fBegin_ + fEnd_);
        _28 = -1.0;
        break;
    }
}

void TFunctionValueAttribute_range::range_set(f64 begin, f64 end) {
    fBegin_ = begin;
    fEnd_ = end;
    fDifference_ = end - begin;

    JUT_ASSERT(458, fDifference_>=TValue(0));
}

// /* 80281A08-80281D18 27C348 0310+00 2/2 0/0 0/0 .text
//  * range_getParameter__Q27JStudio29TFunctionValueAttribute_rangeCFddd */
f64 TFunctionValueAttribute_range::range_getParameter(f64 arg1, f64 arg2, f64 arg3) const {
    f64 progress = range_getParameter_progress(arg1);
    TFunctionValue::TEAdjust adjust = range_getAdjust();

    f64 result;

    switch (adjust) {
    default:
        JUTWarn w;
        w << "unknown adjust : " << adjust;
    case 0:
        result = range_getParameter_outside(progress);
        break;
    case 1:
        result = range_getParameter_outside(progress + fBegin_);
        break;
    case 2:
        result = range_getParameter_outside(progress + fEnd_);
        break;
    case 3:
        result = range_getParameter_outside(progress + 0.5 * (fBegin_ + fEnd_));
        break;
    case 4:
        f64 temp = range_getParameter_outside(progress);
        result = arg2 + ((temp - fBegin_) * (arg3 - arg2)) / fDifference_;
        break;
    }
    return result;
}


TFunctionValueAttribute_range::TFunctionValueAttribute_range()
    : fBegin_(NAN), fEnd_(fBegin_), fDifference_(fBegin_),
      mProgress(TFunctionValue::PROG_INIT), mAdjust(TFunctionValue::ADJ_INIT), _20(fBegin_),
      _28(fBegin_), mBegin(TFunctionValue::OUT_INIT), mEnd(TFunctionValue::OUT_INIT) {}

TFunctionValue_composite::TFunctionValue_composite() : pfn_(NULL), data((void*)NULL) {
}

u32 TFunctionValue_composite::getType() const {
    return 1;
}

TFunctionValueAttributeSet TFunctionValue_composite::getAttributeSet() {
    return TFunctionValueAttributeSet(this, NULL, NULL);
}

void TFunctionValue_composite::initialize() {
    refer_initialize();
    pfn_ = NULL;
    data = TData((void*)NULL);
}

void TFunctionValue_composite::prepare() {
    /* empty function */
}

f64 TFunctionValue_composite::getValue(f64 arg1) {
    const TFunctionValueAttribute_refer* container = refer_getContainer();
    ASSERT(!refer_isReferring(this));
    ASSERT(pfn_ != NULL);

    return pfn_(arg1, container, data_getData());
}

/* 80281E5C-80281EC8 27C79C 006C+00 0/0 1/0 0/0 .text
 * composite_raw__Q27JStudio24TFunctionValue_compositeFRCQ27JGadget44TVector_pointer<PQ27JStudio14TFunctionValue>RCQ37JStudio24TFunctionValue_composite5TDatad
 */
f64 TFunctionValue_composite::composite_raw(TVector_pointer<TFunctionValue*> const& param_1,
                                                 TData const& param_2, f64 param_3) {
    u32 index = param_2.get_unsignedInteger();
    u32 size = param_1.size();
    if (index >= size) {
        return 0.0;
    }
    TFunctionValue** local_18 = (TFunctionValue**)param_1.begin();
    std::advance_pointer(local_18, index);
    TFunctionValue* piVar4 = *local_18;
    return piVar4->getValue(param_3);
}


/* 80281EC8-8028202C 27C808 0164+00 0/0 1/0 0/0 .text
 * composite_index__Q27JStudio24TFunctionValue_compositeFRCQ27JGadget44TVector_pointer<PQ27JStudio14TFunctionValue>RCQ37JStudio24TFunctionValue_composite5TDatad
 */
f64 TFunctionValue_composite::composite_index(TVector_pointer<TFunctionValue*> const& param_1,
                                              TData const& param_2, f64 param_3) {
    s32 size = param_1.size();
    if (size <= 1) {
        return 0.0;
    }
    TFunctionValue** local_148 = (TFunctionValue**)param_1.begin();
    TFunctionValue* pFront = *local_148;
    JUT_ASSERT(599, pFront!=NULL);
    f64 dVar4 = pFront->getValue(param_3);
    s32 index = floor(dVar4);
    u32 uVar2 = param_2.get_unsignedInteger();
    switch (uVar2) {
    case 0:
    case 3:
    default:
        if (index < 0) {
            index = 0;
        } else if (index >= size - 1) {
            index = size - 2;
        }
        break;
    case 1: {
        div_t dt = div(index, size - 1);
        index = dt.rem;
        if (index < 0) {
            index = size + index;
            index--;
        }
        break;
    }
    case 2:
        if (size - 1 == 1) {
            index = 0;
        } else {
            u32 uVar3 = (u32)(size - 2) * 2;
            div_t dt2 = div(index, uVar3);
            index = dt2.rem;
            if (index < 0) {
                index += uVar3;
            }
            if (index >= size - 1) {
                index = uVar3 - index;
            }
        }
        break;
    }
   
    std::advance_pointer(local_148, index + 1);
    pFront = *local_148;
    return pFront->getValue(param_3);
}


// TODO: remove when TContainerEnumerator_const is generic enough
template <typename T>
struct TContainerEnumerator_const_TVector : public JGadget::TEnumerator<const T*> {
    inline TContainerEnumerator_const_TVector(JGadget::TVector_pointer<T> const& param_1)
        : JGadget::TEnumerator<const T*>(param_1.begin(), param_1.end()) {}
};

/* 8028202C-80282094 27C96C 0068+00 0/0 1/0 0/0 .text
 * composite_parameter__Q27JStudio24TFunctionValue_compositeFRCQ27JGadget44TVector_pointer<PQ27JStudio14TFunctionValue>RCQ37JStudio24TFunctionValue_composite5TDatad
 */
f64
TFunctionValue_composite::composite_parameter(TVector_pointer<TFunctionValue*> const& param_1,
                                              TData const& param_2, f64 param_3) {
    f64 dVar4 = param_3 - param_2.get_value();
    TContainerEnumerator_const_TVector<TFunctionValue*> aTStack_18(param_1);
    while (aTStack_18) {
        TFunctionValue* const* ppiVar3 = *aTStack_18;
        TFunctionValue* piVar3 = *ppiVar3;
        dVar4 = piVar3->getValue(dVar4);
    }
    return dVar4;
}

/* 80282094-80282118 27C9D4 0084+00 0/0 1/0 0/0 .text
 * composite_add__Q27JStudio24TFunctionValue_compositeFRCQ27JGadget44TVector_pointer<PQ27JStudio14TFunctionValue>RCQ37JStudio24TFunctionValue_composite5TDatad
 */
f64 TFunctionValue_composite::composite_add(TVector_pointer<TFunctionValue*> const& param_1,
                                                 TData const& param_2, f64 param_3) {
    f64 dVar4 = param_2.get_value();
    TContainerEnumerator_const_TVector<TFunctionValue*> aTStack_18(param_1);
    while (aTStack_18) {
        TFunctionValue* const* ppiVar3 = *aTStack_18;
        TFunctionValue* piVar3 = *ppiVar3;
        dVar4 += piVar3->getValue(param_3);
    }
    return dVar4;
}

/* 80282118-80282200 27CA58 00E8+00 0/0 1/0 0/0 .text
 * composite_subtract__Q27JStudio24TFunctionValue_compositeFRCQ27JGadget44TVector_pointer<PQ27JStudio14TFunctionValue>RCQ37JStudio24TFunctionValue_composite5TDatad
 */
f64 TFunctionValue_composite::composite_subtract(TVector_pointer<TFunctionValue*> const& param_1,
                                                 TData const& param_2, f64 param_3) {
    u32 size = param_1.size();
    if (size == 0) {
        return 0.0;
    }
    TContainerEnumerator_const_TVector<TFunctionValue*> aTStack_18(param_1);
    TFunctionValue* const* local_148 = *aTStack_18;
    TFunctionValue* pFront = *local_148;
    JUT_ASSERT(688, pFront!=NULL);
    f64 dVar4 = pFront->getValue(param_3);
    while (aTStack_18) {
        TFunctionValue* const* ppiVar3 = *aTStack_18;
        TFunctionValue* piVar3 = *ppiVar3;
        dVar4 -= piVar3->getValue(param_3);
    }
    dVar4 -= param_2.f32data;
    return dVar4;
}


/* 80282200-80282284 27CB40 0084+00 0/0 1/0 0/0 .text
 * composite_multiply__Q27JStudio24TFunctionValue_compositeFRCQ27JGadget44TVector_pointer<PQ27JStudio14TFunctionValue>RCQ37JStudio24TFunctionValue_composite5TDatad
 */
f64 TFunctionValue_composite::composite_multiply(TVector_pointer<TFunctionValue*> const& param_1,
                                                 TData const& param_2, f64 param_3) {
    f64 dVar4 = param_2.get_value();
    TContainerEnumerator_const_TVector<TFunctionValue*> aTStack_18(param_1);
    while (aTStack_18) {
        TFunctionValue* const* ppiVar3 = *aTStack_18;
        TFunctionValue* piVar3 = *ppiVar3;
        dVar4 *= piVar3->getValue(param_3);
    }
    return dVar4;
}

/* 80282284-8028236C 27CBC4 00E8+00 0/0 1/0 0/0 .text
 * composite_divide__Q27JStudio24TFunctionValue_compositeFRCQ27JGadget44TVector_pointer<PQ27JStudio14TFunctionValue>RCQ37JStudio24TFunctionValue_composite5TDatad
 */
f64 TFunctionValue_composite::composite_divide(TVector_pointer<TFunctionValue*> const& param_1,
                                                 TData const& param_2, f64 param_3) {
    u32 size = param_1.size();
    if (size == 0) {
        return 0.0;
    }
    TContainerEnumerator_const_TVector<TFunctionValue*> aTStack_18(param_1);
    TFunctionValue* const* local_148 = *aTStack_18;
    TFunctionValue* pFront = *local_148;
    JUT_ASSERT(724, pFront!=NULL);
    f64 dVar4 = pFront->getValue(param_3);
    while (aTStack_18) {
        TFunctionValue* const* ppiVar3 = *aTStack_18;
        TFunctionValue* piVar3 = *ppiVar3;
        dVar4 /= piVar3->getValue(param_3);
    }
    dVar4 /= param_2.f32data;
    return dVar4;
}


TFunctionValue_constant::TFunctionValue_constant() : fValue_(NAN) {}

u32 TFunctionValue_constant::getType() const {
    return 2;
}

TFunctionValueAttributeSet TFunctionValue_constant::getAttributeSet() {
    return TFunctionValueAttributeSet(NULL, NULL, NULL);
}

void TFunctionValue_constant::initialize() {
    fValue_ = NAN;
}

void TFunctionValue_constant::prepare() {
    /* empty function */
}

f64 TFunctionValue_constant::getValue(f64 arg1) {
    return fValue_;
}

TFunctionValue_transition::TFunctionValue_transition() : _48(NAN), _50(_48) {}

u32 TFunctionValue_transition::getType() const {
    return 3;
}

TFunctionValueAttributeSet TFunctionValue_transition::getAttributeSet() {
    return TFunctionValueAttributeSet(NULL, this, this);
}

void TFunctionValue_transition::initialize() {
    range_initialize();
    interpolate_initialize();

    _48 = NAN;
    _50 = _48;
}

void TFunctionValue_transition::prepare() {
    range_prepare();
    interpolate_prepare();
}

f64 TFunctionValue_transition::getValue(f64 param_1) {
    f64 progress = range_getParameter_progress(param_1);
    f64 dVar3 = range_getParameter_outside(progress);
    switch (range_getAdjust()) {
    default:
        if (dVar3 < range_getBegin()) {
            return _48;
        }
        return _50;
    case TFunctionValue::ADJ_UNK2:
        if (dVar3 < range_getEnd()) {
            return _48;
        }
        return _50;
    case TFunctionValue::ADJ_UNK3:
        ADJ_UNK3_label:
        if (dVar3 < 0.5 * (range_getBegin() + range_getEnd())) {
            return _48;
        }
        return _50;
    case TFunctionValue::ADJ_UNK4:
        if (dVar3 < range_getBegin()) {
            return _48;
        }
        if (dVar3 >= range_getEnd()) {
            return _50;
        }
        switch (interpolate_get()) {
        case 0:
            goto ADJ_UNK3_label;
        case 1:
        case 3:
            return _48 + ((dVar3 - range_getBegin()) * data_getDifference()) / range_getDifference();
        case 2:
            return functionvalue::interpolateValue_plateau(dVar3, range_getBegin(), _48, range_getEnd(), _50);
        default:
            goto ADJ_UNK3_label;
        }
    }
}

TFunctionValue_list::TFunctionValue_list() : _44(NULL), uData_(0), _50(NAN), pfnUpdate_(NULL) {}

u32 TFunctionValue_list::getType() const {
    return 4;
}

TFunctionValueAttributeSet TFunctionValue_list::getAttributeSet() {
    return TFunctionValueAttributeSet(NULL, this, this);
}

void TFunctionValue_list::initialize() {
    range_initialize();
    interpolate_initialize();

    _44 = NULL;
    uData_ = 0;
    _50 = NAN;
    pfnUpdate_ = NULL;
}

void TFunctionValue_list::prepare() {
    range_prepare();
    interpolate_prepare();

    u32 interp = interpolate_get();

    switch (interp) {
    default:
        JUTWarn w;
        w << "unknown interpolation : " << interp;
    case 0:
        pfnUpdate_ = update_INTERPOLATE_NONE_;
        break;
    case 1:
        pfnUpdate_ = update_INTERPOLATE_LINEAR_;
        break;
    case 2:
        pfnUpdate_ = update_INTERPOLATE_PLATEAU_;
        break;
    case 3:
        pfnUpdate_ = update_INTERPOLATE_BSPLINE_dataMore3_;
        if (uData_ == 2)
            pfnUpdate_ = update_INTERPOLATE_LINEAR_;
        break;
    }
}

f64 TFunctionValue_list::getValue(f64 param_1) {
    f64 dVar9 = range_getParameter_progress(param_1);
    u32 iVar7 = uData_ - 1;
    TFunctionValue::TEAdjust iVar5 = range_getAdjust();
    f64 dVar12 = iVar7;
    TIndexData_ local_178;
    f64 parOutside;
    switch (iVar5) {
    case 0:
    default:
        parOutside = range_getParameter_outside(dVar9);
        local_178._0 = parOutside / _50;
        break;
    case 1:
        parOutside = range_getParameter_outside(dVar9 + range_getBegin());
        local_178._0 = parOutside / _50;
        break;
    case 2:
        parOutside = range_getParameter_outside(dVar9 + range_getEnd());
        local_178._0 = parOutside / _50;
        break;
    case 3:
        parOutside = range_getParameter_outside(dVar9 + 0.5 * (range_getBegin() + range_getEnd()));
        local_178._0 = parOutside / _50;
        break;
    case 4:
        parOutside = range_getParameter_outside(dVar9);
        local_178._0 = (dVar12 * (parOutside - range_getBegin())) / range_getDifference();
        break;
    }
   
    if (local_178._0 < 0.0) {
        return _44[0];
    }
    if (local_178._0 >= dVar12) {
        return _44[iVar7];
    }
    local_178._8 = floor(local_178._0);
    local_178._10 = local_178._8;
    JUT_ASSERT(1063, pfnUpdate_!=NULL);
    return pfnUpdate_(*this, local_178);
}


f64 TFunctionValue_list::update_INTERPOLATE_NONE_(const TFunctionValue_list& rThis,
                                                  const TIndexData_& data) {
    return rThis._44[data._10];
}

f64 TFunctionValue_list::update_INTERPOLATE_LINEAR_(const TFunctionValue_list& rThis,
                                                    const TIndexData_& data) {
    return functionvalue::interpolateValue_linear_1(data._0, data._8, rThis._44[data._10],
                                                    rThis._44[data._10 + 1]);
}

/* 80282C58-80282CA8 27D598 0050+00 1/1 0/0 0/0 .text
 * update_INTERPOLATE_PLATEAU___Q27JStudio19TFunctionValue_listFRCQ27JStudio19TFunctionValue_listRCQ37JStudio19TFunctionValue_list11TIndexData_
 */
f64 TFunctionValue_list::update_INTERPOLATE_PLATEAU_(const TFunctionValue_list& rThis,
                                                     const TIndexData_& data) {
    const f32* arr = rThis._44;

    return functionvalue::interpolateValue_plateau(data._0, data._8, arr[data._10],
                                                   1.0 + data._8, arr[data._10 + 1]);
}


/* 80282CA8-80282D34 27D5E8 008C+00 1/1 0/0 0/0 .text
 * update_INTERPOLATE_BSPLINE_dataMore3___Q27JStudio19TFunctionValue_listFRCQ27JStudio19TFunctionValue_listRCQ37JStudio19TFunctionValue_list11TIndexData_
 */
f64 TFunctionValue_list::update_INTERPOLATE_BSPLINE_dataMore3_(
    TFunctionValue_list const& rThis, TFunctionValue_list::TIndexData_ const& param_2) {
    f64 dVar11 = rThis._44[param_2._10];
    f64 dVar10 = rThis._44[param_2._10 + 1];
    f64 dVar9;
    f64 dVar8;
    if (param_2._10 == 0) {
        JUT_ASSERT(1119, rThis.uData_>=3);
        dVar9 = 2.0 * dVar11 - dVar10;
        dVar8 = rThis._44[param_2._10 + 2];
    } else {
        if (param_2._10 == rThis.uData_ - 2) {
            JUT_ASSERT(1125, rThis.uData_>=3);
            dVar9 = rThis._44[param_2._10 - 1];
            dVar8 = 2.0 * dVar10 - dVar11;
        } else {
            JUT_ASSERT(1131, rThis.uData_>=3);
            dVar9 = rThis._44[param_2._10 - 1];
            dVar8 = rThis._44[param_2._10 + 2];
        }
    }
    functionvalue::interpolateValue_BSpline_uniform(param_2._0 - param_2._8, dVar9, dVar11, dVar10, dVar8);
}


TFunctionValue_list_parameter::TFunctionValue_list_parameter()
    : pfData_(NULL), uData_(0), dat1(*this, NULL), dat2(dat1), dat3(dat1), pfnUpdate_(NULL) {}

u32 TFunctionValue_list_parameter::getType() const {
    return 5;
}

TFunctionValueAttributeSet TFunctionValue_list_parameter::getAttributeSet() {
    return TFunctionValueAttributeSet(NULL, this, this);
}

void TFunctionValue_list_parameter::data_set(const f32* pf, u32 u) {
    JUT_ASSERT(1277, (pf != NULL) || (u == 0));

    pfData_ = pf;
    uData_ = u;

    dat1.set(pfData_);
    dat2.set(&pfData_[uData_ * 2]);
    dat3 = dat1;
#if DEBUG
    pfnUpdate_ = NULL;
#endif
}

void TFunctionValue_list_parameter::initialize() {
    range_initialize();
    interpolate_initialize();

    pfData_ = NULL;
    uData_ = 0;

    TIterator_data_ iter(*this, NULL);

    dat1 = iter;
    dat2 = dat1;
    dat3 = dat1;
    pfnUpdate_ = NULL;
}

void TFunctionValue_list_parameter::prepare() {
    range_prepare();
    interpolate_prepare();

    u32 interp = interpolate_get();
    switch (interp) {
    default:
        JUTWarn w;
        w << "unknown interpolation : " << interp;
    case 0:
        pfnUpdate_ = update_INTERPOLATE_NONE_;
        break;
    case 1:
        pfnUpdate_ = update_INTERPOLATE_LINEAR_;
        break;
    case 2:
        pfnUpdate_ = update_INTERPOLATE_PLATEAU_;
        break;
    case 3:
        pfnUpdate_ = update_INTERPOLATE_BSPLINE_dataMore3_;
        if (uData_ != 2)
            return;
        pfnUpdate_ = update_INTERPOLATE_LINEAR_;
        break;
    }
}

f64 TFunctionValue_list_parameter::getValue(f64 param_0) {
    param_0 = range_getParameter(param_0, data_getValue_front(), data_getValue_back());
    JUT_ASSERT(1395, pfData_!=NULL)

    dat3 = JGadget::findUpperBound_binary_current(dat1, dat2, dat3, param_0);
    if (dat3 == dat1) {
        return dat3.get()[1];
    }
    if (dat3 == dat2) {
        --dat3;
        return dat3.get()[1];
    } 

    const f32* pf = dat3.get();
    JUT_ASSERT(1411, (pfData_<=pf-suData_size)&&(pf<pfData_+suData_size*uData_));
    JUT_ASSERT(1412, pfnUpdate_!=NULL);
    return pfnUpdate_(*this, param_0);
}

f64 TFunctionValue_list_parameter::update_INTERPOLATE_NONE_(
    const TFunctionValue_list_parameter& rThis, f64 d) {
    return rThis.dat3.get()[-1];
}

f64 TFunctionValue_list_parameter::update_INTERPOLATE_LINEAR_(
    const TFunctionValue_list_parameter& rThis, f64 d) {
    const f32* a = rThis.dat3.get();
    return functionvalue::interpolateValue_linear(d, a[-2], a[-1], a[0], a[1]);
}

/* 80283024-80283060 27D964 003C+00 1/1 0/0 0/0 .text
 * update_INTERPOLATE_PLATEAU___Q27JStudio29TFunctionValue_list_parameterFRCQ27JStudio29TFunctionValue_list_parameterd
 */
f64 TFunctionValue_list_parameter::update_INTERPOLATE_PLATEAU_(
    const TFunctionValue_list_parameter& rThis, f64 d) {
    const f32* a = rThis.dat3.get();
    return functionvalue::interpolateValue_plateau(d, a[-2], a[-1], a[0], a[1]);
}


/* 80283060-802832C4 27D9A0 0264+00 1/1 0/0 0/0 .text
 * update_INTERPOLATE_BSPLINE_dataMore3___Q27JStudio29TFunctionValue_list_parameterFRCQ27JStudio29TFunctionValue_list_parameterd
 */
f64 TFunctionValue_list_parameter::update_INTERPOLATE_BSPLINE_dataMore3_(
    TFunctionValue_list_parameter const& rThis, f64 param_2) {
    JUT_ASSERT(1457, rThis.uData_>=3)
    const f32* pfVar2 = rThis.dat3.get();
    f64 local_68[4];
    f64 local_48[6];
    local_68[1] = pfVar2[-1];
    local_68[2] = pfVar2[1];
    local_48[2] = pfVar2[-2];
    local_48[3] = pfVar2[0];
    s32 iVar5 = ((int)pfVar2 - (int)rThis.dat1.get()) / 4;
    s32 iVar3 = ((int)rThis.dat2.get() - (int)pfVar2) / 4;
    switch(iVar5) {
    case 2:
        local_68[0] = 2.0 * local_68[1] - local_68[2];
        local_68[3] = pfVar2[3];
        local_48[4] = pfVar2[2];
        local_48[1] = 2.0 * local_48[2] - local_48[3];
        local_48[0] = 2.0 * local_48[2] - local_48[4];
        switch (iVar3) {
        case 2:
            JUT_ASSERT(1481, false);
        case 4:
            local_48[5] = 2.0 * local_48[4] - local_48[3];
            break;
        default:
            local_48[5] = pfVar2[4];
            break;
        }
        break;
    case 4:
        local_68[0] = pfVar2[-3];
        local_48[1] = pfVar2[-4];
        local_48[0] = 2.0 * local_48[1] - local_48[2];
        switch (iVar3)
        {
        case 2:
            local_68[3] = 2.0 * local_68[2] - local_68[1];
            local_48[4] = 2.0 * local_48[3] - local_48[2];
            local_48[5] = 2.0 * local_48[3] - local_48[1];
            break;
        case 4:
            local_68[3] = pfVar2[3];
            local_48[4] = pfVar2[2];
            local_48[5] = 2.0 * local_48[4] - local_48[3];
            break;
        default:
            local_68[3] = pfVar2[3];
            local_48[4] = pfVar2[2];
            local_48[5] = pfVar2[4];
        }
        break;
    default:
        local_68[0] = pfVar2[-3];
        local_48[1] = pfVar2[-4];
        local_48[0] = pfVar2[-6];
        switch (iVar3) {
        case 2:
            local_68[3] = 2.0 * local_68[2] - local_68[1];
            local_48[4] = 2.0 * local_48[3] - local_48[2];
            local_48[5] = 2.0 * local_48[3] - local_48[1];
            break;
        case 4:
            local_68[3] = pfVar2[3];
            local_48[4] = pfVar2[2];
            local_48[5] = 2.0 * local_48[4] - local_48[3];
            break;
        default:
            local_68[3] = pfVar2[3];
            local_48[4] = pfVar2[2];
            local_48[5] = pfVar2[4];
            break;
        }
        break;
    }
    return functionvalue::interpolateValue_BSpline_nonuniform(param_2, local_68, local_48);
}


TFunctionValue_hermite::TFunctionValue_hermite()
    : pfData_(NULL), u_(0), uSize_(0), dat1(*this, NULL), dat2(dat1), dat3(dat1) {}

u32 JStudio::TFunctionValue_hermite::getType() const {
    return 6;
}

TFunctionValueAttributeSet TFunctionValue_hermite::getAttributeSet() {
    return TFunctionValueAttributeSet(NULL, this, NULL);
}

void TFunctionValue_hermite::data_set(const f32* pf, u32 u, u32 uSize) {
    JUT_ASSERT(1676, (pf != NULL) || (u == 0));
    JUT_ASSERT(1677, (uSize == 3) || (uSize == 4));

    pfData_ = pf;
    u_ = u;
    uSize_ = uSize;

    dat1.set(pfData_, uSize_);
    dat2.set(&pfData_[u_ * uSize_], uSize_);
    dat3 = dat1;
}

void TFunctionValue_hermite::initialize() {
    range_initialize();

    pfData_ = NULL;
    u_ = 0;
    uSize_ = 0;

    dat1 = TIterator_data_(*this, NULL);
    dat2 = dat1;
    dat3 = dat1;
}

void TFunctionValue_hermite::prepare() {
    range_prepare();
}

f64 TFunctionValue_hermite::getValue(f64 param_0) {
    param_0 = range_getParameter(param_0, data_getValue_front(), data_getValue_back());
    JUT_ASSERT(1716, pfData_!=NULL)
    
    dat3 = JGadget::findUpperBound_binary_current(dat1, dat2, dat3, param_0);
    
    if (dat3 == dat1) {
        return dat3.get()[1];
    }
    if (dat3 == dat2) {
        --dat3;
        return dat3.get()[1];
    }

    const f32* pfVar5 = dat3.get();
    const f32* pfVar7 = pfVar5 - uSize_;
    return functionvalue::interpolateValue_hermite(
        param_0, pfVar7[0], pfVar7[1],
        pfVar7[uSize_ - 1], pfVar5[0],
        pfVar5[1], pfVar5[2]);
}

}  // namespace JStudio
