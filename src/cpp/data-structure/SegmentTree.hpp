#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>
using namespace std;

/// start SegmentTree
/// @prefix: cpSegmentTree
/// @description: SegmentTree : セグメント木
/// @filetemplate: false
template <class S, auto op, auto e>
struct SegmentTree {
public:
    SegmentTree() : SegmentTree(0) {}
    explicit SegmentTree(const size_t n) : SegmentTree(vector<S>(n, e())) {}
    explicit SegmentTree(const vector<S>& v) : m_size(v.size()) {
        for (m_leafSize = 1; m_leafSize < m_size;) {
            m_leafSize <<= 1;
        }
        m_segment.assign(2 * m_leafSize, e());
        for (int i = 0; i < m_size; i++) {
            m_segment[m_leafSize + i] = v[i];
        }
        for (int i = m_leafSize - 1; i >= 1; i--) {
            m_segment[i] = op(m_segment[2 * i], m_segment[2 * i + 1]);
        }
    }
    void set(size_t p, const S& x) {
        assert(0 <= p && p < m_size);
        p += m_leafSize;
        m_segment[p] = x;
        update(p);
    }
    void add(size_t p, const S& x) {
        assert(0 <= p && p < m_size);
        p += m_leafSize;
        m_segment[p] += x;
        update(p);
    }
    S prod(size_t l, size_t r) const {
        assert(0 <= l && l <= r && r <= m_size);
        auto sml = e(), smr = e();
        for(l += m_leafSize, r += m_leafSize; l < r; l >>= 1, r >>= 1) {
            if (l & 1) {
                sml = op(sml, m_segment[l++]);
            }
            if (r & 1) {
                smr = op(m_segment[--r], smr);
            }
        }
        return op(sml, smr);
    }
    inline S allProd() const {
        return m_segment[1];
    }
    template <class F>
    size_t findFirst(size_t l, const F& check) const {
        assert(0 <= l && l <= m_size);
        assert(check(e()));
        if (l == m_size) {
            return m_size;
        }
        l += m_leafSize;
        auto sm = e();
        do {
            while (l % 2 == 0) {
                l >>= 1;
            }
            if (!check(op(sm, m_segment[l]))) {
                while (l < m_leafSize) {
                    l <<= 1;
                    if (check(op(sm, m_segment[l]))) {
                        sm = op(sm, m_segment[l]);
                        l++;
                    }
                }
                return l - m_leafSize;
            }
            sm = op(sm, m_segment[l]);
            l++;
        } while ((static_cast<signed>(l) & -static_cast<signed>(l)) != static_cast<signed>(l));
        return m_size;
    }
    template <class F>
    size_t findLast(size_t r, const F& check) const {
        assert(0 <= r && r <= m_size);
        assert(check(e()));
        if (r == 0) {
            return 0;
        }
        r += m_leafSize;
        auto sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) {
                r >>= 1;
            }
            if (!check(op(m_segment[r], sm))) {
                while (r < m_leafSize) {
                    r <<= 1;
                    r++;
                    if (check(op(m_segment[r], sm))) {
                        sm = check(
                            op(m_segment[r], sm));
                        r--;
                    }
                }
                return r + 1 - m_leafSize;
            }
            sm = op(m_segment[r], sm);
        } while ((static_cast<signed>(r) & -static_cast<signed>(r)) != static_cast<signed>(r));
        return 0;
    }
    inline explicit operator bool() const noexcept {
        return (m_size != 0);
    }
    inline bool isEmpty() const noexcept {
        return (m_size == 0);
    }
    inline size_t size() const noexcept {
        return m_size;
    }
    inline size_t leafSize() const noexcept {
        return m_leafSize;
    }
private:
    size_t m_size;
    size_t m_leafSize = 1;
    inline void update(size_t p) {
        while (p >>= 1) {
            m_segment[p] = op(m_segment[2 * p], m_segment[2 * p + 1]);
        }
    }
};
/// end SegmentTree

/// start RMQ
/// @prefix: cpRMQ
/// @description: RangeMaxQuery
/// @filetemplate: false
/// @placeholder: int=type
using RMQ = SegmentTree<int, [](int a, int b){ return max(a, b); }, [](){ return numeric_limits<int>::lowest(); }>;
/// end RMQ

/// start RmQ
/// @prefix: cpRmQ
/// @description: RangeMinQuery
/// @filetemplate: false
/// @placeholder: int=type
using RmQ = SegmentTree<int, [](int a, int b){ return min(a, b); }, [](){ return numeric_limits<int>::infinity(); }>;
/// end RmQ

/// start RSQ
/// @prefix: cpRSQ
/// @description: RangeSumQuery
/// @filetemplate: false
/// @placeholder: int=type
using RSQ = SegmentTree<int, [](int a, int b){ return a + b; }, [](){ return 0; }>;
/// end RSQ
