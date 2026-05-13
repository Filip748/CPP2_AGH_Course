#pragma once 

#include <vector>
#include <array>
#include <functional>
#include <algorithm>

namespace Iterators {

    class VectorIterator {
    private:
        const std::vector<int>& vec;
        std::array<std::function<bool(int)>, 5> filters;
        int fcount = 0;
        std::array<std::function<int(int)>, 5> maps;
        int mcount = 0;
        size_t limit;


    public:
    
        VectorIterator(const std::vector<int>& v) : vec(v), fcount(0), mcount(0), limit(v.size()) {}
    
        VectorIterator& filter(std::function<bool(int)> f) {
            filters[fcount] = f;
            fcount++;
            return *this;
        }

        VectorIterator& map(std::function<int(int)> m) {
            maps[mcount] = m;
            mcount++;
            return *this;
        }

        VectorIterator& take(size_t n) {
            limit = std::min(n, limit);
            return *this;
        }

        std::vector<int> collect() {
            std::vector<int> ans;
            ans.reserve(limit);
            
            for(const auto& el : vec) {
                bool temp = true;

                for(int i = 0; i < fcount; i++) {
                    if(!filters[i](el)) { 
                        temp = false; 
                        break;
                    }
                }
                
                if(!temp) { continue; }

                int value = el;

                for(int i = 0; i < mcount; i++) {
                    value = maps[i](value);
                }

                ans.push_back(value);

                if(ans.size() == limit) { break; }
            }
            return ans;
        }
 
    };
}