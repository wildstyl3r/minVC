#include <minvc_core.h>

void minVC_core::clique()
{
    vector<vertex> result, test;

    Graph compl_g = !_g;

    size_t l = ClLowerBound(compl_g)-1, m, r = ClUpperBound(compl_g)+1;
    if(use_approx_limit) {
        auto apx = approxVC(_g);
        r = std::min(r, compl_g.size() - apx.size()/2 + 1);
    }
    while(l + 1 < r){
        if(_onstep) _onstep();
        m = l + (r - l)/2;
        test = KCl(compl_g, m);
        if(_onstep)_onstep();
        if(test.size() < m){
            r = m;
            if(test.size() > l){
                l = test.size();
            }
            if(test.size() > result.size()){
                result = test;
            }
        } else {
            l = test.size();
            result = test;
        }
    }

    for (vertex& v : result) {
        _vertex_cover.erase(v);
    }
    drawCurrent();
}

size_t minVC_core::ClLowerBound(Graph &g)
{
    return g.size() / (g.size() - 2 * g.edges() / g.size());
}

size_t minVC_core::ClUpperBound(Graph &g)
{
    vector<vertex> vx = g.ids();
    std::sort(vx.begin(), vx.end(), [&](const vertex& l, const vertex& r){
        return g.deg(l) > g.deg(r);
    });
    size_t l_lim = 0, m_lim, r_lim = vx.size();
    while(l_lim + 1 < r_lim){
        m_lim = l_lim + (r_lim - l_lim)/2;
        if(g.deg(vx[m_lim]) > m_lim){
            l_lim = m_lim;
        } else {
            r_lim = m_lim;
        }
    }
    return g.deg(vx[l_lim]) + 1;
}

vector<vertex> minVC_core::KCl(Graph g, size_t k)
{
    vector<vertex> result;

    if(k == 1){
        if(g.size()){
            result.push_back(g.V().begin()->first);
        }
        return result;
    }

    while(g.size() >= k){
        vector<vertex> vx = g.ids();
        std::sort(vx.begin(), vx.end(), [&](const vertex& l, const vertex& r){
            return g.deg(l) > g.deg(r);
        });
        if(g.deg(vx[k-1]) + 1 < k){
            return result;
        }
        if(g.deg(vx[vx.size()-1]) + 1 == vx.size()){
            return vx;
        }
        if(vx.size() > 2 && g.deg(vx[vx.size()-3]) + 1 == vx.size()){
            vx.resize(vx.size()-1);
            return vx;
        }

        size_t l_tr = 0, m_tr, r_tr = vx.size();
        while(l_tr + 1 < r_tr){
            m_tr = l_tr + (r_tr - l_tr)/2;
            if(g.deg(vx[m_tr]) + 1 < k){
                r_tr = m_tr;
            } else {
                l_tr = m_tr;
            }
        }
        if(r_tr < vx.size()){
            while(r_tr < vx.size()){
                g.remove_vertex(vx[r_tr]);
                r_tr++;
            }

            continue;
        }

        //limit = ClUpperBound(g);

        //if(limit < k){
        //    return result;
        //} else {
        vertex t = vx.back();
        vector<vertex> test = KCl(g.N(t), k - 1);
        test.push_back(t);

        if (test.size() >= k){
            return test;
        }

        if (test.size() > result.size()){
            result = test;
        }

        g.remove_vertex(t);
        //}
    }
    return result;
}
