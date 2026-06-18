#include <iostream>
#include <string>
#include <vector>
#include <algorithm> 
#include <utility>


enum class SortingDirection {
    ASCENDING
};

class DataSet {
private:
    std::string name;
    std::vector<double> vec;
public:
    DataSet(const std::string& n, const std::vector<double>& v) : name(n), vec(v) {}

    const std::vector<double> getV() { return vec; }

    friend std::ostream& operator<<(std::ostream& os, DataSet& d) {
        os << d.name << ": [";
        for(int i = 0; i < d.vec.size(); i++) {
            os << d.vec[i];
            if (i < d.vec.size() - 1) os << ", ";
        }
        return os << "]";
    }
};

class DataStore {
private:
    int capacity;
    std::vector<DataSet> data;

public:
    DataStore(int c) : capacity(c) {
        data.reserve(10);
    }

    void addDataset(std::string name, std::vector<double> vec) {
        DataSet set{std::move(name), std::move(vec)};
        data.push_back(std::move(set));
    }

    template <typename T>
    void addDataset(T&& dataset) {
        data.push_back(std::forward<T>(dataset));
    }

    auto begin() {
        return data.begin();
    }

    auto end() {
        return data.end();
    }
     
    template <class T>
    void sort(T&& calc, SortingDirection direction) {
        std::vector<std::pair<double, int>> cache;
        cache.reserve(data.size());
        for(int i = 0; i < data.size(); i++) {
            auto& v = data[i].getV();
            double c = calc(v.begin(), v.end());
            cache.emplace_back(c, i);
        }

        std::sort(cache.begin(), cache.end(), [direction](const auto& a, const auto& b) {
            if (direction == SortingDirection::ASCENDING) {
                    return a.first < b.first;
                } else {
                    return b.first < a.first;
                }
        });

        std::vector<DataSet> sorted;
        sorted.reserve(data.size());

        for (const auto& item : cache) {
            sorted.push_back(std::move(data[item.second]));
        }

        data = std::move(sorted);
    }
   
    
};