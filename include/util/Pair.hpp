#pragma once

namespace ls {
    template <class T1, class T2>
    class Pair {
    public:
        static Pair of(T1 first, T2 second) { return Pair(first, second); }

    public:
        T1 getFirst()  { return m_first; }
        T2 getSecond() { return m_second; }
        
        Pair& operator = (const Pair& other) {
            this->m_first = other.m_first;
            this->m_second = other.m_second;
            return *this;
        }

    private:
        Pair() = delete;
        Pair(T1 first, T2 second) : m_first(first), m_second(second) { }
        
        T1 m_first;
        T2 m_second;
    };
}
