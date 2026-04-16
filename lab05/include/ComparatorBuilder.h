#pragma once

#include "Comparators.h"
#include <vector>
#include <deque>
#include <string>

class ComparatorBuilder {
private:
    CaseInsensitiveLess m_caseInsensitive;
    CaseSensitiveLess m_caseSensitive;
    LengthLess m_lengthLess;
    std::deque<IgnorePrefixComparator> m_prefixWrappers;
    std::vector<const StringComparator*> m_activeComparators;

public:
    ComparatorBuilder() {}

    ComparatorBuilder& caseInsensitive() {
        m_activeComparators.push_back(&m_caseInsensitive);
        return *this;
    }

    ComparatorBuilder& thenCaseSensitive() {
        m_activeComparators.push_back(&m_caseSensitive);
        return *this;
    }

    ComparatorBuilder& thenByLength() {
        m_activeComparators.push_back(&m_lengthLess);
        return *this;
    }

    ComparatorBuilder&  ignorePrefix(const std::string& prefix) {
        if(!m_activeComparators.empty()) {
            const StringComparator* last = m_activeComparators.back();
            m_prefixWrappers.emplace_back(*last, prefix);
            m_activeComparators.back() = &m_prefixWrappers.back();
        }
        return *this;
    }

    CompositeComparator build() const {
        CompositeComparator comp;
        for(const auto* cmp : m_activeComparators) {
            comp.add(cmp);
        }
        return comp;
    }
};