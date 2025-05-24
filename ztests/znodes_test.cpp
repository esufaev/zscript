#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "zscript/zAST/nodes.h"

using namespace zst::zast;

TEST_CASE("ZIFNODE TEST")
{
    zcontext.clear();
    auto prog = std::make_unique<zBlockNode>();
    prog->add(std::make_unique<zAssignNode>("x", std::make_unique<zNumberNode>(100)));
    auto cond = std::make_unique<zBinaryOpNode>('<',
                                                std::make_unique<zVarNode>("x"),
                                                std::make_unique<zNumberNode>(10));
    auto then_branch = std::make_unique<zPrintNode>(
        std::make_unique<zAssignNode>("x", std::make_unique<zNumberNode>(0)));
    auto else_branch = std::make_unique<zPrintNode>(
        std::make_unique<zAssignNode>("x", std::make_unique<zNumberNode>(1)));
    prog->add(std::make_unique<zIfNode>(
        std::move(cond),
        std::move(then_branch),
        std::move(else_branch)));

    prog->eval();

    REQUIRE(zcontext["x"] == 1);
}

TEST_CASE("ZFORNODE TEST")
{
    zcontext.clear();
    auto prog = std::make_unique<zBlockNode>();
    prog->add(std::make_unique<zAssignNode>("x", std::make_unique<zNumberNode>(0)));
    prog->add(std::make_unique<zForNode>(
        std::make_unique<zAssignNode>("i", std::make_unique<zNumberNode>(0)),
        std::make_unique<zBinaryOpNode>('<', std::make_unique<zVarNode>("i"), std::make_unique<zNumberNode>(10)),
        std::make_unique<zAssignNode>("i", std::make_unique<zBinaryOpNode>('+', std::make_unique<zVarNode>("i"), std::make_unique<zNumberNode>(1))),
        std::make_unique<zPrintNode>(std::make_unique<zVarNode>("i"))));
    prog->eval();
    REQUIRE(zcontext["x"] == 0);
}

TEST_CASE("ZASSIGNNODE TEST")
{
    zcontext.clear();
    auto assign = std::make_unique<zAssignNode>("x", std::make_unique<zNumberNode>(42));
    int result = assign->eval();
    REQUIRE(result == 42);
    REQUIRE(zcontext["x"] == 42);
}

TEST_CASE("ZWHILENODE TEST")
{
    zcontext.clear();
    zcontext["i"] = 0;
    auto cond = std::make_unique<zBinaryOpNode>('<',
                                                std::make_unique<zVarNode>("i"),
                                                std::make_unique<zNumberNode>(3));

    auto inc = std::make_unique<zAssignNode>("i",
                                             std::make_unique<zBinaryOpNode>('+',
                                                                             std::make_unique<zVarNode>("i"),
                                                                             std::make_unique<zNumberNode>(1)));

    auto loop = std::make_unique<zWhileNode>(std::move(cond), std::move(inc));
    loop->eval();

    REQUIRE(zcontext["i"] == 3);
}