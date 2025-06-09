#include <cassert>

#include <functional>
#include <thread>
#include "assert.h"

#include "zscript/zparser/zinterpreter.h"

int main()
{
    zst::zinterpreter interpreter;
    interpreter.zfrun("/home/pod/Projects/zscript/file.zs");
    interpreter.zirun();

    return 0;
}
