#include <minvc_core.h>

void minVC_core::logComb()
{
    size_t l = 0, m, r = _g.size()-1;
    while(l+1 < r){
        m = l + (r-l)/2;
        if (kCover(m)) {
            r = m;
        } else {
            l = m;
        }
    }
}

bool minVC_core::kCover(size_t k)
{
    vector<size_t> comb(k);
    for (size_t i = 0; i < k; ++i) {
        comb[i] = i;
    }
    while(true){
        std::set<vertex> test;
        for(size_t v : comb){
            test.insert(_g_enum[v]);
        }
        if(tryCover(test))
            return true;
        size_t i = k-1;
        while(i+1 > 0 && ++comb[i] > _g.size() - (k - i)) i--;
        while(i < k-1){
            comb[i+1] = comb[i]+1;
            i++;
        }
        if (comb[k-1] > _g.size() - 1) break;
    }
    return false;
}
