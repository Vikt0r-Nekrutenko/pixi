#include "matrix.hpp"

pixi::math::matrix::matrix(const pixi::math::dword &w, const pixi::math::dword &h, const bool &is_rand, const float &min, const float &max)
    : _w(w),
      _h(h)
{
    _array = (vector<> *)malloc(_h * sizeof (vector<>));
    for (dword i = 0; i < _h; ++i) {
        _array[i] = vector<>(_w, is_rand, min, max);
    }
}

pixi::math::matrix::~matrix()
{
    free(_array);
}

pixi::math::matrix &pixi::math::matrix::operator =(const pixi::math::matrix &mtrx)
{
    free(_array);
    _array = (vector<> *)malloc(mtrx._h * sizeof (vector<>));

    _h = mtrx._h;
    _w = mtrx._w;

    for(dword i = 0; i < _h; ++i) {
        _array[i] = vector<>(_w);
        for (dword j = 0; j < _w; ++j) {
            _array[i][j] = mtrx._array[i][j];
        }
    }
    return *this;
}

void pixi::math::matrix::print() const
{
    for (dword i = 0; i < _h; ++i) {
        _array[i].print();
    }
}

pixi::math::dword pixi::math::matrix::weighted_sum(const vector<> &vec) const
{
    assert(_w == vec.size());
    float max = 0.f;
    dword indx = 0u;
    for (dword i = 0; i < _h; ++i) {
        float d = _array[i].dot(vec);
        if (d > max) {
            max = d;
            indx = i;
        }
    }
    return indx;
}

pixi::math::matrix pixi::math::matrix::random(const float &min, const float &max)
{
    for (dword i = 0; i < _h; ++i) {
        _array[i].random(min, max);
    }
    return *this;
}
