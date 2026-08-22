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

    // dedupe is first-occurrence-wins, not best-rank-wins: the same path appears first with
    // origin "data" and again later with origin "bundle" (a better rank); the survivor must
    // keep the first occurrence's origin and therefore sort in the "data" group.
    std::vector<Candidate> dup_different_origin{
        {fs::path{"/dup/tp.iso"}, "data"},
        {fs::path{"/dup/tp.iso"}, "bundle"},
    };
    auto dup_out = select_candidates(dup_different_origin);
    CHECK(dup_out.size() == 1);
    CHECK(dup_out[0].path == fs::path{"/dup/tp.iso"} && dup_out[0].origin == "data");

    // origins other than "bundle"/"data" fall into the lowest-priority "other" tier and sort
    // after both known origins.
    std::vector<Candidate> other_origin{
        {fs::path{"/app/disc/a.iso"}, "bundle"},
        {fs::path{"/data/a.iso"}, "data"},
        {fs::path{"/misc/other.iso"}, "other"},
    };
    auto other_out = select_candidates(other_origin);
    CHECK(other_out.size() == 3);
    CHECK(other_out[0].path == fs::path{"/app/disc/a.iso"} && other_out[0].origin == "bundle");
    CHECK(other_out[1].path == fs::path{"/data/a.iso"} && other_out[1].origin == "data");
    CHECK(other_out[2].path == fs::path{"/misc/other.iso"} && other_out[2].origin == "other");

    std::puts("rules_test OK");
    return 0;
}
