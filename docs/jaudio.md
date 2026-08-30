# JAudio

**JAudio** is the name for the audio engine used by Twilight Princess (along with many other Nintendo games from the era). TP uses exclusively JAudio v2, while other games use v1 or a mix of v1 and v2 infrastructure. This document will primarily focus on TP's use case, but best efforts will be made to document where behavior is TP-specific.

## Common concepts

Audio is almost exclusively 

### `JAISoundID`

A "sound", be that a _sound effect_ or music, is referenced in-code through the `JAISoundID` type. This is a `u32` that the game uses to look up the relevant sound. The actual value is bitpacked (BE) from the following fields:

| Type  | Field      | Description                                                                                                                                                                                               |
|-------|------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `u8`  | Section ID | Section of the Sound Table this sound effect is located in. 0 for sound effects, 1 for sequenced music, 2 for streamed music.[^sectionids]                                                                |
| `u8`  | Group ID   | ID of group further used to organize inside the Sound Table's Section. Sound effects are grouped into things like "SYSTEM" and "ENEMY", other sections leave this at 0.                                   |
| `u16` | "Wave ID"  | Index inside the group to look up the sound at.<br/>**Note** that the term "wave" in code is extremely confusing[^waveterm]; it does **not** refer to audio samples ("waves") in the wave banks directly. |

[^sectionids]: JAudio itself can seemingly work outside this convention, however it is enforced by some TP-specific game code.

[^waveterm]: Possibly vestigial from JAudio v1, where I believe there were less layers of indirection.

## Disc files

All audio data is stored in `/Audiores` on the disc. Files are as follows:

### `/Audiores/Seqs/Z2SoundSeqs.arc`

Contains the BMS instructions for all BMS-based music and sound effects. Not all data is kept in memory at once.

### `/Audiores/Stream/*.ast`

