#include "minvc_core.h"
#include <queue>

minVC_core::minVC_core(VCMode mode)
{
    _mode = mode;
}

minVC_core::minVC_core(string filename, VCMode mode, bool apx_limits, std::function<void()> onstep) :
    _onstep(onstep), _mode(mode), use_approx_limit(apx_limits)
{
    setGraph(filename);
}

minVC_core::minVC_core(Graph& G, VCMode mode)
{
    _mode = mode;
    setGraph(G);
}

void minVC_core::setGraph(string filename)
{
    _g = Graph(filename);
    _g_enum.resize(_g.size());
    size_t i = 0;
    for(const auto& [v, _] : _g.V()){
        _g_enum[i] = v;
        i++;
    }
    minCover();
}

void minVC_core::setGraph(Graph& G)
{
    _g = G;
    _g_enum.resize(_g.size());
    size_t i = 0;
    for(const auto& [v, _] : _g.V()){
        _g_enum[i] = v;
        i++;
    }
    minCover();
}

const Graph* minVC_core::graph() const
{
    return &_g;
}

VCMode minVC_core::mode() const
{
    return _mode;
}

hc::duration minVC_core::time() const
{
    return _time;
}

void minVC_core::setMode(VCMode mode)
{
    _mode = mode;
}

bool minVC_core::isCover(std::set<vertex>& test)
{
    for(const auto& [v, _] : _g.V()){
        for(const auto& [u, _] : _g.V()){
            if(v < u && _g.has({v,u}) && !(test.count(v) || test.count(u))){
                return false;
            }
        }
    }
    return true;
}

void minVC_core::minCover()
{
    _vertex_cover.clear();
    for(const auto& [v, _] : _g.V()){
        _vertex_cover.insert(v);
    }
    hc::time_point begin = hc::now();
    if(_g.size()){
        if(tryBipartiteVC()){
            return;
        }
        vector<short> s;
        switch (_mode) {
        case VCMode::Exhaustive:
            if (_g.size() > 30) {
                _nonsense = true;
                return;
            }
            backtrack(s);
            break;
        case VCMode::LogNCombK:
            if (_g.size() > 33) {
                _nonsense = true;
                return;
            }
            logComb();
            break;
        case VCMode::Clique:
            clique();
            break;
        }
    }
    hc::time_point end = hc::now();
    _time = end-begin;
}

void minVC_core::setStepCallback(std::function<void()> callback)
{
    _onstep = callback;
}

bool minVC_core::nonsense()
{
    return _nonsense;
}

bool minVC_core::tryCover(std::set<vertex>& test)
{
    if(isCover(test) && test.size() < _vertex_cover.size()){
        _vertex_cover = test;
        drawCurrent();
        return true;
    } else {
        return false;
    }
}

std::set<vertex> minVC_core::cover()
{
    return _vertex_cover;
}

void minVC_core::drawCurrent()
{
    for(const auto& [v, _] : _g.V()){
        if(_vertex_cover.count(v)){
            _g.set_color(v,1);
        } else {
            _g.set_color(v,0);
        }
    }
}

void minVC_core::useApproxLimit(bool al)
{
    use_approx_limit = al;
}

unordered_map<vertex, bool> minVC_core::partition(Graph &g)
{
    unordered_map<vertex, bool> part;

    for (auto& [w, _] : g.V()){
        if (!part.count(w)) {
            std::queue<vertex> bfs;
            part[w] = true;
            bfs.push(w);
            while(!bfs.empty()){
                vertex v = bfs.front();
                bfs.pop();
                for(const vertex& u : g.V(v)){
                    if(!part.count(u)) {
                        part[u] = !part[v];
                        bfs.push(u);
                    } else {
                        if(part[u] == part[v])
                            return {};
                    }
                }
            }
        }
    }
    return part;
}

bool minVC_core::KuhnMatching(const vertex &v)
{
    if(used[v])
        return false;
    used[v] = true;
    for(const vertex& u : _g.V(v)){
        if(!matching.count(u) || KuhnMatching(u)){
            matching[u] = v;
            return true;
        }
    }
    return false;
}

void minVC_core::biVC(const vertex &v, unordered_map<vertex, bool>& part)
{
    if(used[v])
        return;
    used[v] = true;
    for(const vertex& u : _g.V(v)){
        if(matching.count(v) && matching[v] == u){
            if(part[u] == true){
                biVC(u, part);
            }
        } else {
            if(part[u] == false){// && part[v] == true) {
                biVC(u, part);
            }
        }
    }
    return;
}

bool minVC_core::tryBipartiteVC()
{
    unordered_map<vertex, bool> part = partition(_g);
    if(part.size()) {
        matching.clear();
        for(auto& [v, _] : _g.V()) {
            used.clear();
            KuhnMatching(v);
        }
        used.clear();
        for(auto& [v, isA] : part){
            if(isA && !matching.count(v)){
                biVC(v, part);
            }
        }
        std::set<vertex> bcover;
        for(auto& [v, _] : _g.V()) {
            if((part[v] == true && !used[v]) || (part[v] == false && used[v])){
                bcover.insert(v);
            }
        }

        return tryCover(bcover);
    } else {
        return false;
    }
}
