#pragma once
#include <vector>

namespace zst::zutils {
    class zmatrix {
        std::vector<double> data{};
        unsigned long long rows{};
        unsigned long long cols{};

    public:
        zmatrix() : zmatrix({0.0}, 1, 1) {}

        zmatrix(const std::vector<double> &data, const unsigned long long rows,
                const unsigned long long cols) : data(data), rows(rows), cols(cols) {
        }
    };
} // namespace zst::zutils
