#include <minvc_core.h>
//#include <iostream>

void minVC_core::clique()
{
    vector<vertex> result, test;

    Graph compl_g = !_g;

    size_t l = 0, m, r = ClLimit(compl_g)+1;
    while(l + 1 < r){
        m = l + (r - l)/2;
        test = KCl(compl_g, m);
        //std::cout << m << ':' << test.size() << '\n';
        if(test.size() < m){
            r = m;
            if(test.size() > l){
                l = test.size();
                result = test;
            }
        } else {
            l = test.size();
            result = test;
        }
    }
    //std::cout.flush();

    for (vertex& v : result) {
        _vertex_cover.erase(v);
    }
    drawCurrent();
}

size_t minVC_core::ClLimit(Graph &g)
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
    return g.deg(vx[r_lim]) + 1;
}

vector<vertex> minVC_core::KCl(Graph g, size_t k)
{
    vector<vertex> result;
    size_t limit;

    if(k == 1){
        if(g.size() > 0){
            result.push_back(g.V().begin()->first);
        }
        return result;
    }

    while(g.size()){
        vector<vertex> vx = g.ids();
        std::sort(vx.begin(), vx.end(), [&](const vertex& l, const vertex& r){
            return g.deg(l) > g.deg(r);
        });
        if(g.deg(vx[0]) + 1 < k){
            return result;
        }
        if(g.deg(vx[vx.size()-1]) + 1 == vx.size()){
            return vx;
        }
        if(g.deg(vx[vx.size()-3]) + 1 == vx.size()){
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

        limit = ClLimit(g);

        if(limit < k){
            return result;
        } else {
            vertex t;
            switch (_cl_mode) {
            case KClMode::MinB:
                t = vx.back();
                break;
            case KClMode::MaxB:
                t = vx.front();
                break;
            case KClMode::Adaptive:
                if(g.density() > 0.5){
                    t = vx.front();
                } else {
                    t = vx.back();
                }
                break;
            }
            vector<vertex> test = KCl(g.N(t), k - 1);
            test.push_back(t);

            if (test.size() >= k){
                return test;
            }

            if (test.size() > result.size()){
                result = test;
            }

            g.remove_vertex(t);
        }
    }
    return result;
}
