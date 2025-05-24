#pragma once

#include <string>

namespace zst
{
    enum class ztoken_type
    {
        Identifier,
        Number,
        If,
        Else,
        While,
        For,
        LParen,
        RParen,
        LBracket,
        RBracket,
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
        Print,
        End
    };

    struct ztoken
    {
        ztoken_type type;
        std::string text;
    };

} // namespace zst
