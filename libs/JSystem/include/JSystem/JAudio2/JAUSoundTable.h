#ifndef JAUSOUNDTABLE_H
#define JAUSOUNDTABLE_H

#include "JSystem/JAudio2/JAISound.h"
#include "JSystem/JAudio2/JASGadget.h"
#include "helpers/endian.h"

// Constants for TypeIDs of JAUSoundTable entries.

/**
 * Used as a test to see if the type ID is valid. All other type IDs contain this bit.
 */
#define SOUND_TYPEID_VALID        0x40

/**
 * Sound is a sound effect.
 */
#define SOUND_TYPEID_SOUND_EFFECT 0x51

/**
 * Sound is a music sequence.
 */
#define SOUND_TYPEID_SEQUENCE     0x60

/**
 * Sound is a streamed music file.
 */
#define SOUND_TYPEID_STREAM       0x70

/**
 * Sound is a streamed music file. Unsure of difference from SOUND_TYPEID_STREAM
 */
#define SOUND_TYPEID_STREAM_ALT   0x71

//
// Values for mSwBit on sound effect items.
//

/**
 * Sound is always calculated as max priority (0).
 */
#define SOUND_SW_ALWAYS_MAX_PRIORITY  0x0000'0001

/**
 * Don't calculate volume by distance.
 */
#define SOUND_SW_IGNORE_DISTANCE_VOL  0x0000'0002

/**
 * Don't calculate FX mix (reverb) by distance.
 */
#define SOUND_SW_IGNORE_FX_MIX        0x0000'0004

/**
 * Offset to shift to access @see SOUND_SW_RANDOM_PITCH_MASK
 */
#define SOUND_SW_RANDOM_PITCH_OFFSET  4

/**
 * 4-bit value (0-15) to control the power of pitch randomization on sound playback.
 * Code acts different for values above 8, not sure what the exact implication is.
 */
#define SOUND_SW_RANDOM_PITCH_MASK    0x0000'00F0

/**
 * Offset to shift to access @see SOUND_SW_DOPPLER_POWER_MASK
 */
#define SOUND_SW_DOPPLER_POWER_OFFSET 8

/**
 * 4-bit value (0-15) to scale the power of the Doppler effect for this sound.
 */
#define SOUND_SW_DOPPLER_POWER_MASK   0x0000'0F00

/**
 * Don't calculate panning (left/right) values for this sound.
 */
#define SOUND_SW_IGNORE_PAN           0x0000'1000

/**
 * Don't calculate Dolby (behind/front) values for this sound.
 */
#define SOUND_SW_IGNORE_DOLBY         0x0000'2000

/**
 * 3-bit mask used to select a volume distance/falloff class for this sound.
 */
#define SOUND_SW_VOL_DIST_BIT_MASK    0x0007'0000

/**
 * Limit minimum volume of this sound (after distance falloff) to 0.2.
 */
#define SOUND_SW_CLAMP_MIN_VOLUME     0x0008'0000

/**
 * 3-bit mask used to select a *different* volume distance/falloff class for this sound.
 * @see SOUND_SW_VOL_DIST_BIT_MASK must be zero for this to work.
 */
#define SOUND_SW_VOL_DIST_BIT_2_MASK  0x0070'0000

/**
 * Mark sound as "far away" or "culled" when at max distance (selected by distance class).
 * This affects a bunch of stuff like culling, automatic stopping, priorities, etc.
 */
#define SOUND_SW_CULL_AT_MAX_DISTANCE 0x0080'0000

/**
 * Not sure what this does.
 * Something causing volume/pan/dolby adjustment in Z2Audible::setOuterParams?
 */
#define SOUND_SW_VOL_SOMETHING_MASK   0x0F00'0000

/**
 * Offset to shift to access @see SOUND_SW_RANDOM_VOLUME_MASK
 */
#define SOUND_SW_RANDOM_VOLUME_OFFSET 28

/**
 * 4-bit value (0-15) to control the power of volume randomization on sound playback.
 */
#define SOUND_SW_RANDOM_VOLUME_MASK   0xF000'0000

#define SOUND_VOL_DIST_BIT_MASK_SHIFTED   (SOUND_SW_VOL_DIST_BIT_MASK >> 16)
#define SOUND_VOL_DIST_BIT_2_MASK_SHIFTED (SOUND_SW_VOL_DIST_BIT_2_MASK >> 16)
#define SOUND_VOL_SOMETHING_MASK_SHIFTED  (SOUND_SW_VOL_SOMETHING_MASK >> 16)

/**
 * @ingroup jsystem-jaudio
 * A single entry in the sound table.
 * Fields are interpreted differently depending on the type ID of the entry,
 * which is not stored in this struct.
 */
struct JAUSoundTableItem {
    u8 mPriority;
    u8 mVolume;
    union {
        /**
         * For music sequences: the resource ID of the sequence.
         */
        BE(u16) mResourceId;

        /**
         * For streamed music: bitpacked channel configuration.
         */
        BE(u16) mStreamPanParameters;
    };
    union {
        /**
         * For sound effects: bitpacked field controlling a *bunch* of audio parameters.
         */
        BE(u32) mSwBit;

