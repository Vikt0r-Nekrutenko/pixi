#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "kernel_global.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>

namespace pixi { namespace math {

using dword = unsigned;

template <dword SIZE = 4096> class KERNEL_EXPORT vector
{
public:
    vector()
        : _size(SIZE)
    {
        for (dword i = 0; i < _size; ++i) {
            _array[i] = 0.f;
        }
    }
    vector(const dword &size, const bool &is_rand = false, const float &min = 0.f, const float &max = 1.f)
        : _size(size)
    {
        assert(_size <= SIZE);

        if (is_rand) {
            random(min, max);
        }
    }
    vector(const dword &size, const float &def_v)
        : _size(size)
    {
        assert(_size <= SIZE);

        for (dword i = 0; i < _size; ++i) {
            _array[i] = def_v;
        }
    }
    inline dword size() const
    {
        return _size;
    }
    inline float &operator [](const dword &indx)
    {
        assert(indx < _size);
        return _array[indx];
    }
    inline const float &operator [](const dword &indx) const
    {
        assert(indx < _size);
        return _array[indx];
    }
    void print() const
    {
        for (dword i = 0; i < _size; ++i) {
            printf("%.2f ", _array[i]);
        }
        printf("\n");
    }
    float dot(const vector &vec) const
    {
        assert(_size == vec._size);
        float result = 0.f;
        for (dword i = 0; i < _size; ++i) {
            result += _array[i] * vec._array[i];
        }
        return result;
    }
    dword max() const
    {
        float max = 0.f;
        dword indx = 0u;
        for (dword i = 0; i < _size; ++i) {
            if (_array[i] > max) {
                max = _array[i];
                indx = i;
            }
        }
        return indx;
    }
    vector &norm()
    {
        float sqr_sum = 0.f;
        for (dword i = 0; i < _size; ++i) {
            sqr_sum += powf(_array[i], 2.f);
        }

        float sqrt_sum = sqrtf(sqr_sum);
        for (dword i = 0; i < _size; ++i) {
            _array[i] /= sqrt_sum;
        }
        return *this;
    }
    vector &random(const float &min, const float &max)
    {
        for (dword i = 0; i < _size; ++i) {
            _array[i] = min + (float(rand()) / RAND_MAX) * (max - min);
        }
        return *this;
    }
    vector operator -(const vector &vec) const
    {
        assert(_size == vec._size);
        vector tmp(_size);
        for (dword i = 0; i < _size; ++i) {
            tmp._array[i] = _array[i] - vec._array[i];
        }
        return tmp;
    }
    vector operator +(const vector &vec) const
    {
        assert(_size == vec._size);
        vector tmp(_size);
        for (dword i = 0; i < _size; ++i) {
            tmp._array[i] = _array[i] + vec._array[i];
        }
        return tmp;
    }
    vector operator *(const float &value) const
    {
        vector tmp(_size);
        for (dword i = 0; i < _size; ++i) {
            tmp._array[i] = _array[i] * value;
        }
        return tmp;
    }
private:
    float _array[SIZE];
    dword _size = 0u;
};

}}

#endif // VECTOR_HPP
