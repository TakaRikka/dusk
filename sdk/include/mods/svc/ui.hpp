#pragma once

#include <mods/svc/ui.h>

#include <string>
#include <utility>

namespace mods::ui {

inline ModResult get_clipboard_text(std::string& outText) {
    outText.clear();

    size_t textLength = 0;
    auto result = svc_ui->get_clipboard_text(mod_ctx, nullptr, 0, &textLength);
    if (result != MOD_OK) {
        return result;
    }

    for (int attempt = 0; attempt < 3; ++attempt) {
        std::string buffer(textLength + 1, '\0');
        size_t actualLength = 0;

        result = svc_ui->get_clipboard_text(
            mod_ctx, buffer.data(), buffer.size(), &actualLength);

        if (result == MOD_OK) {
            buffer.resize(actualLength);
            outText = std::move(buffer);
            return MOD_OK;
        }

        // Retry if the clipboard grew between the two calls.
        if (result != MOD_INVALID_ARGUMENT || actualLength <= textLength) {
            return result;
        }

        textLength = actualLength;
    }

    return MOD_ERROR;
}

inline ModResult set_clipboard_text(const std::string& text) {
    return svc_ui->set_clipboard_text(mod_ctx, text.c_str());
}

}  // namespace mods::ui