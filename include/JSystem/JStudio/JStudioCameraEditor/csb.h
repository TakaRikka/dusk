#ifndef JSTUDIOCAMERAEDITOR_CSB_H
#define JSTUDIOCAMERAEDITOR_CSB_H

#include <dolphin/mtx.h>
#include "JSystem/JGadget/linklist.h"
#include "JSystem/JGadget/std-vector.h"
#include "JSystem/JStudio/JStudio/functionvalue.h"

namespace JStudioCameraEditor {
    namespace csb {
        namespace data {
            enum TEValue {};
        }

        struct TValueSet {
            TValueSet();
            TValueSet(const TValueSet&);

            void set(data::TEValue, f32);
            void setVector_targetPosition(const Vec&);
            void setVector_position(const Vec&);
            void setApplied(data::TEValue, bool);
            void getVector_targetPosition_relativePosition(Vec*) const;
            void getVector_targetPosition(Vec*) const;
            void getVector_position(Vec*) const;
            void getVector_up(Vec*) const;
            f32 getValue(data::TEValue) const;
            void setTime(u32);
            u32 getTime() const;
            void setVector_position_relativeTargetPosition(const Vec&);
            void getVector_position_relativeTargetPosition(Vec*) const;
            void setVector_targetPosition_relativePosition(const Vec&);
            void addValue(data::TEValue, f32);
            void set(const TValueSet&, uint);
            bool isApplied(data::TEValue) const;

            /* 0x00 */ f32 fValue_[8];
            /* 0x20 */ bool bApplied_[8];
            /* 0x28 */ u32 uTime_;
        };

        struct TSequence : public JGadget::TLinkListNode {
            TSequence(u32);
            ~TSequence();

            JGadget::TVector<TValueSet, JGadget::TAllocator<TValueSet> >& referValueSetContainer();
            void getValueSetContainer() const;
            void setInterpolate(JStudio::TFunctionValue::TEInterpolate);
            JStudio::TFunctionValue::TEInterpolate getInterpolate() const;
            void setTime(u32);
            u32 getTime() const;
            void setTime_begin(u32);
            u32 getTime_begin() const;
            void assign(const TSequence&);
            u32 getTime_valueSet_all() const;
            u32 getTime_actual(u32*) const;
            u32 getTime_begin_actual_(u32) const;
            bool getFunctionValue(JStudio::TFunctionValue_list_parameter*, JGadget::TVector<f32, JGadget::TAllocator<f32> >*, data::TEValue, double) const;
            u32 getFunctionValueDataNumberNeeded() const;

            /* 0x08 */ JGadget::TVector<TValueSet, JGadget::TAllocator<TValueSet> > oContainer_;
            /* 0x1C */ u32 uTime_;
            /* 0x20 */ u32 uTimeBegin_;
            /* 0x24 */ JStudio::TFunctionValue::TEInterpolate eInterpolate_;
        };

        struct TSequenceContainer : public JGadget::TLinkList_factory<TSequence, 0> {
            TSequenceContainer();

            virtual ~TSequenceContainer();
            virtual TSequence* Do_create();
            virtual void Do_destroy(TSequence*);
        };
    }
}

#endif
