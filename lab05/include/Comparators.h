#pragma once

#include <string>
#include <vector>
#include <algorithm>

namespace StringUtils {
    std::string myToLower(const std::string& lower) {
        std::string newStr;
        for(auto c : lower) {
            newStr += tolower(c);
        }
        return newStr;
    }

    std::string removePrefix(const std::string& str, const std::string& prefix) {
        std::string s = myToLower(str);
        std::string p = myToLower(prefix);
        if(s.rfind(p, 0) == 0) { return str.substr(prefix.length()); }
        return str;
    }
}


class StringComparator {
public:
    virtual ~StringComparator() = default;
    virtual bool operator()(const std::string& l, const std::string& r) const = 0;
};

class CaseInsensitiveLess : public StringComparator {
public:
    bool operator()(const std::string& l, const std::string& r) const override {
        return StringUtils::myToLower(l) < StringUtils::myToLower(r);
    }
};

class CaseSensitiveLess : public StringComparator {
public:
    bool operator()(const std::string& l, const std::string& r) const override {
        return l < r;
    }
};

class LengthLess : public StringComparator {
public: 
    bool operator()(const std::string& l, const std::string& r) const override {
        return l.length() < r.length();
    }
};

class IgnorePrefixComparator : public StringComparator {
private:
    const StringComparator& m_baseComparator;
    std::string m_prefix;

public:
    IgnorePrefixComparator(const StringComparator& base, const std::string& prefix) 
        : m_baseComparator(base), m_prefix(prefix) {}

    bool operator()(const std::string& l, const std::string& r) const override {
        return m_baseComparator(StringUtils::removePrefix(l, m_prefix),
                                StringUtils::removePrefix(r, m_prefix));
    }
};

class CompositeComparator { 
private:
    std::vector<const StringComparator*> m_comparators;
public:
    void add(const StringComparator* cmp) {
        m_comparators.push_back(cmp);
    }

    bool operator()(const std::string& l, const std::string& r) const {
        for(const auto* cmp : m_comparators) {
            if((*cmp)(l, r)) { 
                return true;
            }
            if((*cmp)(r, l)) {
                return false;
            }
        }
        return false;
    }
};