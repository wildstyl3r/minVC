#include "minvc_core.h"

minVC_core::minVC_core(VCMode mode)
{
    _mode = mode;
}

minVC_core::minVC_core(string filename, VCMode mode)
{
    _mode = mode;
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
        case VCMode::CliqueMinB:
            _cl_mode = KClMode::MinB;
            clique();
            break;
        case VCMode::CliqueMaxB:
            _cl_mode = KClMode::MaxB;
            clique();
            break;
        case VCMode::CliqueAdapt:
            _cl_mode = KClMode::Adaptive;
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
