#pragma endian big

#include "std/sys.pat"
#include "type/magic.pat"
#include "std/array.pat"

/*
WSYS file
The WSYS file contains roughly two sections: WBCT and WINF.
WBCT contains the mapping of Wave ID -> wave archives.
WINF are the wave archives themselves and their metadata.
*/

/**
 * Wrapper for a file-global pointer that allows it to be stored in an array.
 */ 
struct Offset<T> {
    T* offset : u32 [[inline]];
};

/**
 * Format that audio data is in..
 */
enum WaveFormat : u8 {
    /**
     * 16-samples-per-9-bytes custom Nintendo ADPCM.
     */
    ADPCM4 = 0,
    
    /**
     * 16-samples-per-5-bytes custom Nintendo ADPCM.
     */
    ADPCM2 = 1,
    
    /**
     * 8-bit-per-sample PCM.
     */
    PCM8 = 2,
    
    /**
     * 16-bit-per-sample PCM.
     */
    PCM16 = 3,
};

/**
 * Defines playback info for a single audio "wave".
 * This data is stored per archive, making it duplicated (except for mAWOffsetStart)
 */
struct TWave {
    padding[1]; // unknown
    WaveFormat mWaveFormat;
    
    /**
     * Key (as in like, the musical term) this sample is in.
     */
    u8 mBaseKey;
    padding[1];
    
    /**
     * Sample rate of the audio in Hz.
     */
    float mSampleRate;
    
    /**
     * Position where the sample data starts.
     * This is in the .aw file for the archive containing this TWave.
     */
    u32 mAWOffsetStart;
    
    /**
     * Byte length of the sample data.
     */
    u32 mAWLength;
    
    /**
     * Indicates whether the sample should loop or not. All bits appear set if so.
     */
    u32 mLoopFlags;
    
    /**
     * Audio sample at which the loop starts.
     */
    u32 mLoopStartSample;
    
    /**
     * Audio sample at which the loop ends (and goes back to mLoopStartSample).
     */
    u32 mLoopEndSample;
    
    /**
     * Total sample count in this wave.
     */
    u32 mSampleCount;
    
    /**
     * Last sample for continuing ADPCM decode after loop.
     */
    s16 mpLast;
    
    /**
     * Penult sample for continuing ADPCM decode after loop.
     */
    s16 mpPenult;
};

/**
 * A single wave archive on disk.
 * These are paired 1:1 with TCtrlScene objects.
 */
struct TWaveArchive {
    /**
     * Filename of the raw sample data on disc. Relative to /Audiores/Waves/
     */
    char mFileName[0x70];
    
    /**
     * Amount of waves in this archive.
     * Matches the count in the paired TCtrl object.
     */
    u32 mWaveCount;
    
    /**
     * Offsets to the wave metadata (not sample data) in the WSYS.
     * These are paired 1:1 to the TCtrlWaves, and the TCtrlWave contains the actual
     * "Wave ID" used by the game for lookups.
     */
    Offset<TWave> waveOffsets[mWaveCount];
};

/**
 * Header containing data for wave archives and their metadata.
 */
struct TWaveArchiveBank {
    type::Magic<"WINF"> mMagic;

    /**
     * Amount of archives in this wave bank.
     * Matches the value in TCtrlGroup.
     */
    u32 mArchiveCounts;
    Offset<TWaveArchive> mArchiveOffsets[mArchiveCounts];
};

/**
 * Definition for a single wave in a control group.
 */
struct TCtrlWave {
    /**
     * Group ID matches the index of the control group this item is referenced by.
     */
    u16 mGroupId;
    
    /**
     * Wave ID used by the game to look this wave up.
     */
    u16 mWaveId;
};

/**
 * Contains the actual data for a TCtrlScene.
 * Why is this separate? Who knows.
 */
struct TCtrl {
    // Other versions of this struct with different magic (C-EX and C-ST) also exist in the file.
    // They aren't pointed to so we don't need to worry about them.
    type::Magic<"C-DF"> mMagic;
    
    /**
     * Amount of waves in this group.
     * Matches the value in TWaveArchive.
     */
    u32 waveCount;
    Offset<TCtrlWave> mWaveOffsets[waveCount];
};

/**
 * A single scene or "group" of waves that are loaded at once.
 */
struct TCtrlScene {
    type::Magic<"SCNE"> mMagic;
    
    padding[8]; // unknown
    TCtrl* mCtrlOffset : u32 [[inline]];
};

/**
 * Contains the "control" section of the WSYS.
 */
struct TCtrlGroup {
    type::Magic<"WBCT"> mMagic;
    
    padding[4]; // unknown
    u32 mGroupCount;
    
    Offset<TCtrlScene> mCtrlSceneOffsets[mGroupCount];
};

struct THeader {
    type::Magic<"WSYS"> mMagic;
    
    /**
     * Size of WSYS in bytes.
     */
    u32 mSize;
    
    /**
     * ID of wave bank.
     * This matches the value passed to the BAA load command.
     * The game originally does not use this value itself, but Dusklight does rely on it.
     */
    u32 mId;
    
    /**
     * Total amount of waves in this wave bank.
     * (not groups! Waves!)
     */
    u32 mWaveTableSize;
    
    TWaveArchiveBank* archiveBankOffset : u32;
    TCtrlGroup* ctrlGroupOffset : u32;
};

THeader header @ 0x0;

std::assert(
    header.archiveBankOffset.mArchiveCounts == header.ctrlGroupOffset.mGroupCount,
    "Control group and archive count does not match!");