        /**
         * For streamed music: offset (relative to start of BST)
         * to null-terminated string containing music file path.
         */
        BE(u32) mStreamFilePath;
    };

    /**
     * For sound effects: pitch multiplier.
     */
    BE(f32) mPitch;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
template<typename Root, typename Section, typename Group, typename Typename_0>
struct JAUSoundTable_ {
    JAUSoundTable_() {
        mData = NULL;
        mRoot = 0;
    }

    void reset() {
        mData = NULL;
        mRoot = NULL;
    }

    void init(const void* dataStart) {
        mData = dataStart;
        // magic number is not in debug rom. I'm not sure what this comparison is (maybe some sort of '' number?)
        // I also do not know how it is different between JAUSoundTable and JAUSoundNameTable
        // Future person here: This is checking for either "BST " or "BSTN", with the second two letters in Root::magicNumber().
        // Idk why the operations here are all weird but I can't use objdiff right now.
        if (*(BE(u32)*)mData + 0xbdad0000 != Root::magicNumber()) {
            mData = NULL;
        } else {
            mRoot = (Root*)((u8*)mData + *((BE(u32)*)mData + 3));
        }
    }

    Section* getSection(int index) const {
        if (index < 0) {
            return NULL;
        }
        if ((u32)index >= mRoot->mSectionNumber) {
            return NULL;
        }
        u32 offset = mRoot->mSectionOffsets[index];
        if (offset == 0) {
            return NULL;
        } 
        return (Section*)((u8*)mData + offset);
    }

    Group* getGroup(Section* section, int index) const {
        int iVar1;

        if (index < 0) {
            return NULL;
        } 
        if ((u32)index >= section->mNumGroups) {
            return NULL;
        }
        u32 offset = section->getGroupOffset(index);
        if (offset == 0) {
            return NULL;
        } 
        return (Group*)((u8*)mData + offset);
    }

    const void* mData;
    Root* mRoot;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAUSoundTableRoot {
    static inline u32 magicNumber() { return 'T '; } // Second half of "BST "
    BE(u32) mSectionNumber;
    BE(u32) mSectionOffsets[0];
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAUSoundTableSection {
    int getGroupOffset(int index) const {
        if (index < 0) {
            return 0;
        }
        if (index >= mNumGroups) {
            return 0;
        }
        return mGroupOffsets[index];
    }

    BE(u32) mNumGroups;
    BE(u32) mGroupOffsets[0];
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAUSoundTableGroup {
    u8 getTypeID(int index) const {
        if (index < 0) {
            return 0;
        }
        if (index >= mNumItems) {
            return 0xff;
        }
        return mTypeIds[index * 4];
    }

    u32 getItemOffset(int index) const {
        if (index < 0) {
            return 0;
        }
        if (index >= mNumItems) {
            return 0;
        }
        return *(BE(u32)*)(mTypeIds + index * 4) & 0xffffff;
    }

    BE(u32) mNumItems;
    BE(u32) field_0x4;
    u8 mTypeIds[0]; // TODO: Should probably be BE(u32), but I can't objdiff rn.
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAUSoundTable : public JASGlobalInstance<JAUSoundTable> {
    JAUSoundTable(bool setInstance) : JASGlobalInstance<JAUSoundTable>(setInstance) {
    }
    ~JAUSoundTable() {}
    
    void init(void const*);
    u8 getTypeID(JAISoundID) const;
    JAUSoundTableItem* getData(JAISoundID) const;
    int getNumGroups_inSection(u8) const;
    int getNumItems_inGroup(u8, u8) const;

    JAUSoundTableItem* getItem(JAUSoundTableGroup* group, int index) const {
        u32 offset = group->getItemOffset(index);
        if (offset == 0) {
            return NULL;
        }
        return (JAUSoundTableItem*)((u8*)field_0x0.mData + offset);
    }

    const void* getResource() const { return field_0x0.mData; }
    bool isValid() const { return field_0x0.mData != NULL; }

    JAUSoundTable_<JAUSoundTableRoot,JAUSoundTableSection,JAUSoundTableGroup,void> field_0x0;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAUSoundNameTableRoot {
    static inline u32 magicNumber() { return 'TN'; } // Second half of "BSTN"
    BE(u32) mSectionNumber;
    BE(u32) mSectionOffsets[0];
};
/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAUSoundNameTableSection {};

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAUSoundNameTableGroup {};

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAUSoundNameTable : public JASGlobalInstance<JAUSoundNameTable> {
    JAUSoundNameTable(bool param_0) : JASGlobalInstance<JAUSoundNameTable>(param_0) {
    }
    ~JAUSoundNameTable() {}
    int getNumGroups_inSection(u8) const;
    int getNumItems_inGroup(u8, u8) const;
    void init(void const*);
    const char* getName(JAISoundID) const;
    const char* getGroupName(JAISoundID) const;

    JAUSoundTable_<JAUSoundNameTableRoot,JAUSoundNameTableSection,JAUSoundNameTableGroup,void> field_0x0;
};

#endif /* JAUSOUNDTABLE_H */
