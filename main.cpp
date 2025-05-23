#include "zscript/AST/nodes.h"

int main()
{
    /*
        x = 1;
        for (i = 0; i < 100; i = i + 10)
            x = x + i;
    */

   using namespace zst::ast;

    auto prog = std::make_unique<BlockNode>();
    prog->add(std::make_unique<AssignNode>("x", std::make_unique<NumberNode>(1)));
    prog->add(std::make_unique<ForNode>(
        std::make_unique<AssignNode>("i", std::make_unique<NumberNode>(0)),
        std::make_unique<BinaryOpNode>('<', std::make_unique<VarNode>("i"), std::make_unique<NumberNode>(100)),
        std::make_unique<AssignNode>("i", std::make_unique<BinaryOpNode>('+', std::make_unique<VarNode>("i"), std::make_unique<NumberNode>(10))),
        std::make_unique<AssignNode>("x", std::make_unique<BinaryOpNode>('+', std::make_unique<VarNode>("x"), std::make_unique<VarNode>("i")))));
    prog->add(std::make_unique<PrintNode>(std::make_unique<VarNode>("x")));
    prog->eval();

    return 0;
}
