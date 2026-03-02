#include "DbgPrintBase.h"

#include "../ut/Color.h"
#include "../ut/CharWriter.h"

// Unused file. Only here to force the function CharWriter::SetTextColor(ut::Color&)
// into the right place.

namespace nw4hbm {
    namespace db {
        // Dummy function
        void dummy(ut::CharWriter* pCharWriter) {
            ut::Color color;
            pCharWriter->SetTextColor(color);
        }
    }  // namespace db
}  // namespace nw4r
