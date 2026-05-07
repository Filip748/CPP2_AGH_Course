#pragma once
#include <vector>
#include <algorithm>

namespace Querying {
    class Query {
    public:
        virtual ~Query() = default;
        virtual bool check(int v) const = 0;
        virtual Query* clone() const = 0;
    };

    namespace Simple {
        class GreaterThan : public Query {
        private:
            int m_l;
        public:
            GreaterThan(int l) : m_l(l) {}
            bool check(int v) const override { return v > m_l; }
            Query* clone() const override { return new GreaterThan(m_l); }
        };

        class LessThan : public Query {
        private:
            int m_l;
        public:
            LessThan(int l) : m_l(l) {}
            bool check(int v) const override { return v < m_l; }
            Query* clone() const override { return new LessThan(m_l); }
        };

        class EqualTo : public Query {
        private:
            int m_l;
        public:
            EqualTo (int l) : m_l(l) {}
            bool check(int v) const override { return v == m_l; }
            Query* clone() const override { return new EqualTo (m_l); }
        };


        class IsEven : public Query {
        public:
            bool check(int v) const override { return v % 2 == 0; }
            Query* clone() const override { return new IsEven(); }
        };

        class NotQuery : public Query {
        private:
            Query* m_q;
        public:
            NotQuery(Query* q) : m_q(q) {}
            ~NotQuery() { delete m_q; }
            NotQuery(const NotQuery& o) : m_q(o.m_q-> clone()) {}
            bool check(int v) const override { return !m_q->check(v); }
            Query* clone() const override { return new NotQuery(m_q->clone()); }
        };
        
    }

    class CompositeQuery : public Query {
    private:
        std::vector<Query*> m_qs;
        void clear() { 
            for(auto q : m_qs) {
                delete q; 
            }
            m_qs.clear();
        }
    public:
        CompositeQuery(int cap = 10) {
            m_qs.reserve(cap); 
        }

        ~CompositeQuery() { clear(); }
        CompositeQuery(const CompositeQuery& o) {
            for(auto q : o.m_qs) {
                m_qs.push_back(q->clone());
            }
        }
        CompositeQuery& operator=(const CompositeQuery& o) {
            if(this != &o) {
                clear();
                for(auto q : o.m_qs) {
                    m_qs.push_back(q-> clone());
                }
            }
            return *this;
        }

        void addQuery(Query* q) {
            m_qs.push_back(q);
        }

        bool check(int v) const override {
            return std::all_of(m_qs.begin(), m_qs.end(), [v](Query* q) { return q->check(v);});
        }

        Query* clone() const override { return new CompositeQuery(*this); }
    };

    inline std::vector<int> filter(const std::vector<int>& data, const Query& q) {
        std::vector<int> res;
        for(int v : data) {
            if(q.check(v)) {
                res.push_back(v);
            }
        }
        return res;
    }
}