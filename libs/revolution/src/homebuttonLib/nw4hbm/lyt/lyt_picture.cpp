#include "picture.h"

#include "layout.h"

#include <new>

namespace nw4hbm {
    namespace lyt {

        NW4HBM_UT_GET_DERIVED_RUNTIME_TYPEINFO(Picture, Pane);

        Picture::Picture(const res::Picture* pResPic, const ResBlockSet& resBlockSet)
            : Pane(pResPic) {
            u8 texCoordNum = ut::Min<u8>(pResPic->texCoordNum, 8);

            Init(texCoordNum);

            for (int i = 0; i < (int)ARRAY_SIZE(mVtxColors); i++) {
                mVtxColors[i] = pResPic->vtxCols[i];
            }

            if (texCoordNum && !mTexCoordAry.IsEmpty()) {
                mTexCoordAry.Copy(&pResPic[1], texCoordNum);
            }

            if (Material* pMemMaterial =
                    static_cast<Material*>(Layout::AllocMemory(sizeof(*pMemMaterial))))
            {
                NW4HBM_ASSERT_CHECK_NULL(149, resBlockSet.pMaterialList);
                const u32* matOffsTbl =
                    detail::ConvertOffsToPtr<u32>(resBlockSet.pMaterialList, 0xc);
                const res::Material* pResMaterial = detail::ConvertOffsToPtr<res::Material>(
                    resBlockSet.pMaterialList, matOffsTbl[pResPic->materialIdx]);

                mpMaterial = new (pMemMaterial) Material(pResMaterial, resBlockSet);
            }
        }

        void Picture::Init(u8 texNum) {
            if (texNum) {
                ReserveTexCoord(texNum);
            }
        }

        Picture::~Picture() {
            if (mpMaterial && !mpMaterial->IsUserAllocated()) {
                mpMaterial->~Material();
                Layout::FreeMemory(mpMaterial);
                mpMaterial = NULL;
            }

            mTexCoordAry.Free();
        }

        void Picture::Append(TPLPalette* pTplRes) {
            GXTexObj texObj;
            detail::InitGXTexObjFromTPL(&texObj, pTplRes, 0);

            Append(texObj);
        }

        void Picture::Append(const GXTexObj& texObj) {
            if (mpMaterial->GetTextureNum() >= mpMaterial->GetTextureCap() ||
                mpMaterial->GetTextureNum() >= mpMaterial->GetTexCoordGenCap())
            {
                NW4R_DB_WARNING(
                    192, false,
                    "mpMaterial->GetTextureNum(%d) is large. mpMaterial->GetTextureCap(%d), "
                    "mpMaterial->GetTexCoordGenCap(%d)\n",
                    mpMaterial->GetTextureNum(), mpMaterial->GetTextureCap(),
                    mpMaterial->GetTexCoordGenCap());
                return;
            }

            u8 texIdx = mpMaterial->GetTextureNum();
            mpMaterial->SetTextureNum(texIdx + 1);
            mpMaterial->SetTexture(texIdx, texObj);
            mpMaterial->SetTexCoordGenNum(mpMaterial->GetTextureNum());
            mpMaterial->SetTexCoordGen(texIdx, TexCoordGen());

            SetTexCoordNum(mpMaterial->GetTextureNum());

            if (mSize == Size(0.0f, 0.0f) && mpMaterial->GetTextureNum() == 1) {
                mSize = detail::GetTextureSize(mpMaterial, 0);
            }
        }

        void Picture::ReserveTexCoord(u8 num) {
            mTexCoordAry.Reserve(num);
        }

        void Picture::SetTexCoordNum(u8 num) {
            mTexCoordAry.SetSize(num);
        }

        ut::Color Picture::GetVtxColor(u32 idx) const {
            NW4HBM_ASSERT(251, idx < VERTEXCOLOR_MAX);
            return mVtxColors[idx];
        }

        void Picture::SetVtxColor(u32 idx, ut::Color value) {
            NW4HBM_ASSERT(262, idx < VERTEXCOLOR_MAX);
            mVtxColors[idx] = value;
        }

        u8 Picture::GetVtxColorElement(u32 idx) const {
            return detail::GetVtxColorElement(mVtxColors, idx);
        }

        void Picture::SetVtxColorElement(u32 idx, u8 value) {
            detail::SetVtxColorElement(mVtxColors, idx, value);
        }

        void Picture::DrawSelf(const DrawInfo& drawInfo) {
            if (!mpMaterial) {
                return;
            }

            LoadMtx(drawInfo);

            bool bUseVtxCol = mpMaterial->SetupGX(
                detail::IsModulateVertexColor(mVtxColors, mGlbAlpha), mGlbAlpha);
            nw4hbm::lyt::detail::SetVertexFormat(bUseVtxCol, mTexCoordAry.GetSize());

            detail::DrawQuad(GetVtxPos(), mSize, mTexCoordAry.GetSize(), mTexCoordAry.GetArray(),
                             bUseVtxCol ? mVtxColors : NULL, mGlbAlpha);
        }

    }  // namespace lyt
}  // namespace nw4hbm
