#include <minvc_core.h>

std::set<vertex> minVC_core::approxVC(Graph test)
{
    std::set<vertex> res;
    while (test.edges()) {
        vertex v, u;
        bool found = false;
        for(auto& [v_cand, _] : test.V()){
            for(auto& u_cand : test.V(v_cand)){
                v = v_cand;
                u = u_cand;
                found = true;
                break;
            }
            if(found) break;
        }
        res.insert(v);
        res.insert(u);
        test.remove_vertex(v);
        test.remove_vertex(u);
    }
    return res;
}
