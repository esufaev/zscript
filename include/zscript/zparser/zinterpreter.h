#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "zscript/zparser/zparser.h"
#include "zscript/zparser/zlexer.h"

namespace zst
{
    class zinterpreter
    {
    public:
        void zfrun(const std::string &filename)
        {
            std::ifstream file(filename);
            if (!file.is_open())
            {
                std::cerr << "Zerror. Zfile not found: " << filename << std::endl;
                return;
            }

            std::stringstream buffer;
            buffer << file.rdbuf();
            execute(buffer.str());
        }

        void zirun()
        {
            std::string line;
            std::cout << "exit - to leave\n";

            while (true)
            {
                std::cout << ">>> ";
                if (!std::getline(std::cin, line))
                    break;

                if (line == "exit")
                    break;

                try
                {
                    execute(line);
                }
                catch (const std::exception &ex)
                {
                    std::cerr << "Zerror: " << ex.what() << std::endl;
                }
            }
        }

        void execute(const std::string &code)
        {
            zlexer lexer(code);
            zparser parser(lexer);
            auto tree = parser.parse();
            tree->eval();
        }
    };

} // namespace zst
