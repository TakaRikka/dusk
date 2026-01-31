#ifndef JSTUDIOTOOLLIBRARY_CONTROLSET_PREVIEW_H
#define JSTUDIOTOOLLIBRARY_CONTROLSET_PREVIEW_H

#include "JSystem/JStudio/JStudioToolLibrary/controlset.h"

namespace JStudioToolLibrary {
    struct TControlSet_preview : public TControlSet {
        struct TAdaptor {
            virtual bool do_isReady();
            virtual void do_begin();
            virtual void do_end();
            virtual void do_current(u32, s32);
        };

        enum TEPreview {};

        TControlSet_preview(TConsole*, u32, uint, TAdaptor*);

        void preview_start();
        void preview_stop();
        void preview_rewind();
        void preview_step();
        void preview_pause();
        void preview_play();
        bool isPreview_stop() const;
        u32 getTime() const;
        u8 isTime_end() const;
        void update();
        void forwardTime_next_();
        int getTime_step() const;
        void adaptor_current_(u32, s32);
        void flushTime_();
        void setTime_(u32);
        void value_setTime_(u32);
        void preview(TEPreview);
        void adaptor_begin_();
        void resetTime_();
        bool adaptor_isReady_();
        u32 getControlStyle_time_end_() const;
        u32 getControlStyle_time_() const;
        void adaptor_end_();
        void control_updateTime_();
        void control_generate_preview(JORMContext*, TControlSet*, const TControl&);
        void control_reply_preview(const JORPropertyEvent*, TControlSet*, const TControl&);
        void control_update_preview(TControlSet*, const TControl&);
        void control_generate_preview_rewind(JORMContext*, TControlSet*, const TControl&);
        void control_reply_preview_rewind(const JORPropertyEvent*, TControlSet*, const TControl&);
        void control_update_preview_rewind(TControlSet*, const TControl&);
        void control_generate_preview_play(JORMContext*, TControlSet*, const TControl&);
        void control_reply_preview_play(const JORPropertyEvent*, TControlSet*, const TControl&);
        void control_update_preview_play(TControlSet*, const TControl&);
        void control_generate_preview_step(JORMContext*, TControlSet*, const TControl&);
        void control_reply_preview_step(const JORPropertyEvent*, TControlSet*, const TControl&);
        void control_update_preview_step(TControlSet*, const TControl&);
        void control_generate_time(JORMContext*, TControlSet*, const TControl&);
        void control_reply_time(const JORPropertyEvent*, TControlSet*, const TControl&);
        void control_update_time(TControlSet*, const TControl&);
        void control_generate_time_end(JORMContext*, TControlSet*, const TControl&);
        u32 getTime_end() const;
        void control_reply_time_end(const JORPropertyEvent*, TControlSet*, const TControl&);
        void control_update_time_end(TControlSet*, const TControl&);
        void control_generate_time_step(JORMContext*, TControlSet*, const TControl&);
        void control_reply_time_step(const JORPropertyEvent*, TControlSet*, const TControl&);
        void setTime_step(u32);
        void control_updateTime_step();
        void value_setTime_step(u32);
        void control_update_time_step(TControlSet*, const TControl&);

        /* 0x14 */ u8 field_0x14;
        /* 0x18 */ int field_0x18;
        /* 0x1C */ u32 uTime_;
        /* 0x20 */ u32 field_0x20;
        /* 0x24 */ int field_0x24;
        /* 0x28 */ u32 field_0x28;
        /* 0x2C */ int iTimeStep_;
        /* 0x30 */ u8 bTimeEnd_;
        /* 0x34 */ TAdaptor* pAdaptor_;
    };
}

#endif
