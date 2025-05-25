#pragma once
#include <vector>
#include <array>
#include <string>
#include <stdexcept>

namespace zst::zutils
{
    class zmatrix
    {
        std::vector<double> data;
        unsigned long long rows, cols;

    public:
        zmatrix(const std::vector<double> &d, unsigned long long r, unsigned long long c)
            : data(d), rows(r), cols(c)
        {
        }

        zmatrix(unsigned long long r, unsigned long long c)
            : rows(r), cols(c)
        {
            data.resize(r * c);
        }

        zmatrix() : data{0.0}, rows(1), cols(1)
        {
        }

        zmatrix(double v) : data{v}, rows(1), cols(1)
        {
        }

        [[nodiscard]] std::array<unsigned long long, 2> dimensions() const
        {
            return {rows, cols};
        }

        // доступ по индексу
        double &operator()(unsigned long long i, unsigned long long j)
        {
            return data[i * cols + j];
        }

        double operator()(unsigned long long i, unsigned long long j) const
        {
            return data[i * cols + j];
        }

        [[nodiscard]] zmatrix any_of() const
        {
            bool res = false;
            for (int i = 0; i < data.size() && !res; i++)
            {
                res = res || (data[i] >= 1);
            }

            return {static_cast<double>(res)};
        }

        [[nodiscard]] zmatrix all_of() const
        {
            bool res = true;
            for (int i = 0; i < data.size() && res; i++)
            {
                res = res && (data[i] >= 1);
            }

            return {static_cast<double>(res)};
        }

        // Матричные операции
        [[nodiscard]] zmatrix operator==(const zmatrix &B) const
        {
            return bin_matrix(B, std::equal_to<double>{});
        }

        [[nodiscard]] zmatrix operator!=(const zmatrix &B) const
        {
            return bin_matrix(B, std::not_equal_to<double>{});
        }

        [[nodiscard]] zmatrix operator<=(const zmatrix &B) const
        {
            return bin_matrix(B, std::less_equal<double>{});
        }

        [[nodiscard]] zmatrix operator>=(const zmatrix &B) const
        {
            return bin_matrix(B, std::greater_equal<double>{});
        }

        [[nodiscard]] zmatrix operator<(const zmatrix &B) const
        {
            return bin_matrix(B, std::less<double>{});
        }

        [[nodiscard]] zmatrix operator>(const zmatrix &B) const
        {
            return bin_matrix(B, std::greater<double>{});
        }

        [[nodiscard]] zmatrix operator+(const zmatrix &B) const
        {
            return bin_matrix(B, std::plus<double>{});
        }

        [[nodiscard]] zmatrix operator-(const zmatrix &B) const
        {
            return bin_matrix(B, std::minus<double>{});
        }

        [[nodiscard]] zmatrix operator*(const zmatrix &B) const
        {
            return bin_matrix(B, std::multiplies<double>{});
        }

        [[nodiscard]] zmatrix operator/(const zmatrix &B) const
        {
            return bin_matrix(B, std::divides<double>{});
        }

        [[nodiscard]] zmatrix dot(const zmatrix &B) const
        {
            if (cols != B.rows)
                throw std::invalid_argument("Incompatible dimensions for dot product");
            zmatrix result(std::vector<double>(rows * B.cols, 0.0), rows, B.cols);
            for (unsigned long long i = 0; i < rows; ++i)
            {
                for (unsigned long long k = 0; k < cols; ++k)
                {
                    double val = operator()(i, k);
                    for (unsigned long long j = 0; j < B.cols; ++j)
                    {
                        result(i, j) += val * B(k, j);
                    }
                }
            }

            return result;
        }

        void print() const
        {
            for (unsigned long long i = 0; i < cols; ++i)
            {
                printf(" [ ");
                for (unsigned long long j = 0; j < rows; ++j)
                {
                    printf("%.2f ", operator()(i, j));
                    if (j + 1 < cols)
                        printf(", ");
                }
                printf("]\n");
            }
        }

        operator bool() const { return all_of()(0, 0); }

    private:
        template <typename Op>
        [[nodiscard]] zmatrix bin_matrix(const zmatrix &B, Op op) const
        {
            zmatrix C(rows, cols);
            if (dimensions() == B.dimensions())
            {
                for (std::size_t i = 0; i < data.size(); ++i)
                    C.data[i] = op(data[i], B.data[i]);
                return C;
            }
            else if (B.dimensions()[0] == 1 && B.dimensions()[1] == 1)
            {
                for (std::size_t i = 0; i < data.size(); ++i)
                    C.data[i] = op(data[i], B(0, 0));
                return C;
            }
            else
            {
                throw std::invalid_argument("Incompatible dimensions for elementwise operation");
            }
        }
    };
} // namespace zst::zutils
