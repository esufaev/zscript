#pragma once
#include "ztoken.h"
#include <string>
#include <utility>

namespace zst::zast
{
  class zlexer
  {
    std::string data;
    long long pos{0};

  public:
    explicit zlexer(std::string data) : data(std::move(data)) {}

    [[nodiscard]] char peek() const
    {
      return pos < data.size() ? data[pos] : '\0';
    }

    char get() { return pos < data.size() ? data[pos++] : '\0'; }

    ztoken next()
    {
      while (isspace(peek()))
        get();

      if (pos >= data.size()) { return {ztoken_type::End, ""}; } // Блять!

      if (isalpha(peek()))
      {
        const long long start = pos++; // Так на 1 сравнение меньше

        while (isalnum(peek()))
          get();

        const std::string substr = data.substr(start, pos - start);
        if (substr == "if")
        {
          return {ztoken_type::If, substr};
        }
        else if (substr == "else")
        {
          return {ztoken_type::Else, substr};
        }
        else if (substr == "while")
        {
          return {ztoken_type::While, substr};
        }
        else if (substr == "for")
        {
          return {ztoken_type::For, substr};
        }
        else if (substr == "func")
        {
          return {ztoken_type::Function, substr};
        }
        else
        {
          return {ztoken_type::Identifier, substr};
        }
      }

      if (isdigit(peek()))
      {
        const long long start = pos++;
        while (isdigit(peek()))
          get();

        return {ztoken_type::Number, data.substr(start, pos - start)};
      }

      const char c = get();
      switch (c)
      {
      case ';':
        return {ztoken_type::Semicolon, ";"};
      case '[':
        return {ztoken_type::LBracket, "["};
      case ']':
        return {ztoken_type::RBracket, "]"};
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
        if (peek() == '=')
        {
          get();
          return {ztoken_type::Equal, "=="};
        }
        return {ztoken_type::Assign, "="};
      case '!':
        if (peek() == '=')
        {
          get();
          return {ztoken_type::Notequal, "!="};
        }
        return {ztoken_type::Not, "!"};
      case '<':
        if (peek() == '=')
        {
          get();
          return {ztoken_type::Lessequal, "<="};
        }
        return {ztoken_type::Less, "<"};
      case '>':
        if (peek() == '=')
        {
          get();
          return {ztoken_type::Greaterequal, ">="};
        }
        return {ztoken_type::Greater, ">"};
      default:
        throw std::runtime_error(std::string("Unknown character: ") + c);
      }
    }
  };
}