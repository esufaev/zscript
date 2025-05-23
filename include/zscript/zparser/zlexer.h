#pragma once
#include "token.h"
#include <string>
#include <utility>

namespace zst::ast {

class zlexer {
  std::string data;
  long long pos{0};

public:
  explicit zlexer(std::string data) : data(std::move(data)) {}

  [[nodiscard]] char peek() const {
    return pos < data.size() ? data[pos] : '\0';
  }

  char get() { return pos < data.size() ? data[pos++] : '\0'; }

  token next() {
    while (isspace(peek()))
      get();

    if (isalpha(peek())) {
      const long long start = pos++; // Так на 1 сравнение меньше

      while (isalnum(peek()))
        get();

      const std::string substr = data.substr(start, pos - start);
      if (substr == "if") {
        return {token_type::If, substr};
      } else if (substr == "else") {
        return {token_type::Else, substr};
      } else if (substr == "while") {
        return {token_type::While, substr};
      } else if (substr == "for") {
        return {token_type::For, substr};
      } else if (substr == "func") {
        return {token_type::Function, substr};
      } else {
        return {token_type::Identifier, substr};
      }
    }

    if (isdigit(peek())) {
      const long long start = pos++;
      while (isdigit(peek()))
        get();

      return {token_type::Number, data.substr(start, pos - start)};
    }

    const char c = get();
    switch (c) {
    case ';':
      return {token_type::Semicolon, ";"};
    case '[':
      return {token_type::LBracket, "["};
    case ']':
      return {token_type::RBracket, "]"};
    case '{':
      return {token_type::LBrace, "{"};
    case '}':
      return {token_type::RBrace, "}"};
    case '(':
      return {token_type::LParen, "("};
    case ')':
      return {token_type::RParen, ")"};
    case '+':
      return {token_type::Plus, "+"};
    case '-':
      return {token_type::Minus, "-"};
    case '*':
      return {token_type::Mul, "*"};
    case '/':
      return {token_type::Div, "/"};
    case '@':
      return {token_type::Matmul, "@"};
    case '=':
      if (peek() == '=') {
        get();
        return {token_type::Equal, "=="};
      }
      return {token_type::Assign, "="};
    case '!':
      if (peek() == '=') {
        get();
        return {token_type::Notequal, "!="};
      }
      return {token_type::Not, "!"};
    case '<':
      if (peek() == '=') {
        get();
        return {token_type::Lessequal, "<="};
      }
      return {token_type::Less, "<"};
    case '>':
      if (peek() == '=') {
        get();
        return {token_type::Greaterequal, ">="};
      }
      return {token_type::Greater, ">"};
    default:
      return {token_type::End, data.substr(pos, data.size() - pos + 1)};
    }
}
};
}