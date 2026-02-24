#ifndef JKRARCHIVE_H
#define JKRARCHIVE_H

#include "JSystem/JKernel/JKRCompression.h"
#include "JSystem/JKernel/JKRFileLoader.h"
#include "global.h"

class JKRHeap;

/**
 * @ingroup jsystem-jkernel
 * 
 */
struct SArcHeader {
    /* 0x00 */ u32 signature;
    /* 0x04 */ u32 file_length;
    /* 0x08 */ u32 header_length;
    /* 0x0C */ u32 file_data_offset;
    /* 0x10 */ u32 file_data_length;
    /* 0x14 */ u32 field_0x14;
    /* 0x18 */ u32 field_0x18;
    /* 0x1C */ u32 field_0x1c;
};

/**
 * @ingroup jsystem-jkernel
 * 
 */
struct SArcDataInfo {
    /* 0x00 */ u32 num_nodes;
    /* 0x04 */ u32 node_offset;
    /* 0x08 */ u32 num_file_entries;
    /* 0x0C */ u32 file_entry_offset;
    /* 0x10 */ u32 string_table_length;
    /* 0x14 */ u32 string_table_offset;
    /* 0x18 */ u16 next_free_file_id;
    /* 0x1A */ bool sync_file_ids_and_indices;
    /* 0x1B */ u8 field_1b[5];
};

inline u32 read_big_endian_u32(void* ptr) {
    u8* uptr = (u8*)ptr;
    return ((u32)uptr[0] << 0x18) | ((u32)uptr[1] << 0x10) | ((u32)uptr[2] << 8) | (u32)uptr[3];
}

inline u16 read_big_endian_u16(void* ptr) {
    u8* uptr = (u8*)ptr;
    return ((u16)uptr[0] << 8) | ((u16)uptr[1]);
}

