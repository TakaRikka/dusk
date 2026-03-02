#ifndef NW4HBM_LYT_PANE_H
#define NW4HBM_LYT_PANE_H

#include <revolution/types.h>

#include "../ut/Color.h"
#include "../ut/LinkList.h"
#include "../ut/Rect.h"
#include "../ut/RuntimeTypeInfo.h"

#include "../db/assert.h"

#include "../math/types.h"

#include "animation.h"
#include "drawInfo.h"
#include "lyt_types.h"
#include "material.h"


namespace nw4hbm {
    namespace lyt {
        class Pane;

        namespace detail {
            class PaneBase {
            public:
                inline PaneBase() : mLink() {}

                /* 0x08 */ virtual ~PaneBase() {}

                /* 0x00 (vtable) */
                /* 0x04 */ ut::LinkListNode mLink;
            };
        }  // namespace detail
        typedef ut::LinkList<Pane, offsetof(detail::PaneBase, mLink)> PaneList;

        enum {
            /* 1 */ ANIMOPTION_SKIP_INVISIBLE = (1 << 0),
        };

        class Pane : detail::PaneBase {
        private:
            enum {
                /* 0 */ BIT_VISIBLE = 0,
                /* 1 */ BIT_INFLUENCED_ALPHA,
                /* 2 */ BIT_LOCATION_ADJUST
            };

        public:
            Pane();
            Pane(const res::Pane* pBlock);

            /* 0x08 */ virtual ~Pane();
            /* 0x0C */ NW4HBM_UT_RUNTIME_TYPEINFO;
            /* 0x10 */ virtual void CalculateMtx(const DrawInfo& drawInfo);
            /* 0x14 */ virtual void Draw(const DrawInfo& drawInfo);
            /* 0x18 */ virtual void DrawSelf(const DrawInfo& drawInfo);
            /* 0x1C */ virtual void Animate(u32 option = 0);
            /* 0x20 */ virtual void AnimateSelf(u32 option = 0);
            /* 0x24 */ virtual ut::Color GetVtxColor(u32 idx) const;
            /* 0x28 */ virtual void SetVtxColor(u32 idx, ut::Color valuw);
            /* 0x2C */ virtual u8 GetColorElement(u32 idx) const;
            /* 0x30 */ virtual void SetColorElement(u32 idx, u8 color);
            /* 0x34 */ virtual u8 GetVtxColorElement(u32 idx) const;
            /* 0x38 */ virtual void SetVtxColorElement(u32 idx, u8 element);
            /* 0x3C */ virtual Pane* FindPaneByName(const char* findName, bool bRecursive = true);
            /* 0x40 */ virtual Material* FindMaterialByName(const char* findName,
                                                            bool bRecursive = true);
            /* 0x44 */ virtual void BindAnimation(AnimTransform* animTrans, bool bRecursive = true);
            /* 0x48 */ virtual void UnbindAnimation(AnimTransform* animTrans,
                                                    bool bRecursive = true);
            /* 0x4C */ virtual void UnbindAllAnimation(bool bRecursive = true);
            /* 0x50 */ virtual void UnbindAnimationSelf(AnimTransform* animTrans);
            /* 0x54 */ virtual AnimationLink* FindAnimationLink(AnimTransform* animTrans);
            /* 0x58 */ virtual void SetAnimationEnable(AnimTransform* animTrans, bool bEnable,
                                                       bool bRecursive = true);
            /* 0x5C */ virtual Material* GetMaterial() const;
            /* 0x60 */ virtual void LoadMtx(const DrawInfo& drawInfo);

            Pane* GetParent() const { return mpParent; }
            PaneList& GetChildList() { return mChildList; }

            const math::VEC3& GetTranslate() { return mTranslate; }
            void SetTranslate(const math::VEC3& translate) { mTranslate = translate; }
            void SetTranslate(const math::VEC2& translate) {
                SetTranslate(math::VEC3(translate.x, translate.y, 0.0f));
            }

            const math::VEC3& GetRotate() const { return mRotate; }
            void SetRotate(const math::VEC3& rotate) { mRotate = rotate; }

            const math::VEC2& GetScale() const { return mScale; }
            void SetScale(const math::VEC2& scale) { mScale = scale; }

            const Size& GetSize() const { return mSize; }
            void SetSize(const Size& size) { mSize = size; }

            bool IsVisible() { return detail::TestBit(mFlag, BIT_VISIBLE); };
            void SetVisible(bool visible) { detail::SetBit(&mFlag, BIT_VISIBLE, visible); };

            bool IsInfluencedAlpha() { return detail::TestBit(mFlag, BIT_INFLUENCED_ALPHA); };
            void SetInfluencedAlpha(bool visible) {
                detail::SetBit(&mFlag, BIT_INFLUENCED_ALPHA, visible);
            };

            bool IsLocationAdjust() { return detail::TestBit(mFlag, BIT_LOCATION_ADJUST); };
            void SetLocationAdjust(bool visible) {
                detail::SetBit(&mFlag, BIT_LOCATION_ADJUST, visible);
            };

            const math::MTX34& GetGlobalMtx() const { return mGlbMtx; }
            void SetGlobalMtx(const math::MTX34& mtx) { mGlbMtx = mtx; }

            const math::MTX34& GetMtx() const { return mMtx; }
            void SetMtx(const math::MTX34& mtx) { mMtx = mtx; }

            u8 GetAlpha() { return mAlpha; }
            void SetAlpha(u8 alpha) { mAlpha = alpha; }

            const char* GetName() const { return mName; }

            void SetSRTElement(u32 idx, f32 value) {
                NW4HBM_ASSERT(250, idx < ANIMTARGET_PANE_MAX);
                reinterpret_cast<f32*>(&mTranslate)[idx] = value;
            }

            bool IsUserAllocated() const { return mbUserAllocated; }

            const ut::Rect GetPaneRect(const DrawInfo& drawInfo) const;

            math::VEC2 GetVtxPos() const;

            void SetName(const char* name);
            void SetUserData(const char* userData);

            void Init();

            void InsertChild(PaneList::Iterator next, Pane* pChild);
            void InsertChild(Pane* pNext, Pane* pChild);

            void PrependChild(Pane* pChild);
            void AppendChild(Pane* pChild);

            void RemoveChild(Pane* pChild);

            void CalculateMtxChild(const DrawInfo& drawInfo);

            void AddAnimationLink(AnimationLink* animationLink);

        protected:
            /* 0x00 (base) */
            /* 0x0C */ Pane* mpParent;
            /* 0x10 */ PaneList mChildList;
            /* 0x1C */ AnimationLinkList mAnimList;
            /* 0x28 */ Material* mpMaterial;
            /* 0x2C */ math::VEC3 mTranslate;
            /* 0x38 */ math::VEC3 mRotate;
            /* 0x44 */ math::VEC2 mScale;
            /* 0x4C */ Size mSize;
            /* 0x54 */ math::MTX34 mMtx;
            /* 0x84 */ math::MTX34 mGlbMtx;
            /* 0xB4 */ char mName[16];
            /* 0xC4 */ char mUserData[8];
            /* 0xCC */ u8 mBasePosition;
            /* 0xCD */ u8 mAlpha;
            /* 0xCE */ u8 mGlbAlpha;
            /* 0xCF */ u8 mFlag;
            /* 0xD0 */ bool mbUserAllocated;
        };  // size = 0xD4

    }  // namespace lyt
}  // namespace nw4hbm

#endif
