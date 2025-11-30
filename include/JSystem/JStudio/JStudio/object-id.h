#ifndef OBJECT_ID_H
#define OBJECT_ID_H

#include "dolphin/types.h"

namespace JStudio {
namespace object {


struct TIDData {
public:
    TIDData(const void* id, u32 id_size) : mID(id), mID_size(id_size) {}

    static bool isEqual(JStudio::object::TIDData const&, JStudio::object::TIDData const&);
    inline const u8* getID() const { return (const u8*)mID; }
	inline u32 getIDSize() const { return mID_size; }

protected:
    /* 0x00 */ const void* mID;
    /* 0x04 */ u32 mID_size;
};

struct TObject_ID : public TIDData {
    TObject_ID(const void* id, u32 id_size) : TIDData(id, id_size) {}
    ~TObject_ID() {}
    TIDData const& getIDData() const { return *this; }
    const u8 *getID() const { return TIDData::getID(); }
};

struct TPRObject_ID_equal : public TIDData {
    TPRObject_ID_equal(const void* id, u32 id_size) : TIDData(id, id_size) {}
    ~TPRObject_ID_equal() {}
    bool operator()(TObject_ID const& id) const { return TIDData::isEqual(id.getIDData(), *this); }
};

}  // namespace object
}  // namespace JStudio

#endif /* OBJECT_ID_H */
