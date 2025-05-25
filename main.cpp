#include <cassert>

#include "zscript/zAST/nodes.h"
#include <functional>
#include "pot/utils/time_it.h"
#include <thread>
#include "zscript/zparser/zparser.h"
#include "assert.h"

#include "zscript/zparser/zlexer.h"

int main()
{
    std::string code = R"(

        
        zov = [3, -33, 333];
        for (goida = 0; goida <= 100; goida = goida + 3)
        {
            if (zov == [9, 39, 339]) { print goida; }
            else { while (goida < 1000) { zov = zov + [5, 5, 5]; goida = goida + 1; } }
        }
        print zov - 5;


    )";

    zst::zlexer lexer(code);
    zst::zparser parser(lexer);
    auto tree = parser.parse();
    tree->eval();

    return 0;
}
