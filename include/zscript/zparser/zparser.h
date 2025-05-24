#pragma once

#include "zscript/zparser/zlexer.h"
#include "zscript/zAST/nodes.h"

#include <string>

namespace zst
{
    using namespace zast;

    class zparser
    {
        zlexer m_lexer;
        ztoken m_current;

        void next() { m_current = m_lexer.next(); }

        bool match(ztoken_type type)
        {
            if (m_current.type == type)
            {
                next();
                return true;
            }
            return false;
        }

        void expect(ztoken_type type)
        {
            if (!match(type))
                throw std::runtime_error("Unexpected token: " + m_current.text);
        }

        std::unique_ptr<zINode> parse_identifier()
        {
            std::string variable = m_current.text;
            next();
            expect(ztoken_type::Assign);
            auto expression = parse_assignment();
            expect(ztoken_type::Semicolon);
            return std::make_unique<zAssignNode>(variable, std::move(expression));
        }

        std::unique_ptr<zINode> parse_statement()
        {
            if (m_current.type == ztoken_type::LBrace)          { return parse_block();      }
            else if (m_current.type == ztoken_type::If)         { return parse_if();         }
            else if (m_current.type == ztoken_type::While)      { return parse_while();      }
            else if (m_current.type == ztoken_type::For)        { return parse_for();        }
            else if (m_current.type == ztoken_type::Identifier) { return parse_identifier(); }

            throw std::runtime_error("Unexpected statement: " + m_current.text);
        }

        std::unique_ptr<zINode> parse_binary_expression()
        {
            auto left = parse_left();

            while (m_current.type == ztoken_type::Plus || m_current.type == ztoken_type::Minus ||
                   m_current.type == ztoken_type::Mul || m_current.type == ztoken_type::Div ||
                   m_current.type == ztoken_type::Matmul || m_current.type == ztoken_type::Equal ||
                   m_current.type == ztoken_type::Notequal || m_current.type == ztoken_type::Less ||
                   m_current.type == ztoken_type::Greater || m_current.type == ztoken_type::Lessequal ||
                   m_current.type == ztoken_type::Greaterequal)
            {
                ztoken token = m_current;
                next();
                auto right = parse_left();
                left = std::make_unique<zBinaryOpNode>(token.type, std::move(left), std::move(right));
            }

            return left;
        }

        std::unique_ptr<zINode> parse_left()
        {
            if (m_current.type == ztoken_type::LParen)
            {
                next();
                auto expr = parse_assignment();
                expect(ztoken_type::RParen);
                return expr;
            }
            else if (m_current.type == ztoken_type::Number)
            {
                int value = std::stoi(m_current.text);
                next();
                return std::make_unique<zNumberNode>(value);
            }
            else if (m_current.type == ztoken_type::Identifier)
            {
                std::string name = m_current.text;
                next();
                return std::make_unique<zVarNode>(name);
            }

            throw std::runtime_error("Unexpected left expression");
        }

        std::unique_ptr<zINode> parse_block()
        {
            expect(ztoken_type::LBrace);
            auto block = std::make_unique<zBlockNode>();
            while (m_current.type != ztoken_type::RBrace && m_current.type != ztoken_type::End)
            {
                block->add(parse_statement());
            }
            expect(ztoken_type::RBrace);
            return block;
        }

        std::unique_ptr<zINode> parse_if()
        {
            expect(ztoken_type::If);
            expect(ztoken_type::LParen);
            auto condition = parse_assignment();
            expect(ztoken_type::RParen);
            auto true_branch = parse_statement();

            std::unique_ptr<zINode> else_branch = nullptr;
            if (m_current.type == ztoken_type::Else)
            {
                next();
                else_branch = parse_statement();
            }

            return std::make_unique<zIfNode>(std::move(condition), std::move(true_branch), std::move(else_branch));
        }

        std::unique_ptr<zINode> parse_assignment()
        {
            auto expr = parse_binary_expression();
            if (m_current.type == ztoken_type::Assign)
            {
                if (!dynamic_cast<zVarNode *>(expr.get()))
                    throw std::runtime_error("Left side of assignment must be a variable");
                std::string var = static_cast<zVarNode *>(expr.get())->name;
                next();
                auto right = parse_assignment();
                return std::make_unique<zAssignNode>(var, std::move(right));
            }
            return expr;
        }

        std::unique_ptr<zINode> parse_while()
        {
            expect(ztoken_type::While);
            expect(ztoken_type::LParen);
            auto condition = parse_assignment();
            expect(ztoken_type::RParen);
            auto body = parse_statement();
            return std::make_unique<zWhileNode>(std::move(condition), std::move(body));
        }

        std::unique_ptr<zINode> parse_for()
        {
            expect(ztoken_type::For);
            expect(ztoken_type::LParen);
            auto init = parse_statement();
            auto condition = parse_assignment();
            expect(ztoken_type::Semicolon);
            auto increment = parse_assignment();
            expect(ztoken_type::RParen);
            auto body = parse_statement();
            return std::make_unique<zForNode>(std::move(init), std::move(condition), std::move(increment), std::move(body));
        }

    public:
        explicit zparser(std::string code) : m_lexer(std::move(code)) { next(); }
        explicit zparser(zlexer lexer) : m_lexer(std::move(lexer)) { next(); }

        std::unique_ptr<zINode> parse()
        {
            std::unique_ptr<zst::zBlockNode> block = std::make_unique<zst::zBlockNode>();
            while (m_current.type != ztoken_type::End)
            {
                auto node = parse_statement();
                block->add(std::move(node));
            }
            return block;
        }
    };
}