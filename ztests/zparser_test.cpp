#include <catch2/catch_test_macros.hpp>
#include "zscript/zparser/zparser.h"

#include "iostream"

using namespace zst;

TEST_CASE("EXPRESSION AND ASSIGNE TEST")
{
    std::string code = R"(
      y = 12;
      x = 123;
      z = x + 123 - y - x - x - 12;
    )";

    zst::zlexer lexer(code);
    zst::zparser parser(lexer);
    auto tree = parser.parse();
    tree->eval();

    REQUIRE(zcontext["x"] == 123);
    REQUIRE(zcontext["y"] == 12);
    REQUIRE(zcontext["z"] == -24);
}

TEST_CASE("IF TEST")
{
    std::string code = R"(
        x = 1111;
        y = 1;
        if(x < 123)
        {
            y = 2;
        }
        else
        {
            y = 3;
        }

        x = 1;
        z = 1;
        if(x < 123)
        {
            z = 2;
        }
        else
        {
            z = 3;
        }
    )";

    zst::zlexer lexer(code);
    zst::zparser parser(lexer);
    auto tree = parser.parse();
    tree->eval();
    
    std::cout << zcontext["x"] << std::endl;
    std::cout << zcontext["y"] << std::endl;

    REQUIRE(zcontext["y"] == 3);
    REQUIRE(zcontext["z"] == 2);
}

TEST_CASE("WHILE TEST")
{
    std::string code = R"(
        x = 0;
        while(x < 10)
        {
            x = x + 1;
        }
    )";

    zst::zlexer lexer(code);
    zst::zparser parser(lexer);
    auto tree = parser.parse();
    tree->eval();

    std::cout << zcontext["x"] << std::endl;

    REQUIRE(zcontext["x"] == 10);
}

TEST_CASE("FOR TEST")
{
    std::string code = R"(
        x = 0;
        for(i = 0; i < 10; i = i + 1)
        {
            x = x + 1;
        }
    )";

    zst::zlexer lexer(code);
    zst::zparser parser(lexer);
    auto tree = parser.parse();
    tree->eval();

    std::cout << zcontext["x"] << std::endl;

    REQUIRE(zcontext["x"] == 10);
}

TEST_CASE("PRINT TEST")
{
    std::string code = R"(
        x = 123;
        print x;
        print 123333;
    )";

    zst::zlexer lexer(code);
    zst::zparser parser(lexer);
    auto tree = parser.parse();
    tree->eval();
}