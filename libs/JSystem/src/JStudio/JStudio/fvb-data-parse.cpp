#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio/fvb-data-parse.h"

void JStudio::fvb::data::TParse_TParagraph::getData(TParse_TParagraph::TData* pData) const
{
	JUT_ASSERT(24, pData!=NULL);
	const void *ptr = getRaw();
	ptr = JGadget::binary::parseVariableUInt_16_32_following(ptr, (u32*)pData, (u32*)&pData->u32Type, NULL);
	u32 t      = (u32)pData->u32Size;
	if (!t) {
		pData->pContent = NULL;
		pData->next    = (const u16*)ptr;
	} else {
		pData->pContent = (const u16*)ptr;
		pData->next    = (u8*)ptr + JGadget::binary::align_roundUp(t, 4);
	}
}