extern u32 sCurrentDirID__10JKRArchive;  // JKRArchive::sCurrentDirID

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRArchive : public JKRFileLoader {
public:
    struct SDirEntry {
        u8 flags;
        u8 padding;
        u16 id;
        const char* name;
    };

    struct SDIDirEntry {
        u32 type;
        u32 name_offset;
        u16 field_0x8;
        u16 num_entries;
        u32 first_file_index;
    };

    struct SDIFileEntry {
        u16 file_id;
        u16 name_hash;
        u32 type_flags_and_name_offset;
        u32 data_offset;
        u32 data_size;
        void* data;

        u32 getNameOffset() const { return type_flags_and_name_offset & 0xFFFFFF; }
        u16 getNameHash() const { return name_hash; }
        u32 getFlags() const { return type_flags_and_name_offset >> 24; }
        u32 getAttr() const { return getFlags(); }
        u16 getFileID() const { return file_id; }
        bool isDirectory() const { return (getFlags() & 0x02) != 0; }
        bool isUnknownFlag1() const { return (getFlags() & 0x01) != 0; }
        bool isCompressed() const { return (getFlags() & 0x04) != 0; }
        // was needed for open__14JKRAramArchiveFl
        u8 getCompressFlag() const { return (getFlags() & 0x04); }
        bool isYAZ0Compressed() const { return (getFlags() & 0x80) != 0; }
        u32 getSize() const { return data_size; }
    };

    enum EMountMode {
        UNKNOWN_MOUNT_MODE = 0,
        MOUNT_MEM = 1,
        MOUNT_ARAM = 2,
        MOUNT_DVD = 3,
        MOUNT_COMP = 4,
    };

    enum EMountDirection {
        UNKNOWN_MOUNT_DIRECTION = 0,
        MOUNT_DIRECTION_HEAD = 1,
        MOUNT_DIRECTION_TAIL = 2,
    };

    class CArcName {
    public:
        CArcName() {}
        CArcName(char const* data) { this->store(data); }
        CArcName(char const** data, char endChar) { *data = this->store(*data, endChar); }

        void store(char const* data);
        const char* store(char const* data, char endChar);

        u16 getHash() const { return mHash; }

        const char* getString() const { return mData; }

    private:
        u16 mHash;
        u16 mLength;
        char mData[256];
    };

protected:
    JKRArchive();
    JKRArchive(s32, EMountMode);

public:
    bool getDirEntry(SDirEntry*, u32) const;
    void* getIdxResource(u32);
    void* getResource(u16);
    u32 readIdxResource(void*, u32, u32);
    u32 readResource(void*, u32, u16);
    u32 countResource(void) const;
    u32 getFileAttribute(u32) const;

    SDIFileEntry* findNameResource(const char*) const;
    bool isSameName(CArcName&, u32, u16) const;
    SDIDirEntry* findResType(u32) const;
    SDIDirEntry* findDirectory(const char*, u32) const;
    SDIFileEntry* findTypeResource(u32, const char*) const;
    SDIFileEntry* findFsResource(const char*, u32) const;
    SDIFileEntry* findIdxResource(u32) const;
    SDIFileEntry* findPtrResource(const void*) const;
    SDIFileEntry* findIdResource(u16) const;

public:
    /* vt[04] */ virtual bool becomeCurrent(const char*);                /* override */
    /* vt[05] */ virtual void* getResource(const char*);                 /* override */
    /* vt[06] */ virtual void* getResource(u32, const char*);            /* override */
    /* vt[07] */ virtual u32 readResource(void*, u32, const char*);      /* override */
    /* vt[08] */ virtual u32 readResource(void*, u32, u32, const char*); /* override */
    /* vt[09] */ virtual void removeResourceAll(void);                   /* override */
    /* vt[10] */ virtual bool removeResource(void*);                     /* override */
    /* vt[11] */ virtual bool detachResource(void*);                     /* override */
    /* vt[12] */ virtual u32 getResSize(const void*) const;              /* override */
    /* vt[13] */ virtual u32 countFile(const char*) const;               /* override */
    /* vt[14] */ virtual JKRFileFinder* getFirstFile(const char*) const; /* override */
    /* vt[15] */ virtual u32 getExpandedResSize(const void* res) const { return getResSize(res); }
    /* vt[16] */ virtual void* fetchResource(SDIFileEntry*, u32*) = 0;
    /* vt[17] */ virtual void* fetchResource(void*, u32, SDIFileEntry*, u32*) = 0;
    /* vt[18] */ virtual void setExpandSize(SDIFileEntry*, u32);
    /* vt[19] */ virtual u32 getExpandSize(SDIFileEntry*) const;
    virtual ~JKRArchive();

    u32 countFile() const { return mArcInfoBlock->num_file_entries; }
    s32 countDirectory() const { return mArcInfoBlock->num_nodes; }
    u8 getMountMode() const { return mMountMode; }
    bool isFileEntry(u32 param_0) {
        return getFileAttribute(param_0) & 1;
    }

public:
    /* 0x00 */  // vtable
    /* 0x04 */  // JKRFileLoader
    /* 0x38 */ JKRHeap* mHeap;
    /* 0x3C */ u8 mMountMode;
    /* 0x3D */ u8 field_0x3d[3];
    /* 0x40 */ s32 mEntryNum;
    /* 0x44 */ SArcDataInfo* mArcInfoBlock;
    /* 0x48 */ SDIDirEntry* mNodes;
    /* 0x4C */ SDIFileEntry* mFiles;
    /* 0x50 */ s32* mExpandedSize;
    /* 0x54 */ const char* mStringTable;

protected:
    /* 0x58 */ u32 field_0x58;
    /* 0x5C */ JKRCompression mCompression;
    /* 0x60 */ EMountDirection mMountDirection;

public:
    static JKRArchive* check_mount_already(s32, JKRHeap*);
    static JKRArchive* mount(const char*, EMountMode, JKRHeap*, EMountDirection);
    static JKRArchive* mount(void*, JKRHeap*, EMountDirection);
    static JKRArchive* mount(s32, EMountMode, JKRHeap*, EMountDirection);
    static void* getGlbResource(u32, const char*, JKRArchive*);

    static JKRCompression convertAttrToCompressionType(int attr) {
#define JKRARCHIVE_ATTR_COMPRESSION 0x04
#define JKRARCHIVE_ATTR_YAZ0 0x80

        if (!(attr & JKRARCHIVE_ATTR_COMPRESSION)) {
            return COMPRESSION_NONE;
        } else if (attr & JKRARCHIVE_ATTR_YAZ0) {
            return COMPRESSION_YAZ0;
        } else {
           return COMPRESSION_YAY0;
        }
    }

    static u32 getCurrentDirID() { return sCurrentDirID; }
    static void setCurrentDirID(u32 dirID) { sCurrentDirID = dirID; }

protected:
    static u32 sCurrentDirID;
};

#ifdef TARGET_PC
#include "dusk/endian.h"

