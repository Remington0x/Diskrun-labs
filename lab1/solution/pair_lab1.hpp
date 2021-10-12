#ifndef PAIR_LAB1_HPP
#define PAIR_LAB1_HPP

template <class T1, class T2>
class TPair {
public:
    T1 first;
    T2 second;
    TPair() {}
    TPair(T1 f, T2 s) : first(f), second(s) {}
    ~TPair() {}
    template <class U1, class U2>
    friend bool operator > (TPair a, TPair b);
    template <class U1, class U2>
    friend bool operator < (TPair a, TPair b);
    template <class U1, class U2>
    friend bool operator == (TPair a, TPair b);
};

template <class T1, class T2>
bool operator > (const TPair<T1, T2> & a, const TPair<T1, T2> & b) {
    return a.first > b.first;
}

template <class T1, class T2>
bool operator < (const TPair<T1, T2> & a, const TPair<T1, T2> & b) {
    return a.first < b.first;
}

template <class T1, class T2>
bool operator == (const TPair<T1, T2> & a, const TPair<T1, T2> & b) {
    return a.first == b.first;
}

#endif