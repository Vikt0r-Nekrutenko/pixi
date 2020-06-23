#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vector.hpp"
#include <tuple>

namespace pixi { namespace math {

class KERNEL_EXPORT matrix
{
public:
    matrix(const dword &w, const dword &h, const bool &is_rand = false, const float &min = 0.f, const float &max = 1.f);
    ~matrix();
    inline vector<> &operator [](const dword &indx)
    {
        assert(indx < _h);
        return _array[indx];
    }
    inline const vector<> &operator [](const dword &indx) const
    {
        assert(indx < _h);
        return  _array[indx];
    }
    inline dword w() const
    {
        return _w;
    }
    inline dword h() const
    {
        return _h;
    }
    void print() const;
    math::dword weighted_sum(const vector<> &vec) const;
    matrix &operator =(const matrix &mtrx);
    matrix random(const float &min, const float &max);
private:
    vector<> *_array = nullptr;
    dword _w = 0u, _h = 0u;
};

}}

#endif // MATRIX_HPP
