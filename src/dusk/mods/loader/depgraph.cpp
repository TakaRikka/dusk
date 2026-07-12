#include "depgraph.hpp"

#include <algorithm>
#include <string>

#include "dusk/logging.h"
#include "loader.hpp"
#include "native_module.hpp"  // IWYU pragma: keep

namespace dusk::mods::loader {
namespace {
aurora::Module Log{"dusk::mods::loader"};

struct Edge {
    size_t provider;
    size_t importer;
    bool required;
    bool alive = true;
};

std::vector<Edge> collect_edges(const std::vector<std::unique_ptr<LoadedMod>>& mods) {
    for (auto& mod : mods) {
        mod->dependencies.clear();
        mod->dependents.clear();
    }

    // Mirrors the registry's first-registration-wins rule for duplicate exports.
    const auto findProvider = [&](const ModManifestInfo::Import& serviceImport) -> size_t {
        for (size_t i = 0; i < mods.size(); ++i) {
            const auto matches = [&](const ModManifestInfo::Export& serviceExport) {
                return serviceExport.major == serviceImport.major &&
                       serviceExport.id == serviceImport.id;
            };
            if (std::ranges::any_of(mods[i]->manifestInfo.exports, matches)) {
                return i;
            }
        }
        return mods.size();  // Host-provided or unavailable: no ordering constraint.
    };

    std::vector<Edge> edges;
    for (size_t importer = 0; importer < mods.size(); ++importer) {
        auto& mod = *mods[importer];
        for (const auto& serviceImport : mod.manifestInfo.imports) {
            const size_t provider = findProvider(serviceImport);
            if (provider >= mods.size() || provider == importer) {
                continue;
            }
            auto& providerMod = *mods[provider];
            const bool required = serviceImport.required;

            const auto existing = std::ranges::find_if(edges, [&](const Edge& edge) {
                return edge.provider == provider && edge.importer == importer;
            });
            if (existing != edges.end()) {
                if (required && !existing->required) {
                    existing->required = true;
                    for (auto& dep : mod.dependencies) {
                        if (dep.mod == &providerMod) {
                            dep.required = true;
                        }
                    }
                    for (auto& dep : providerMod.dependents) {
                        if (dep.mod == &mod) {
                            dep.required = true;
                        }
                    }
                }
            } else {
                edges.push_back({provider, importer, required});
                mod.dependencies.push_back({&providerMod, required});
                providerMod.dependents.push_back({&mod, required});
            }
        }
    }
    return edges;
}

// True if `start` can reach itself following live required edges through unplaced mods.
bool in_required_cycle(
    const size_t start, const std::vector<Edge>& edges, const std::vector<bool>& placed) {
    std::vector pending{start};
    std::vector visited(placed.size(), false);
    while (!pending.empty()) {
        const size_t current = pending.back();
        pending.pop_back();
        for (const auto& edge : edges) {
            if (!edge.alive || !edge.required || edge.provider != current || placed[edge.importer])
            {
                continue;
            }
            if (edge.importer == start) {
                return true;
            }
            if (!visited[edge.importer]) {
                visited[edge.importer] = true;
                pending.push_back(edge.importer);
            }
        }
    }
    return false;
}

}  // namespace

void sort_mods(std::vector<std::unique_ptr<LoadedMod>>& mods) {
    const size_t count = mods.size();
    auto edges = collect_edges(mods);
    if (edges.empty()) {
        return;
    }

    std::vector<size_t> indegree(count, 0);
    for (const auto& edge : edges) {
        ++indegree[edge.importer];
    }

    std::vector<size_t> order;
    order.reserve(count);
    std::vector placed(count, false);

    const auto place = [&](const size_t index) {
        placed[index] = true;
        order.push_back(index);
        for (auto& edge : edges) {
            if (edge.alive && edge.provider == index) {
                edge.alive = false;
                --indegree[edge.importer];
            }
        }
    };

    while (order.size() < count) {
        // Always take the lowest unplaced scan index that is ready, keeping the
        // final order as close to scan (filename) order as the graph allows.
        const auto ready = [&]() -> size_t {
            for (size_t i = 0; i < count; ++i) {
                if (!placed[i] && indegree[i] == 0) {
                    return i;
                }
            }
            return count;
        }();
        if (ready < count) {
            place(ready);
            continue;
        }

        // Stalled: every unplaced mod is on or downstream of a cycle.
        std::vector<size_t> cycleMods;
        for (size_t i = 0; i < count; ++i) {
            if (!placed[i] && in_required_cycle(i, edges, placed)) {
                cycleMods.push_back(i);
            }
        }
        if (!cycleMods.empty()) {
            std::string names;
            for (const size_t index : cycleMods) {
                if (!names.empty()) {
                    names += ", ";
                }
                names += mods[index]->metadata.id;
            }
            for (const size_t index : cycleMods) {
                fail_mod(*mods[index], MOD_CONFLICT,
                    "Required service import cycle between mods: " + names);
                place(index);
            }
            continue;
        }

        // Only optional imports left in the cycle: drop one edge and retry. The
        // import still resolves, but without any initialization-order guarantee.
        const auto optionalEdge = std::ranges::find_if(edges, [&](const Edge& edge) {
            return edge.alive && !edge.required && !placed[edge.provider] && !placed[edge.importer];
        });
        if (optionalEdge == edges.end()) {
            // Unreachable: a stall with no required cycle implies an optional edge.
            Log.error("mod dependency sort stalled unexpectedly");
            break;
        }
        Log.warn("optional service import cycle: '{}' will initialize before its optional "
                 "provider '{}'",
            mods[optionalEdge->importer]->metadata.id, mods[optionalEdge->provider]->metadata.id);
        optionalEdge->alive = false;
        --indegree[optionalEdge->importer];
    }

    // Defensive: append anything a stall break left behind, in scan order.
    for (size_t i = 0; i < count; ++i) {
        if (!placed[i]) {
            place(i);
        }
    }

    std::vector<std::unique_ptr<LoadedMod>> sorted;
    sorted.reserve(count);
    for (const size_t index : order) {
        sorted.push_back(std::move(mods[index]));
    }
    mods = std::move(sorted);
}

}  // namespace dusk::mods::loader
