#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio/fvb-data-parse.h"

void JStudio::fvb::data::TParse_TParagraph::getData(TParse_TParagraph::TData* data) const
{
	u16* parse = (u16*)JGadget::binary::parseVariableUInt_16_32_following(getRaw(), (u32*)data, (u32*)&data->u32Type, NULL);
	u32 t      = (u32)data->u32Size;
	if (!t) {
		data->pContent = NULL;
		data->next    = parse;
	} else {
		data->pContent = parse;
		data->next    = parse + ((t + 3) >> 1 & ~1);
	}
}
