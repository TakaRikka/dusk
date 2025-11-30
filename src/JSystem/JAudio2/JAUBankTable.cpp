#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAUBankTable.h"

void JAUBankTableDictionary::appendBankTable(JSULink<JAUBankTable>* bankTableList) {
    append(bankTableList);
}

// Needed to make the inline function JAUBankTable::getBank appear in this TU
static const JASBank* dummy(const JAUBankTable* table) {
    return table->getBank(0);
}
