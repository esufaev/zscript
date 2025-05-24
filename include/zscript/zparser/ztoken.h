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
        // LBracket,
        // RBracket,
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
        Print,
        End
    };

    struct ztoken
    {
        ztoken_type type;
        std::variant<std::string, zutils::zmatrix> data;
    };
} // namespace zst
