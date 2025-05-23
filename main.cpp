#include "zscript/zAST/nodes.h"
#include <functional>
#include "pot/utils/time_it.h"
#include <thread>

int main()
{
    /*
        x = 0;
        for (i = 0; i < 10; i = i + 1)
            x = x + i;
    */

    using namespace zst::zast;

    // auto dur = pot::utils::time_it<std::chrono::nanoseconds>(5, [] {}, []()
    //                                                          {
    //     auto prog = std::make_unique<BlockNode>();
    //     prog->add(std::make_unique<AssignNode>("x", std::make_unique<NumberNode>(0)));
    //     prog->add(std::make_unique<ForNode>(
    //     std::make_unique<AssignNode>("i", std::make_unique<NumberNode>(0)),
    //     std::make_unique<BinaryOpNode>('<', std::make_unique<VarNode>("i"), std::make_unique<NumberNode>(10)),
    //     std::make_unique<AssignNode>("i", std::make_unique<BinaryOpNode>('+', std::make_unique<VarNode>("i"), std::make_unique<NumberNode>(1))),
    //     std::make_unique<PrintNode>(std::make_unique<VarNode>("i"))));
    //     prog->eval(); 
    // }).count();

    // printf("\n\n");
    // std::cout << dur << "ns" << std::endl;

    auto prog = std::make_unique<zBlockNode>();
    prog->add(std::make_unique<zAssignNode>("x", std::make_unique<zNumberNode>(100)));
    prog->add(std::make_unique<zIfNode>(
        std::make_unique<zBinaryOpNode>('<', std::make_unique<zVarNode>("x"), std::make_unique<zNumberNode>(10)),
        std::make_unique<zPrintNode>(std::make_unique<zNumberNode>(0)),
        std::make_unique<zPrintNode>(std::make_unique<zNumberNode>(1))));

    prog->eval();
    return 0;
}
