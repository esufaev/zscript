#include <iostream> // временно
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

namespace zst::zast
{
    std::unordered_map<std::string, int> zcontext;

    struct zINode
    {
        virtual int eval() = 0;
        virtual ~zINode() = default;
    };


    struct zNumberNode : zINode
    {
        int value;
        zNumberNode(int v) : value(v) {}
        int eval() override { return value; }
    };

    struct zVarNode : zINode
    {
        std::string name;
        zVarNode(const std::string &n) : name(n) {}
        int eval() override { return zcontext[name]; }
    };

    struct zAssignNode : zINode
    {
        std::string name;
        std::unique_ptr<zINode> expr;
        zAssignNode(const std::string &n, std::unique_ptr<zINode> e) : name(n), expr(move(e)) {}
        int eval() override
        {
            int val = expr->eval();
            zcontext[name] = val;
            return val;
        }
    };

    struct zBinaryOpNode : zINode
    {
        char op;
        std::unique_ptr<zINode> lhs, rhs;
        zBinaryOpNode(char o, std::unique_ptr<zINode> l, std::unique_ptr<zINode> r) : op(o), lhs(move(l)), rhs(move(r)) {}
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

    struct zPrintNode : zINode
    {
        std::unique_ptr<zINode> expr;
        zPrintNode(std::unique_ptr<zINode> e) : expr(move(e)) {}
        int eval() override
        {
            int val = expr->eval();
            std::cout << val << std::endl;
            return val;
        }
    };

    struct zBlockNode : zINode
    {
        std::vector<std::unique_ptr<zINode>> stmts;
        void add(std::unique_ptr<zINode> stmt)
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

    struct zIfNode : zINode
    {
        std::unique_ptr<zINode> cond, then_branch, else_branch;
        zIfNode(std::unique_ptr<zINode> c, std::unique_ptr<zINode> t, std::unique_ptr<zINode> e = nullptr)
            : cond(move(c)), then_branch(move(t)), else_branch(move(e)) {}
        int eval() override
        {
            if (cond->eval())     return then_branch->eval();
            else if (else_branch) return else_branch->eval();
            return 0;
        }
    };

    struct zWhileNode : zINode
    {
        std::unique_ptr<zINode> cond, body;
        zWhileNode(std::unique_ptr<zINode> c, std::unique_ptr<zINode> b) : cond(move(c)), body(move(b)) {}
        int eval() override
        {
            while (cond->eval()) body->eval();
            return 0;
        }
    };

    struct zForNode : zINode
    {
        std::unique_ptr<zINode> init, cond, step, body;

        zForNode(std::unique_ptr<zINode> i, std::unique_ptr<zINode> c, std::unique_ptr<zINode> p, std::unique_ptr<zINode> b)
            : init(move(i)), cond(move(c)), step(move(p)), body(move(b)) {}
        int eval() override
        {
            for (init->eval(); cond->eval(); step->eval()) body->eval();
            return 0;
        }
    };
}
