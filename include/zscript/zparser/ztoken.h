#pragma once

#include <string>
#include <variant>

#include "zscript/zutils/zmatrix.h"

namespace zst
{
    enum class ztoken_type
    {
        Identifier,
        Rmatrix,
        Number,
        If,
        Else,
        While,
        For,
        LParen,
        RParen,
        LBrace,
        RBrace,
        Semicolon,
        Assign,
        Plus,
        Minus,
        Mul,
        Matmul,
        Div,
        Less,
        Greater,
        Lessequal,
        Greaterequal,
        Equal,
        Not,
        Notequal,
        Function,
        Anyof,
        Allof,
        Print,
        End
    };

    struct ztoken
    {
        ztoken_type type;
        std::variant<std::string, zutils::zmatrix> data;

        std::string text() {
            return std::get<std::string>(data);
        }

        zutils::zmatrix zmatrix() {
            return std::get<zutils::zmatrix>(data);
        }
    };
} // namespace zst
