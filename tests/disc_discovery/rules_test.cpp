#include "dusk/disc_discovery_rules.hpp"

#include <cassert>
#include <cstdio>
#include <filesystem>
#include <vector>

using dusk::disc_discovery::Candidate;
using dusk::disc_discovery::is_disc_filename;
using dusk::disc_discovery::select_candidates;
namespace fs = std::filesystem;

#define CHECK(cond) do { if (!(cond)) { std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); return 1; } } while (0)

int main() {
    // extensions, case-insensitive
    CHECK(is_disc_filename(fs::path{"game.iso"}));
    CHECK(is_disc_filename(fs::path{"GAME.ISO"}));
    CHECK(is_disc_filename(fs::path{"tp.rvz"}));
    CHECK(is_disc_filename(fs::path{"tp.nkit.iso"}));
    CHECK(!is_disc_filename(fs::path{"readme.txt"}));
    CHECK(!is_disc_filename(fs::path{"tp.iso.txt"}));
    CHECK(!is_disc_filename(fs::path{"noext"}));
    CHECK(!is_disc_filename(fs::path{".iso"}));

    // ordering: bundle before data regardless of input order, alphabetical within origin, non-discs dropped, duplicates dropped
    std::vector<Candidate> found{
        {fs::path{"/data/discs/b.rvz"}, "data"},
        {fs::path{"/app/disc/notes.txt"}, "bundle"},
        {fs::path{"/app/disc/z.iso"}, "bundle"},
        {fs::path{"/app/disc/a.iso"}, "bundle"},
        {fs::path{"/data/a.iso"}, "data"},
        {fs::path{"/data/discs/b.rvz"}, "data"},
    };
    auto out = select_candidates(found);
    CHECK(out.size() == 4);
    CHECK(out[0].path == fs::path{"/app/disc/a.iso"} && out[0].origin == "bundle");
    CHECK(out[1].path == fs::path{"/app/disc/z.iso"} && out[1].origin == "bundle");
    CHECK(out[2].path == fs::path{"/data/a.iso"} && out[2].origin == "data");
    CHECK(out[3].path == fs::path{"/data/discs/b.rvz"} && out[3].origin == "data");

    std::puts("rules_test OK");
    return 0;
}
