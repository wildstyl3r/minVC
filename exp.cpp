#include <minvc_core.h>

void minVC_core::backtrack(vector<short>& subset){
    if(subset.size() == _g.size()){
        std::set<vertex> t;
        for(size_t i = 0; i < _g.size(); ++i){
            if (subset[i])
                t.insert(_g_enum[i]);
        }
        tryCover(t);
        return;
    }
    subset.push_back(false);
    backtrack(subset);
    subset.pop_back();

    subset.push_back(true);
    backtrack(subset);
    subset.pop_back();
}
