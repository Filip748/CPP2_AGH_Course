#pragma once 

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

namespace StringUtils {
    inline std::string toLower(const std::string& str) {
        std::string res = str;
        std::transform(res.begin(), res.end(), res.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return res;
    }

    inline std::string removePrefix(const std::string& str, const std::string& prefix) {
        if(str.find(prefix) == 0) {
            return str.substr(prefix.length());
        }     
        return str;
    }
}

class StringComparator {
public:
    virtual ~StringComparator() = default;
    // Funktor zwracający true, jesli lhs < rhs  wg specyficznego kryterium
    virtual bool operator()(const std::string& lhs, const std::string& rhs) const = 0;
};

class CaseInsensitiveLess : public StringComparator {
public:
    bool operator()(const std::string& lhs, const std::string& rhs) const override {
        return StringUtils::toLower(lhs) < StringUtils::toLower(rhs);
    }
};

class CaseSensitiveLess : public StringComparator {
public:
    bool operator()(const std::string& lhs, const std::string& rhs) const override {
        return lhs < rhs;
    }
};

class LengthLess : public StringComparator {
public:
    bool operator()(const std::string& lhs, const std::string& rhs) const override {
        return lhs.length() < rhs.length();
    }
};

class IgnorePrefixComparator : public StringComparator {
private:
    const StringComparator& m_baseComparator;
    std::string m_prefix;
public:
    IgnorePrefixComparator(const StringComparator& base, const std::string& prefix)
        : m_baseComparator(base), m_prefix(prefix) {}
    bool operator()(const std::string& lhs, const std::string& rhs) const override {
        return m_baseComparator(StringUtils::removePrefix(lhs, m_prefix),
                                StringUtils::removePrefix(rhs, m_prefix));
    }
};

class CompositeComparator {
private:
    std::vector<const StringComparator*> m_comparators;

public:
    void add(const StringComparator* cmp) {
        m_comparators.push_back(cmp);
    }

    bool operator()(const std::string& lhs, const std::string& rhs) const {
        for(const auto* cmp : m_comparators) {
            if((*cmp)(lhs, rhs)) {
                return true;
            }
            if((*cmp)(rhs,lhs)) {
                return false;
            }
        }
        return false;
    }
};