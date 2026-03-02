#ifndef NW4HBM_SND_UTIL_H
#define NW4HBM_SND_UTIL_H

#include <revolution/types.h>

#include "../db/assert.h"
#include "snd_types.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {
            class Util {
            public:
                static const int SEMITONE_MAX = 12;
                static const int MICROTONE_MAX = 256;

                static const int VOLUME_MIN = static_cast<int>(10 * VOLUME_MIN_DB);  // -90.4db
                static const int VOLUME_MAX = static_cast<int>(10 * VOLUME_MAX_DB);  // +6.0db

            public:
                typedef enum RefType {
                    REFTYPE_ADDRESS,
                    REFTYPE_OFFSET,
                } RefType;

                typedef enum DataType {
                    DATATYPE_T0,
                    DATATYPE_T1,
                    DATATYPE_T2,
                    DATATYPE_T3,
                    DATATYPE_INVALID
                } DataType;

                template <typename T0, typename T1 = void, typename T2 = void, typename T3 = void>
                struct DataRef {
                    /* 0x00 */ u8 refType;
                    /* 0x01 */ u8 dataType;
                    /* 0x02 */ u16 reserved;
                    /* 0x04 */ u32 value;
                };

                template <typename T0, typename T1, typename T2, typename T3>
                static inline const T0* GetDataRefAddress0(const DataRef<T0, T1, T2, T3>& ref,
                                                           const void* base) {
                    NW4HBM_ASSERT(111, ref.dataType == 0);
                    return static_cast<const T0*>(
                        GetDataRefAddressImpl(static_cast<RefType>(ref.refType), ref.value, base));
                }

                template <typename T0, typename T1, typename T2, typename T3>
                static inline const T1* GetDataRefAddress1(const DataRef<T0, T1, T2, T3>& ref,
                                                           const void* base) {
                    NW4HBM_ASSERT(112, ref.dataType == 1);
                    return static_cast<const T1*>(
                        GetDataRefAddressImpl(static_cast<RefType>(ref.refType), ref.value, base));
                }

                template <typename T0, typename T1, typename T2, typename T3>
                static inline const T2* GetDataRefAddress2(const DataRef<T0, T1, T2, T3>& ref,
                                                           const void* base) {
                    NW4HBM_ASSERT(113, ref.dataType == 2);
                    return static_cast<const T2*>(
                        GetDataRefAddressImpl(static_cast<RefType>(ref.refType), ref.value, base));
                }

                template <typename T0, typename T1, typename T2, typename T3>
                static inline const T3* GetDataRefAddress3(const DataRef<T0, T1, T2, T3>& ref,
                                                           const void* base) {
                    NW4HBM_ASSERT(114, ref.dataType == 3);
                    return static_cast<const T3*>(
                        GetDataRefAddressImpl(static_cast<RefType>(ref.refType), ref.value, base));
                }

                template <typename T>
                struct Table {
                    /* 0x00 */ u32 count;
                    /* 0x04 */ T items[1];
                };

                static inline u16 ReadBigEndian(u16 x) { return x; }
                static inline u32 ReadBigEndian(u32 x) { return x; }

                static f32 CalcPitchRatio(int pitch);
                static f32 CalcVolumeRatio(f32 db);
                static f32 CalcPanRatio(f32 pan);
                static f32 CalcSurroundPanRatio(f32 surroundPan);
                static int CalcLpfFreq(f32 scale);

                static u16 CalcRandom();

            private:
                static const void* GetDataRefAddressImpl(RefType type, u32 value, const void* base);

            private:
                // Chromatic scale (tbl[idx] / tbl[idx-1] == ~1.06)
                static const f32 NoteTable[SEMITONE_MAX];
                // Each note contains 256 microtones
                static const f32 PitchTable[MICROTONE_MAX];

                // Table index is the millibel / hundredth-of-a-decibel
                // dB(idx) = VOLUME_MIN_DB + (idx / 10)
                // tbl[idx] = 10 ** (dB(idx) / 20)
                static const int VOLUME_TABLE_SIZE = VOLUME_RANGE_MB + 1;
                static const f32 Decibel2RatioTable[VOLUME_TABLE_SIZE];

                // 1/256 step
                static const int PAN_TABLE_MIN = 0;
                static const int PAN_TABLE_MAX = 256;
                static const int PAN_TABLE_CENTER = PAN_TABLE_MAX / 2;
                static const int PAN_TABLE_SIZE = PAN_TABLE_MAX + 1;
                // tbl[idx] = sqrt(1 - (idx / PAN_TABLE_MAX))
                static const f32 Pan2RatioTable[PAN_TABLE_SIZE];
            };
        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
