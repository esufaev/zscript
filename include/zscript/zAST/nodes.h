#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

#include "zscript/zparser/ztoken.h"
#include "zscript/zutils/zmatrix.h"

namespace zst::zast
{
    std::unordered_map<std::string, zst::zutils::zmatrix> zcontext;

    struct zINode
    {
        virtual zst::zutils::zmatrix eval() = 0;
        virtual ~zINode() = default;
    };

    struct zNumberNode : zINode
    {
        zst::zutils::zmatrix value;
        zNumberNode(zst::zutils::zmatrix v) : value(v) {}
        zst::zutils::zmatrix eval() override { return value; }
    };

    struct zVarNode : zINode
    {
        std::string name;
        zVarNode(const std::string &n) : name(n) {}
        zst::zutils::zmatrix eval() override { return zcontext[name]; }
    };

    struct zAssignNode : zINode
    {
        std::string name;
        std::unique_ptr<zINode> expr;
        zAssignNode(const std::string &n, std::unique_ptr<zINode> e) : name(n), expr(move(e)) {}
        zst::zutils::zmatrix eval() override
        {
            zst::zutils::zmatrix val = expr->eval();
            zcontext[name] = val;
            return val;
        }
    };

    struct zBinaryOpNode : zINode
    {
        ztoken_type op;
        std::unique_ptr<zINode> lhs, rhs;
        zBinaryOpNode(ztoken_type o, std::unique_ptr<zINode> l, std::unique_ptr<zINode> r) : op(o), lhs(move(l)), rhs(move(r)) {}
        zst::zutils::zmatrix eval() override
        {
            zst::zutils::zmatrix a = lhs->eval();
            zst::zutils::zmatrix b = rhs->eval();
            switch (op)
            {
                case zst::ztoken_type::Plus:         return a + b;
                case zst::ztoken_type::Minus:        return a - b;
                case zst::ztoken_type::Mul:          return a * b;
                case zst::ztoken_type::Div:          return b / a;
                case zst::ztoken_type::Less:         return a < b;
                case zst::ztoken_type::Greater:      return a > b;
                case zst::ztoken_type::Equal:        return a == b;
                case zst::ztoken_type::Notequal:     return a != b;
                case zst::ztoken_type::Greaterequal: return a >= b;
                case zst::ztoken_type::Lessequal:    return a <= b;
                case zst::ztoken_type::Matmul:       return a.dot(b);
            }
            return 0;
        }
    };

    struct zPrintNode : zINode
    {
        std::unique_ptr<zINode> expr;
        zPrintNode(std::unique_ptr<zINode> e) : expr(move(e)) {}
        zst::zutils::zmatrix eval() override
        {
            zst::zutils::zmatrix val = expr->eval();
            val.print();
            return val;
        }
    };

    struct zBlockNode : zINode
    {
        std::vector<std::unique_ptr<zINode>> stmts;
        void add(std::unique_ptr<zINode> stmt)
        {
            stmts.push_back(std::move(stmt));
        }
        zst::zutils::zmatrix eval() override
        {
            zst::zutils::zmatrix result = 0;
            for (auto &stmt : stmts) result = stmt->eval();
            return result;
        }
    };

    struct zIfNode : zINode
    {
        std::unique_ptr<zINode> cond, then_branch, else_branch;
        zIfNode(std::unique_ptr<zINode> c, std::unique_ptr<zINode> t, std::unique_ptr<zINode> e = nullptr)
            : cond(move(c)), then_branch(move(t)), else_branch(move(e)) {}
        zst::zutils::zmatrix eval() override
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
        zst::zutils::zmatrix eval() override
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
        zst::zutils::zmatrix eval() override
        {
            for (init->eval(); cond->eval(); step->eval()) body->eval();
            return 0;
        }
    };

    struct zAllofNode : zINode
    {
        std::unique_ptr<zINode> expr;    
        zAllofNode(std::unique_ptr<zINode> e) : expr(move(e)) {}
        zst::zutils::zmatrix eval() override
        {
            zst::zutils::zmatrix result = expr->eval().all_of();
            return result;
        }
    };

    struct zAnyofNode : zINode
    {
        std::unique_ptr<zINode> expr;
        zAnyofNode(std::unique_ptr<zINode> e) : expr(move(e)) {}
        zst::zutils::zmatrix eval() override
        {
            zst::zutils::zmatrix result = expr->eval().any_of();
            return result;
        }
    };
}
