#ifndef DUSK_TRANSFER_UPLOADER_PAGE_HPP
#define DUSK_TRANSFER_UPLOADER_PAGE_HPP

#include <string_view>

namespace dusk::transfer {

// Served verbatim by GET /, with %%ACCEPTED_GAME_IDS%% substituted for the catalog's ids. Must stay
// self-contained -- no external CSS, fonts or scripts -- because the Apple TV serving it has no
// route the browser could fetch them from.
inline constexpr std::string_view kUploaderPage = R"HTML(<!doctype html>
<meta charset="utf-8"><title>Add a disc</title>
<p>Uploader page placeholder.</p>
<script>const ACCEPTED_GAME_IDS = %%ACCEPTED_GAME_IDS%%;</script>
)HTML";

}  // namespace dusk::transfer

#endif  // DUSK_TRANSFER_UPLOADER_PAGE_HPP
