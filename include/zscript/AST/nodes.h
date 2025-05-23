#include <iostream> // временно
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

namespace zst::ast
{
    std::unordered_map<std::string, int> context;

    struct INode
    {
        virtual int eval() = 0;
        virtual ~INode() = default;
    };


    struct NumberNode : INode
    {
        int value;
        NumberNode(int v) : value(v) {}
        int eval() override { return value; }
    };

    struct VarNode : INode
    {
        std::string name;
        VarNode(const std::string &n) : name(n) {}
        int eval() override { return context[name]; }
    };

    struct AssignNode : INode
    {
        std::string name;
        std::unique_ptr<INode> expr;
        AssignNode(const std::string &n, std::unique_ptr<INode> e) : name(n), expr(move(e)) {}
        int eval() override
        {
            int val = expr->eval();
            context[name] = val;
            return val;
        }
    };

    struct BinaryOpNode : INode
    {
        char op;
        std::unique_ptr<INode> lhs, rhs;
        BinaryOpNode(char o, std::unique_ptr<INode> l, std::unique_ptr<INode> r) : op(o), lhs(move(l)), rhs(move(r)) {}
        int eval() override
        {
            int a = lhs->eval();
            int b = rhs->eval();
            switch (op)
            {
            case '+':
                return a + b;
            case '-':
                return a - b;
            case '*':
                return a * b;
            case '/':
                return b / a;
            case '<':
                return a < b;
            case '>':
                return a > b;
            case '=':
                return a == b;
            }
            return 0;
        }
    };

    struct PrintNode : INode
    {
        std::unique_ptr<INode> expr;
        PrintNode(std::unique_ptr<INode> e) : expr(move(e)) {}
        int eval() override
        {
            int val = expr->eval();
            std::cout << val << std::endl;
            return val;
        }
    };

    struct BlockNode : INode
    {
        std::vector<std::unique_ptr<INode>> stmts;
        void add(std::unique_ptr<INode> stmt)
        {
            stmts.push_back(move(stmt));
        }
        int eval() override
        {
            int result = 0;
            for (auto &stmt : stmts)
                result = stmt->eval();
            return result;
        }
    };

    struct IfNode : INode
    {
        std::unique_ptr<INode> cond, thenBranch, elseBranch;
        IfNode(std::unique_ptr<INode> c, std::unique_ptr<INode> t, std::unique_ptr<INode> e = nullptr)
            : cond(move(c)), thenBranch(move(t)), elseBranch(move(e)) {}
        int eval() override
        {
            if (cond->eval())
                return thenBranch->eval();
            else if (elseBranch)
                return elseBranch->eval();
            return 0;
        }
    };

    struct WhileNode : INode
    {
        std::unique_ptr<INode> cond, body;
        WhileNode(std::unique_ptr<INode> c, std::unique_ptr<INode> b) : cond(move(c)), body(move(b)) {}
        int eval() override
        {
            while (cond->eval()) body->eval();
            return 0;
        }
    };

    struct ForNode : INode
    {
        std::unique_ptr<INode> init, cond, step, body;

        ForNode(std::unique_ptr<INode> i, std::unique_ptr<INode> c, std::unique_ptr<INode> p, std::unique_ptr<INode> b)
            : init(move(i)), cond(move(c)), step(move(p)), body(move(b)) {}
        int eval() override
        {
            for (init->eval(); cond->eval(); step->eval()) body->eval();
            return 0;
        }
    };
}
