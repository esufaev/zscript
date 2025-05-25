#pragma once
#include <stdexcept>

#include "ztoken.h"
#include <string>
#include <utility>

namespace zst::zast {
    class zlexer {
        std::string data;
        long long pos{0};

    public:
        explicit zlexer(std::string data) : data(std::move(data)) {
        }

        [[nodiscard]] char peek() const {
            return pos < data.size() ? data[pos] : '\0';
        }

        char get() { return pos < data.size() ? data[pos++] : '\0'; }

        ztoken next() {
            while (isspace(peek()))
                get();

            if (pos >= data.size()) [[unlikely]] { return {ztoken_type::End, ""}; }

            if (isalpha(peek())) {
                const long long start = pos++;

                while (isalnum(peek()) || peek() == '_')
                    get();

                const std::string substr = data.substr(start, pos - start);
                if (substr == "if") {
                    return {ztoken_type::If, substr};
                } else if (substr == "else") {
                    return {ztoken_type::Else, substr};
                } else if (substr == "while") {
                    return {ztoken_type::While, substr};
                } else if (substr == "for") {
                    return {ztoken_type::For, substr};
                } else if (substr == "func") {
                    return {ztoken_type::Function, substr};
                } else if (substr == "any_of") {
                    return {ztoken_type::Anyof, substr};
                } else if (substr == "all_of") {
                    return {ztoken_type::Allof, substr};
                } else if (substr == "print") {
                    return {ztoken_type::Print, substr};
                } else {
                    return {ztoken_type::Identifier, substr};
                }
            }

            if (isdigit(peek()) || peek() == '.') {
                const long long start = pos++;
                while (isdouble(peek()))
                    get();

                return {ztoken_type::Number, zutils::zmatrix(std::stod(data.substr(start, pos - start)))};
            }

            const char c = get();
            switch (c) {
                case ';':
                    return {ztoken_type::Semicolon, ";"};
                case '[':
                    return {ztoken_type::Rmatrix, construct_matrix()};
                case '{':
                    return {ztoken_type::LBrace, "{"};
                case '}':
                    return {ztoken_type::RBrace, "}"};
                case '(':
                    return {ztoken_type::LParen, "("};
                case ')':
                    return {ztoken_type::RParen, ")"};
                case '+':
                    return {ztoken_type::Plus, "+"};
                case '-':
                    return {ztoken_type::Minus, "-"};
                case '*':
                    return {ztoken_type::Mul, "*"};
                case '/':
                    return {ztoken_type::Div, "/"};
                case '@':
                    return {ztoken_type::Matmul, "@"};
                case '=':
                    if (peek() == '=') {
                        get();
                        return {ztoken_type::Equal, "=="};
                    }
                    return {ztoken_type::Assign, "="};
                case '!':
                    if (peek() == '=') {
                        get();
                        return {ztoken_type::Notequal, "!="};
                    }
                    return {ztoken_type::Not, "!"};
                case '<':
                    if (peek() == '=') {
                        get();
                        return {ztoken_type::Lessequal, "<="};
                    }
                    return {ztoken_type::Less, "<"};
                case '>':
                    if (peek() == '=') {
                        get();
                        return {ztoken_type::Greaterequal, ">="};
                    }
                    return {ztoken_type::Greater, ">"};
                default:
                    return {ztoken_type::End, data.substr(pos, data.size() - pos + 1)};
            }
        }

    private:
        // TODO: Поправить ошибку с пробелами и точками
        zutils::zmatrix construct_matrix() {
            unsigned long long rows = 0;
            unsigned long long cols = 0;
            std::vector<double> matrix_data{};

            bool run = true;
            while (run) {
                while (isspace(peek()))
                    get();

                char c = get();
                if (c == ';' || c == ']') {
                    rows++;

                    if (cols == 0) {
                        cols = matrix_data.size();
                    } else if (matrix_data.size() - cols * rows != 0) {
                        throw std::runtime_error("Inappropriate dimensions");
                    }

                    run = c != ']';
                } else if (isdouble(c)) {
                    const long long subpos = pos - 1;
                    c = peek();
                    while (c != ',' && c != ';' && c != ']' && c != '\0') {
                        get();
                        c = peek();
                    }

                    matrix_data.push_back(std::stod(data.substr(subpos, pos - subpos)));
                } else if (c != ',') {
                    throw std::runtime_error("Error when parsing matrix");
                }
            }

            return {matrix_data, rows, cols};
        }

        [[nodiscard]] static bool isdouble(char c) { return isdigit(c) || c == '.' || c == '-'; }
    };
}
