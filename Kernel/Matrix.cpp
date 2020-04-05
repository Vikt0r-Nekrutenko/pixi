#include "Matrix.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

using namespace pixi::math;

Matrix::Matrix(const int w, const int h)
    : m_w { w }, m_h { h }
{
    m_line.resize(w * h, 0);
}

Matrix::Matrix(const matrix mtrx)
    : m_h { int(mtrx.size()) }
{
    for (size_t i = 0; i < mtrx.size(); i++) {
        if (mtrx.at(i).size() > size_t(m_w)) {
            m_w = mtrx.at(i).size();
        }
    }

    m_line.resize(m_w * m_h, 0);

    for (size_t y = 0; y < mtrx.size(); y++) {
        for (size_t x = 0; x < mtrx.at(y).size(); x++) {
            (*this)(x, y) = mtrx.at(y).at(x);
        }
    }
}

Matrix &Matrix::random(const float min, const float max)
{
    for (int i = m_line.size() - 1; i >= 0; i--) {
        m_line[i] = frand(min, max);
    }
    return *this;
}

Matrix Matrix::exp() const
{
    Matrix tmp(m_w, m_h);
    for (int i = m_line.size() - 1; i >= 0; i--) {
        tmp(i) = expf(m_line[i]);
    }
    return tmp;
}

Matrix Matrix::transpose() const
{
    Matrix tmp(m_h, m_w);
    for (int y = 0; y < m_h; y++) {
        for (int x = 0; x < m_w; x++) {
            tmp(y, x) = (*this)(x,y);
        }
    }
    return tmp;
}

Matrix Matrix::dot(const Matrix mtrx) const
{
    assert(m_w == mtrx.m_h);

    Matrix tmp(mtrx.m_w, m_h);
    for  (int y = 0; y < m_h; y++) {
        for (int x = 0; x < mtrx.m_w; x++) {
            for (int k = 0; k < m_w; k++) {
                tmp(x, y) += (*this)(k, y) * mtrx(x, k);
            }
        }
    }
    return tmp;
}

Matrix Matrix::operator -() const
{
    Matrix tmp(m_w, m_h);
    for (int i = m_line.size() - 1; i >= 0; i--) {
        tmp(i) = -m_line[i];
    }
    return tmp;
}

Matrix Matrix::operator +(const float value) const
{
    Matrix tmp(m_w, m_h);
    for (int i = m_line.size() - 1; i >= 0; i--) {
        tmp(i) = m_line[i] + value;
    }
    return tmp;
}

Matrix Matrix::operator *(const float value) const
{
    Matrix tmp(m_w, m_h);
    for (int i = m_line.size() - 1; i >= 0; i--) {
        tmp(i) = m_line[i] * value;
    }
    return tmp;
}

Matrix Matrix::operator /(const float value) const
{
    Matrix tmp(m_w, m_h);
    for (int i = m_line.size() - 1; i >= 0; i--) {
        tmp(i) = m_line[i] / value;
    }
    return tmp;
}

Matrix Matrix::operator -(const Matrix mtrx) const
{
    assert(m_line.size() == mtrx.m_line.size());

    Matrix tmp(m_w, m_h);
    for (int i = m_line.size() - 1; i >= 0; i--) {
        tmp(i) = m_line[i] - mtrx(i);
    }
    return tmp;
}

Matrix Matrix::operator +(const Matrix mtrx) const
{
    assert(m_line.size() == mtrx.m_line.size());

    Matrix tmp(m_w, m_h);
    for (int i = m_line.size() - 1; i >= 0; i--) {
        tmp(i) = m_line[i] + mtrx(i);
    }
    return tmp;
}

Matrix Matrix::operator *(const Matrix mtrx) const
{
    assert(m_w == mtrx.m_w);
    assert(m_h == mtrx.m_h || mtrx.m_h == 1);

    Matrix tmp(fmax(m_w, mtrx.m_w), fmax(m_h, mtrx.m_h));

    for (int i = tmp.m_line.size() - 1; i >= 0; i--) {
        int inx1 = i % m_line.size();
        int inx2 = i % mtrx.m_line.size();
        tmp(i) = m_line[inx1] * mtrx(inx2);
    }
    return tmp;
}

Matrix &Matrix::operator +=(const Matrix mtrx)
{
    assert(m_line.size() == mtrx.m_line.size());

    for (int i = m_line.size() - 1; i >= 0; i--) {
        m_line[i] += mtrx(i);
    }
    return *this;
}

Matrix &Matrix::operator *=(const Matrix mtrx)
{
    Matrix tmp = (*this) * mtrx;
    (*this) = tmp;

    return *this;
}

void Matrix::print() const
{
    for (size_t i = 0; i < m_line.size(); i++) {
        std::cout << m_line[i] << " ";
        if (i % m_w == m_w - 1) {
            std::cout << std::endl;
        }
    }
}

Matrix KERNEL_EXPORT pixi::math::operator /(const float value, const Matrix mtrx)
{
    Matrix tmp(mtrx.m_w, mtrx.m_h);
    for (int i = mtrx.m_line.size() - 1; i >= 0; i--) {
        tmp(i) = value / mtrx(i);
    }
    return tmp;
}

Matrix KERNEL_EXPORT pixi::math::operator *(const float value, const Matrix mtrx) {
    Matrix tmp(mtrx.m_w, mtrx.m_h);
    for (int i = mtrx.m_line.size() - 1; i >= 0; i--) {
        tmp(i) = value * mtrx(i);
    }
    return tmp;
}

Matrix KERNEL_EXPORT pixi::math::operator -(const float value, const Matrix mtrx) {
    Matrix tmp(mtrx.m_w, mtrx.m_h);
    for (int i = mtrx.m_line.size() - 1; i >= 0; i--) {
        tmp(i) = value - mtrx(i);
    }
    return tmp;
}

Matrix KERNEL_EXPORT pixi::math::operator +(const float value, const Matrix mtrx) {
    Matrix tmp(mtrx.m_w, mtrx.m_h);
    for (int i = mtrx.m_line.size() - 1; i >= 0; i--) {
        tmp(i) = value + mtrx(i);
    }
    return tmp;
}

float KERNEL_EXPORT pixi::math::frand(const float min, const float max) {
    return min + (float(rand()) / RAND_MAX) * (max - min);
}

