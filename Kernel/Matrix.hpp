#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Kernel_global.h"
#include <vector>
#include <cstdlib>

namespace pixi {
    namespace math {
        using matrix = std::vector<std::vector<float>>;

        float KERNEL_EXPORT frand(const float min, const float max);

        class KERNEL_EXPORT Matrix
        {
        public:
            Matrix(const int w, const int h);
            Matrix(const matrix mtrx);
            Matrix &random(const float min, const float max);
            Matrix exp() const;
            Matrix transpose() const;
            Matrix dot(const Matrix mtrx) const;
            void print() const;

            inline float &operator ()(const int x, const int y) {
                return m_line[m_w * y + x];
            }
            inline float operator ()(const int x, const int y) const {
                return m_line[m_w * y + x];
            }

            inline float &operator ()(const int indx) {
                return m_line[indx];
            }
            inline float operator ()(const int indx) const {
                return m_line[indx];
            }

            Matrix operator -() const;
            Matrix operator +(const float value) const;
            Matrix operator *(const float value) const;
            Matrix operator /(const float value) const;

            Matrix operator -(const Matrix mtrx) const;
            Matrix operator +(const Matrix mtrx) const;
            Matrix operator *(const Matrix mtrx) const;

            Matrix &operator +=(const Matrix mtrx);
            Matrix &operator *=(const Matrix mtrx);

            friend Matrix KERNEL_EXPORT operator /(const float value, const Matrix mtrx);
            friend Matrix KERNEL_EXPORT operator *(const float value, const Matrix mtrx);
            friend Matrix KERNEL_EXPORT operator -(const float value, const Matrix mtrx);
            friend Matrix KERNEL_EXPORT operator +(const float value, const Matrix mtrx);
        private:
            std::vector<float> m_line;
            int m_w = 0, m_h = 0;
        };

        Matrix KERNEL_EXPORT operator /(const float value, const Matrix mtrx);
        Matrix KERNEL_EXPORT operator *(const float value, const Matrix mtrx);
        Matrix KERNEL_EXPORT operator -(const float value, const Matrix mtrx);
        Matrix KERNEL_EXPORT operator +(const float value, const Matrix mtrx);

    }
}


#endif // MATRIX_HPP
