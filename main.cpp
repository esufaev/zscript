#include "zscript/zAST/nodes.h"
#include "zscript/zparser/zlexer.h"
#include "zscript/zparser/zlexer.h"

int main()
{
    /*
        x = 0;
        for (i = 0; i < 10; i = i + 1)
            x = x + i;
    */

    using namespace zst::ast;

    // auto prog = std::make_unique<BlockNode>();
    // prog->add(std::make_unique<AssignNode>("x", std::make_unique<NumberNode>(0)));
    // prog->add(std::make_unique<ForNode>(
    //     std::make_unique<AssignNode>("i", std::make_unique<NumberNode>(0)),
    //     std::make_unique<BinaryOpNode>('<', std::make_unique<VarNode>("i"), std::make_unique<NumberNode>(10)),
    //     std::make_unique<AssignNode>("i", std::make_unique<BinaryOpNode>('+', std::make_unique<VarNode>("i"), std::make_unique<NumberNode>(1))),
    //     std::make_unique<PrintNode>(std::make_unique<VarNode>("i"))));
    // prog->eval();

    std::string input = "if (A == 0) { A = A + 1 }; \n x = 0; \nfor (i = 0; i < 10; i = i + 1)\n x = x + i; ";
    zlexer lexer(input);
    std::vector<token> tokens{lexer.next()};
    while (tokens.back().type != token_type::End) {
        tokens.push_back(lexer.next());
    }

    return 0;
}