// Byte-swap archive header from Big-Endian to host after loading from disk
inline void JKRSwapArcHeader(SArcHeader* h) {
    h->signature         = be32(h->signature);
    h->file_length       = be32(h->file_length);
    h->header_length     = be32(h->header_length);
    h->file_data_offset  = be32(h->file_data_offset);
    h->file_data_length  = be32(h->file_data_length);
    h->field_0x14        = be32(h->field_0x14);
    h->field_0x18        = be32(h->field_0x18);
    h->field_0x1c        = be32(h->field_0x1c);
}

// Byte-swap archive data info block from Big-Endian to host
inline void JKRSwapArcDataInfo(SArcDataInfo* info) {
    info->num_nodes           = be32(info->num_nodes);
    info->node_offset         = be32(info->node_offset);
    info->num_file_entries    = be32(info->num_file_entries);
    info->file_entry_offset   = be32(info->file_entry_offset);
    info->string_table_length = be32(info->string_table_length);
    info->string_table_offset = be32(info->string_table_offset);
    info->next_free_file_id   = be16(info->next_free_file_id);
}

// Byte-swap all directory entries
inline void JKRSwapDirEntries(JKRArchive::SDIDirEntry* nodes, u32 count) {
    for (u32 i = 0; i < count; i++) {
        nodes[i].type             = be32(nodes[i].type);
        nodes[i].name_offset      = be32(nodes[i].name_offset);
        nodes[i].field_0x8        = be16(nodes[i].field_0x8);
        nodes[i].num_entries      = be16(nodes[i].num_entries);
        nodes[i].first_file_index = be32(nodes[i].first_file_index);
    }
}

// Byte-swap all file entries
inline void JKRSwapFileEntries(JKRArchive::SDIFileEntry* files, u32 count) {
    for (u32 i = 0; i < count; i++) {
        files[i].file_id                    = be16(files[i].file_id);
        files[i].name_hash                  = be16(files[i].name_hash);
        files[i].type_flags_and_name_offset = be32(files[i].type_flags_and_name_offset);
        files[i].data_offset                = be32(files[i].data_offset);
        files[i].data_size                  = be32(files[i].data_size);
        // data pointer is runtime-only, no swap needed
    }
}

// Swap all archive structures after loading from disk
inline void JKRSwapArchiveMemory(SArcDataInfo* arcInfo) {
    // First swap the info block itself to read offsets
    JKRSwapArcDataInfo(arcInfo);

    // Then swap directory and file entries using the now-native offsets
    JKRArchive::SDIDirEntry* nodes = (JKRArchive::SDIDirEntry*)((u8*)arcInfo + arcInfo->node_offset);
    JKRArchive::SDIFileEntry* files = (JKRArchive::SDIFileEntry*)((u8*)arcInfo + arcInfo->file_entry_offset);

    JKRSwapDirEntries(nodes, arcInfo->num_nodes);
    JKRSwapFileEntries(files, arcInfo->num_file_entries);
}
#else
inline void JKRSwapArcHeader(SArcHeader*) {}
inline void JKRSwapArcDataInfo(SArcDataInfo*) {}
inline void JKRSwapDirEntries(JKRArchive::SDIDirEntry*, u32) {}
inline void JKRSwapFileEntries(JKRArchive::SDIFileEntry*, u32) {}
inline void JKRSwapArchiveMemory(SArcDataInfo*) {}
#endif

inline JKRCompression JKRConvertAttrToCompressionType(int attr) {
    return JKRArchive::convertAttrToCompressionType(attr);
}

inline void* JKRGetResource(u32 node, const char* path, JKRArchive* archive) {
    return JKRArchive::getGlbResource(node, path, archive);
}

inline void* JKRGetTypeResource(u32 tag, const char* name, JKRArchive* arc) {
    return JKRArchive::getGlbResource(tag, name, arc);
}

inline bool JKRRemoveResource(void* resource, JKRFileLoader* fileLoader) {
    return JKRFileLoader::removeResource(resource, fileLoader);
}

inline JKRArchive* JKRMountArchive(void* ptr, JKRHeap* heap,
                                   JKRArchive::EMountDirection mountDirection) {
    return JKRArchive::mount(ptr, heap, mountDirection);
}

inline void JKRUnmountArchive(JKRArchive* arc) {
    arc->unmount();
}

inline u32 JKRReadIdxResource(void* buffer, u32 bufsize, u32 resIdx, JKRArchive* archive) {
    return archive->readIdxResource(buffer, bufsize, resIdx);
}

#endif