Contains individual streamed music. Each file is a separate music track. See [this page](https://www.lumasworkshop.com/wiki/AST_(File_Format)) for file format description.

### `/Audiores/Waves/*.aw`

Contains audio sample data for sequenced music and sound effects. These files are pure meat, no bone: they are loaded directly into ARAM and all metadata is stored in the BAA WSYS sections.

Each file contains audio samples for one "scene", with factors like the current level determining what "scenes" are made resident in memory. There is tons of duplicate data between scenes, presumably to increase simplicity and loading performance.

### `/Audiores/Z2Sound.baa`

Contains all remaining metadata for the audio system. This is effectively a container for a bunch of different sub-sections. The file starts with a bunch of "commands" that indicate where other data in the file is. Each command has a 4-character identifier and depending on the command will be followed by some extra arguments before the next command.

The commands used by TP's BAA file are as follows (note that the decompiled code has support for more load commands, which are unused):

| Command/Argument | Value           | Description                                                                                                                               |
|------------------|-----------------|-------------------------------------------------------------------------------------------------------------------------------------------|
| Command          | `AA_<`          | Start of BAA commands. Must be at the start of the file and only appear once.                                                             |
| Command          | `>_AA`          | End of BAA commands.                                                                                                                      |
| Command          | `ws  `[^spaces] | Wave bank/WSYS data. Defines where audio samples are located on disc.                                                                     |
| Argument         | u32             | Wave bank ID, max 255. In TP, 0 is sound effects, 1 is music samples.                                                                     |
| Argument         | u32             | File offset for start of `WSYS` data                                                                                                      |
| Argument         | u32             | Bit field selecting which groups (= `.aw` files) to load immediately. TP leaves this at zero.[^32ws]                                      |
| Command          | `bnk `[^spaces] | Instrument bank/IBNK                                                                                                                      |
| Argument         | u32             | Target wave bank ID                                                                                                                       |
| Argument         | u32             | File offset for start of `IBNK` data                                                                                                      |
| Command          | `bsc `[^spaces] | Sound effect sequence collection.                                                                                                         |
| Argument         | u32             | File offset for start of `SC` data.                                                                                                       |
| Argument         | u32             | File offset for end of `SC` data.                                                                                                         |
| Command          | `bst `[^spaces] | Sound table. Defines parameters for music and sound effects.                                                                              |
| Argument         | u32             | File offset for start of `BST `[^spaces] data.                                                                                            |
| Argument         | u32             | File offset for end of `BST `[^spaces] data.                                                                                              |
| Command          | `bstn`          | Sound name table. Defines names of all music and sound effects. Present on disc, but loading is disabled on release versions of the game. |
| Argument         | u32             | File offset for start of `BSTN` data.                                                                                                     |
| Argument         | u32             | File offset for end of `BSTN` data.                                                                                                       |
| Command          | `bfca`          | Unknown, something related to initialization of DSP FX data.                                                                              |
| Argument         | u32             | File offset for start of `RARC` data.                                                                                                     |

[^spaces]: Padded with spaces.

[^32ws]: Both of TP's wave banks have far more than 32 groups, and seemingly this mechanism would not be able to deal with that.

Following is data descriptions for the remaining data in the `.BAA`, as pointed to by the above commands.

### `WSYS` / Wave banks

`WSYS` / Wave bank data defines where a set of audio samples can be found on disc. Each wave bank is made of multiple "groups", where each group corresponds to one `.aw` file on disc. Each group has a set of "wave IDs" it contains, along with the metadata (e.g. sample rate) and data offset in the `.aw` file.

Multiple groups can contain the same wave ID, thus meaning the raw audio samples can be duplicated on disk.

*Relevant classes: `JASWSParser`, `JASBasicWaveBank`, `JASSimpleWaveBank`.* For the actual binary layout of this data, check [the ImHex pattern](imhex/jaudio/wsys.hexpat)

### `BST ` / Sound Table

The `BST` / Sound table defines various parameters for music and sound effect playback. 

The layout is pretty simple: it's hierarchical with sections (sound effects, music sequences, streamed music)[^sectionids], 
which have groups (only sound effects use this) and each group just has a flat list of items.
These indices match directly to the fields of the `JAISoundID`.

*Relevant classes: `JAUSoundTable`.* For the actual binary layout of this data, check [the ImHex pattern](imhex/jaudio/bst.hexpat)

Each item has a Type ID and a set of data. Types used by TP are as follows:

#### `0x51` / sound effect

Defines that this is a sound effect. The actual BMS code to execute (and unlike the other types, including its location) is looked up in the BSC.

Layout:

```
u8 mPriority; // Priority relative to other sound effects.
u8 mVolume; // Converted to float: mVolume * (1.0/127.0)
padding[2];
u32 mSwBit; // See below.
float mPitch; // Pitch multiplier
```

```cpp
// Values for mSwBit on sound effect items. (taken from JAUSoundTable.h)

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
 * Mute all BGM sequences while this sound is playing.
 */
#define SOUND_SW_MUTE_BGM             0x0000'0008

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
 * Unsure. Relates to Z2 pooling of sound handles.
 */
#define SOUND_SW_POOL_FLAG_1          0x0000'4000

/**
 * Unsure. Relates to Z2 pooling of sound handles.
 */
#define SOUND_SW_POOL_FLAG_2          0x0000'8000

/**
 * 3-bit mask used to select a volume distance/falloff class for this sound.
 */
#define SOUND_SW_VOL_DIST_BIT_MASK    0x0007'0000
#define SOUND_SW_VOL_DIST_BIT_OFFSET  16

/**
 * Limit minimum volume of this sound (after distance falloff) to 0.2.
 */
#define SOUND_SW_CLAMP_MIN_VOLUME     0x0008'0000

/**
 * 3-bit mask used to select a *different* volume distance/falloff class for this sound.
 * @see SOUND_SW_VOL_DIST_BIT_MASK must be zero for this to work.
 */
#define SOUND_SW_VOL_DIST_BIT_2_MASK  0x0070'0000
#define SOUND_SW_VOL_DIST_BIT_2_OFFSET 20

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
```

#### `0x60` / music sequence

Defines background music that plays through the BMS system.

```
u8 mPriority;
u8 mVolume; // Converted to float: mVolume * (1.0/127.0)
u16 mResourceId; // ID to look up in Z2SoundSeqs.arc
```

#### `0x70` / `0x71` / music sequence

Defines a streamed music track. Difference between `0x70` and `0x71` seems to only be that `0x71` stops automatically
on scene changes in TP's game code.

```
u8 mPriority;
u8 mVolume; // Converted to float: mVolume * (1.0/127.0)
u16 mStreamPanParameters; // Bitpacked, two bits per channel determining whether a channel is center (00), left (01), or right (10).
char* mStreamFilePath[] : u32; // File path to the .ast on disc.
```

## Further reading & credits

* https://www.lumasworkshop.com/wiki/SMR.szs (note that details like the exact layout of the BAA are not the same as TP)
* XAYRGA for doing much RE work and making [JAMTools](https://xayr.gay/tools/SoundModdingToolkit/)
* The decompiled source code, duh.
