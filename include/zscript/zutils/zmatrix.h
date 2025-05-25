// ReSharper disable CppDFAConstantConditions
#pragma once
#include <vector>
#include <array>
#include <string>
#include <stdexcept>
#include <sstream>
#include <ios>

namespace zst::zutils {
    class zmatrix {
        std::vector<double> data;
        unsigned long long rows, cols;

    public:
        zmatrix(const std::vector<double> &d, unsigned long long r, unsigned long long c)
            : data(d), rows(r), cols(c) {
        }

        zmatrix(unsigned long long r, unsigned long long c)
            : rows(r), cols(c) {
            data.reserve(r * c);
        }

        zmatrix() : data{0.0}, rows(1), cols(1) {
        }

        zmatrix(double v) : data{v}, rows(1), cols(1) {
        }

        [[nodiscard]] std::array<unsigned long long, 2> dimensions() const {
            return {rows, cols};
        }

        // доступ по индексу
        double &operator()(unsigned long long i, unsigned long long j) {
            return data[i * cols + j];
        }

        double operator()(unsigned long long i, unsigned long long j) const {
            return data[i * cols + j];
        }

        [[nodiscard]] zmatrix any_of() const {
            bool res = false;
            for (int i = 0; i < data.size() && !res; i++) {
                res = res || (data[i] >= 1);
            }

            return {static_cast<double>(res)};
        }

        [[nodiscard]] zmatrix all_of() const {
            bool res = true;
            for (int i = 0; i < data.size() && res; i++) {
                res = res && (data[i] >= 1);
            }

            return {static_cast<double>(res)};
        }

        [[nodiscard]] zmatrix operator==(const zmatrix &B) const {
            zmatrix C(rows, cols);
            for (std::size_t i = 0; i < data.size(); ++i)
                C.data[i] = static_cast<double>(data[i] == B.data[i]);
            return C;
        }

        [[nodiscard]] zmatrix operator!=(const zmatrix &B) const {
            zmatrix C(rows, cols);
            for (std::size_t i = 0; i < data.size(); ++i)
                C.data[i] = static_cast<double>(data[i] != B.data[i]);
            return C;
        }

        [[nodiscard]] zmatrix operator<=(const zmatrix &B) const {
            zmatrix C(rows, cols);
            for (std::size_t i = 0; i < data.size(); ++i)
                C.data[i] = static_cast<double>(data[i] <= B.data[i]);
            return C;
        }

        [[nodiscard]] zmatrix operator>=(const zmatrix &B) const {
            zmatrix C(rows, cols);
            for (std::size_t i = 0; i < data.size(); ++i)
                C.data[i] = static_cast<double>(data[i] >= B.data[i]);
            return C;
        }

        [[nodiscard]] zmatrix operator<(const zmatrix &B) const {
            zmatrix C(rows, cols);
            for (std::size_t i = 0; i < data.size(); ++i)
                C.data[i] = static_cast<double>(data[i] < B.data[i]);
            return C;
        }

        [[nodiscard]] zmatrix operator>(const zmatrix &B) const {
            zmatrix C(rows, cols);
            for (std::size_t i = 0; i < data.size(); ++i)
                C.data[i] = static_cast<double>(data[i] > B.data[i]);
            return C;
        }

        [[nodiscard]] zmatrix operator+(const zmatrix &B) const {
            zmatrix C(rows, cols);
            for (std::size_t i = 0; i < data.size(); ++i)
                C.data[i] = data[i] + B.data[i];
            return C;
        }

        [[nodiscard]] zmatrix operator-(const zmatrix &B) const {
            zmatrix C(rows, cols);
            for (std::size_t i = 0; i < data.size(); ++i)
                C.data[i] = data[i] - B.data[i];
            return C;
        }

        [[nodiscard]] zmatrix operator*(const zmatrix &B) const {
            zmatrix C(rows, cols);
            for (std::size_t i = 0; i < data.size(); ++i)
                C.data[i] = data[i] * B.data[i];
            return C;
        }

        [[nodiscard]] zmatrix operator/(const zmatrix &B) const {
            zmatrix C(rows, cols);
            for (std::size_t i = 0; i < data.size(); ++i)
                C.data[i] = data[i] / B.data[i];
            return C;
        }

        // скалярные операции
        [[nodiscard]] zmatrix operator+(double s) const {
            zmatrix C(data, rows, cols);
            for (auto &x: C.data) x += s;
            return C;
        }

        [[nodiscard]] zmatrix operator-(double s) const {
            zmatrix C(data, rows, cols);
            for (auto &x: C.data) x -= s;
            return C;
        }

        [[nodiscard]] zmatrix operator*(double s) const {
            zmatrix C(data, rows, cols);
            for (auto &x: C.data) x *= s;
            return C;
        }

        [[nodiscard]] zmatrix operator/(double s) const {
            zmatrix C(data, rows, cols);
            for (auto &x: C.data) x /= s;
            return C;
        }

        [[nodiscard]] zmatrix dot(const zmatrix &B) const {
            if (cols != B.rows)
                throw std::invalid_argument("Incompatible dimensions for dot product");
            zmatrix result(std::vector<double>(rows * B.cols, 0.0), rows, B.cols);
            for (unsigned long long i = 0; i < rows; ++i) {
                for (unsigned long long k = 0; k < cols; ++k) {
                    double val = operator()(i, k);
                    for (unsigned long long j = 0; j < B.cols; ++j) {
                        result(i, j) += val * B(k, j);
                    }
                }
            }
            return result;
        }

        [[nodiscard]] std::string print() const {
            std::ostringstream out;
            out.precision(6);
            out << std::fixed;
            for (unsigned long long i = 0; i < rows; ++i) {
                out << "[ ";
                for (unsigned long long j = 0; j < cols; ++j) {
                    out << (*this)(i, j);
                    if (j + 1 < cols) out << ", ";
                }
                out << " ]\n";
            }
            return out.str();
        }
    };
} // namespace zst::zutils
