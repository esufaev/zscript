#include "zscript/zAST/nodes.h"
#include <functional>
#include "pot/utils/time_it.h"
#include <thread>
#include "zscript/zparser/zparser.h"
#include "assert.h"

#include "zscript/zparser/zlexer.h"

int main()
{
    /*
        x = 0;
        for (i = 0; i < 10; i = i + 1)
            x = x + i;
    */
    std::string data = "A = []";

    zst::zast::zlexer lexer(data);
    std::vector<zst::ztoken> ztokens{};
    ztokens.push_back(lexer.next());
    while (ztokens.back().type != zst::ztoken_type::End) {
        ztokens.push_back(lexer.next());
    }
    return 0;
}
