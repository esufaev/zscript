#pragma once
#include <string>

namespace zst::ast {
enum class token_type {
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
    End
};

struct token
{
    token_type type;
    std::string text;
};

} // namespace zlexer
