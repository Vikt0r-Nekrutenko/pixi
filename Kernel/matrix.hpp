#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vector.hpp"

namespace pixi { namespace math {

template <dword SIZE = 16> class matrix
{
public:
    matrix(const dword &w, const dword &h, const bool &is_rand = false, const float &min = 0.f, const float &max = 1.f)
        : _w(w),
          _h(h)
    {
        assert(_h <= SIZE);
        for (dword i = 0; i < _h; ++i) {
            _array[i] = vector<>(_w, is_rand, min, max);
        }
    }
    inline vector<> &operator [](const dword &indx) {
        assert(indx < _h);
        return _array[indx];
    }
    inline const vector<> &operator [](const dword &indx) const {
        assert(indx < _h);
        return _array[indx];
    }
    inline dword w() const
    {
        return _w;
    }
    inline dword h() const
    {
        return _h;
    }
    void print() const
    {
        for (dword i = 0; i < _h; ++i) {
            _array[i].print();
        }
    }
    vector<> dot(const vector<> &vec) const
    {
        assert(_w == vec.size());
        vector<> tmp(_h);
        for (dword i = 0; i < _h; ++i) {
            tmp[i] = _array[i].dot(vec);
        }
        return tmp;
    }
    matrix<> random(const float &min, const float &max)
    {
        for (dword i = 0; i < _h; ++i) {
            _array[i].random(min, max);
        }
    }
private:
    vector<> _array[SIZE];
    dword _w = 0u, _h = 0u;
};

}}

#endif // MATRIX_HPP
