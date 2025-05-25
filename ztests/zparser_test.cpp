#include <catch2/catch_test_macros.hpp>
#include "zscript/zparser/zparser.h"

#include "iostream"

using namespace zst;

TEST_CASE("EXPRESSION AND ASSIGNE TEST")
{
    std::string code = R"(
      y = [1; 2];
      x = [2; 123];
      z = y + [ 2  ;88] - x;
      print z;
    )";

    zst::zlexer lexer(code);
    zst::zparser parser(lexer);
    auto tree = parser.parse();
    tree->eval();

    REQUIRE((zcontext["z"](0, 0) == 1. && zcontext["z"](0, 1) == -33.));
}

TEST_CASE("IF TEST")
{
    std::string code = R"(
       zov = [123, 123; 0, 0];
       temp = zov > [1, 1; 1, 1];
       print temp + [9, 9; 10, 10];

       if ([0, 0; 0, 0]) {
           print 0;
       } else {
           print 1;
       }

       if     ([123, 12; 9, 0] @ [1, 2; 123, 1123]) { print [123, 12; 9, 0] * [1, 2; 123, 1123]; }
       if     ([123, 12; 9, 0] @ [1, 2; 123, 1123] * [0, 0; 0, 0]) { print [123, 12; 9, 0] * [1, 2; 123, 1123]; }
       else  { print 11111; }
    )";

    zst::zlexer lexer(code);
    zst::zparser parser(lexer);
    auto tree = parser.parse();
    tree->eval();
}

TEST_CASE("WHILE TEST")
{
    std::string code = R"(
        x = 0;
        while(x <= 100)
        {
            print x;
            x = x + 1;
        }
    )";

    zst::zlexer lexer(code);
    zst::zparser parser(lexer);
    auto tree = parser.parse();
    tree->eval();

    REQUIRE(zcontext["x"](0, 0) == 101);
}

TEST_CASE("FOR TEST")
{
    std::string code = R"(
        x = 0;
        for(i = 0; i < 113; i = i + 5)
        {
            x = x + i;
            print x;
        }
    )";

    zst::zlexer lexer(code);
    zst::zparser parser(lexer);
    auto tree = parser.parse();
    tree->eval();

    REQUIRE(zcontext["x"](0, 0) == 1265);
}

TEST_CASE("ALL and ANY")
{
    std::string code = R"(
        C = [0; 0] == [1; 0];
        print C;
        flag1 = all_of(C);
        flag2 = any_of(C);
        print flag1;
        print flag2;
    )";

    zst::zlexer lexer(code);
    zst::zparser parser(lexer);
    auto tree = parser.parse();
    tree->eval();

    REQUIRE(zcontext["flag1"](0, 0) == 0.);
    REQUIRE(zcontext["flag2"](0, 0) == 1.);
}