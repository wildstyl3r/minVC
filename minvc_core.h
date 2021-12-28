#ifndef MINVC_CORE_H
#define MINVC_CORE_H

#include <grypho.hpp>
#include <set>
#include <chrono>
#include <functional>
#include <bitset>

using hc = std::chrono::high_resolution_clock;

enum class VCMode{
    Exhaustive,
    LogNCombK,
    CliqueMinB,
    CliqueMaxB,
    CliqueAdapt,
};

enum class KClMode{
    MinB,
    MaxB,
    Adaptive,
};

class minVC_core
{
public:
    minVC_core(VCMode mode = VCMode::Exhaustive);
    minVC_core(string filename, VCMode mode);
    minVC_core(Graph& G, VCMode mode);
    const Graph* graph() const;
    VCMode mode() const;
    hc::duration time() const;
    void setMode(VCMode mode);
    void setGraph(Graph& G);
    void setGraph(string filename);
    void setStepCallback(std::function<void()> = nullptr);
    bool isCover(std::set<vertex>& test);
    bool nonsense();
    std::set<vertex> cover();
private:
    bool _nonsense = false;
    vector<vertex> _g_enum;
    std::function<void()> _onstep;
    bool tryCover(std::set<vertex>& test);
    void minCover();
    void backtrack(vector<short>& subset);
    void logComb();

    KClMode _cl_mode;
    void clique();
    vector<vertex> KCl(Graph g, size_t k);
    size_t ClLimit(Graph& g);

    bool kCover(size_t k);
    void drawCurrent();
    VCMode _mode;
    Graph _g;
    std::set<vertex> _vertex_cover;
    hc::duration _time;
};

#endif // MINVC_CORE_H
