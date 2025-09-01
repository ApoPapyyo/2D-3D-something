#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_
#include <cstddef>
#include <cassert>
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>

namespace okazawa
{
template <int M, int N, typename T = double>
class matrix;

template <int M, int N, typename T = double>
matrix<M, N, T> operator+(const matrix<M, N, T>& a, const matrix<M, N, T>& b);

template <int M, int N, typename T = double>
matrix<M, N, T> operator-(const matrix<M, N, T>& a, const matrix<M, N, T>& b);

template <int M, int N, typename T = double>
matrix<M, N, T> operator*(const T& a, const matrix<M, N, T>& b);

template <int M, int N, typename T = double>
matrix<M, N, T> operator*(const matrix<M, N, T>&a, const T& b);

template <int N, typename T = double>
T operator*(const matrix<1, N, T>& a, const matrix<N, 1, T>& b);

template <int M, int N, int P, typename T = double>
matrix<M, P, T> operator*(const matrix<M, N, T>& a, const matrix<N, P, T>& b);

template <int M, int N, typename T>
class matrix {
    static_assert(M > 0 && N > 0, "行数・列数は自然数でなければなりません。");
    T m_data[M*N];//ROWC * COLUMNC
public:
    using thistype = matrix<M, N, T>;

    matrix()
    {
        for(int i = 0; i < M * N; i++) m_data[i] = (T)0;
    }

    matrix(const thistype& v)
    {
        for(int i = 0; i < M * N; i++) m_data[i] = v.m_data[i];
    }//copy

    matrix(const T (&data)[M][N])
    {
        for(int i = 0; i < M; i++) for(int j(0); j < N; j++) m_data[i*N + j] = data[i][j];
    }

    matrix(std::initializer_list<std::initializer_list<T>> init)
    {
        for(int i = 0; i < M * N; i++) m_data[i] = (T)0;
        assert(init.size() <= M);
        for(const auto& row: init) {
            assert(row.size() <= N);
        }
        int i = 0;
        for(const auto& row: init) {
            for(const auto& v: row) {
                m_data[i++] = v;
            }
        }
    }

    explicit matrix(const T& v)
    {
        for(int i = 0; i < M * N; i++) m_data[i] = v;
    }//すべてvに設定

    thistype& operator+=(const thistype& v)
    {
        *this = *this + v;
        return *this;
    }

    thistype& operator-=(const thistype& v)
    {
        *this = *this - v;
        return *this;
    }

    template <typename U>
    thistype& operator*=(const U& v)
    {
        *this = *this * v;
        return *this;
    }

    const T *operator[](int m) const
    {
        return &m_data[m * N];
    }

    T *operator[](int m)
    {
        return &m_data[m * N];
    }

    operator std::string() const
    {
        int strmax = 0;

        for(int i = 0; i < M; i++) {
            for(int j(0); j < N; j++) {
                std::stringstream ss;
                ss << std::fixed << std::setprecision(3)<< m_data[i * N + j];
                auto s(ss.str());
                if(strmax < s.length()) strmax = s.length();
            }
        }
        std::stringstream ss;
        for(int i = 0; i < M; i++) {
            for(int j(0); j < N; j++) {
                if(!j) ss << "[ ";
                ss << std::setw(strmax) << std::fixed << std::setprecision(3) << m_data[i * N + j] << (j == N-1 ? " ]" : " ");
            }
            if(i < M-1)ss << "\n";
        }

        return ss.str();
    }

    matrix<N, M, T> transposed() const
    {
        matrix<N, M, T> ret;
        for(int i = 0; i < N; i++) {
            for(int j(0); j < M; j++) {
                ret[i][j] = m_data[j * N + i];
            }
        }
        return ret;
    }
};

template <int M, int N, typename T>
matrix<M, N, T> operator+(const matrix<M, N, T>& a, const matrix<M, N, T>& b)
{
    matrix<M, N, T> ret;
    for(int i = 0; i < M; i++) {
        for(int j(0); j < N; j++) {
            ret[i][j] = a[i][j] + b[i][j];
        }
    }
    return ret;
}

template <int M, int N, typename T>
matrix<M, N, T> operator-(const matrix<M, N, T>& a, const matrix<M, N, T>& b)
{
    matrix<M, N, T> ret;
    for(int i = 0; i < M; i++) {
        for(int j(0); j < N; j++) {
            ret[i][j] = a[i][j] - b[i][j];
        }
    }
    return ret;
}

template <int M, int N, typename T>
matrix<M, N, T> operator*(const T& a, const matrix<M, N, T>& b)
{
    matrix<M, N, T> ret;
    for(int i = 0; i < M; i++) {
        for(int j(0); j < N; j++) {
            ret[i][j] = a * b[i][j];
        }
    }
    return ret;
}

template <int M, int N, typename T>
matrix<M, N, T> operator*(const matrix<M, N, T>&a, const T& b)
{
    matrix<M, N, T> ret;
    for(int i = 0; i < M; i++) {
        for(int j(0); j < N; j++) {
            ret[i][j] = a[i][j] * b;
        }
    }
    return ret;
}

template <int N, typename T>
T operator*(const matrix<1, N, T>& a, const matrix<N, 1, T>& b)
{
    T ret = (T)0;
    for(int i = 0; i < N; i++) {
        ret += a[0][i] * b[i][0];
    }
    return ret;
}

template <int M, int N, int P, typename T>
matrix<M, P, T> operator*(const matrix<M, N, T>& a, const matrix<N, P, T>& b)
{
    matrix<M, P, T> ret;
    for(int i = 0; i < M; i++) {
        for(int j(0); j < P; j++) {
            for(int k(0); k < N; k++) {
                ret[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return ret;
}

template <int N, typename T = double>
matrix<1, N, T> row_vector(T (&data)[N])
{
    matrix<1, N, T> ret((T)0);
    for(int i = 0; i < N; i++) ret[0][i] = data[i];
    return ret;
}

template <int N, typename T = double>
matrix<1, N, T> row_vector(std::initializer_list<T> init)
{
    matrix<1, N, T> ret((T)0);
    int i = 0;
    for(const auto& val: init) {
        if(i >= N) break;
        ret[0][i++] = val;
    }
    return ret;
}

template <int N, typename T = double>
matrix<N, 1, T> column_vector(T (&data)[N])
{
    matrix<N, 1, T> ret((T)0);
    for(int i = 0; i < N; i++) ret[i][0] = data[i];
    return ret;
}

template <int N, typename T = double>
matrix<N, 1, T> column_vector(std::initializer_list<T> init)
{
    matrix<N, 1, T> ret((T)0);
    int i = 0;
    for(const auto& val: init) {
        if(i >= N) break;
        ret[i++][0] = val;
    }
    return ret;
}

}
#endif