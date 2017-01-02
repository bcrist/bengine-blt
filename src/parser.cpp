#include "pch.hpp"
#include "parser.hpp"
#include "document_node.hpp"
#include "lua_node.hpp"
#include "token_node.hpp"
#include "until_node.hpp"
#include "while_node.hpp"
#include "for_node.hpp"
#include "with_node.hpp"
#include "with_each_node.hpp"
#include "with_each_using_node.hpp"
#include "with_each_using_index_node.hpp"
#include "token_data_node.hpp"
#include "choose_node.hpp"
#include "assign_node.hpp"
#include "id_list_node.hpp"
#include "expr_list_node.hpp"
#include "ternary_node.hpp"
#include "unary_op_node.hpp"
#include "binary_op_node.hpp"
#include "paren_expr_node.hpp"
#include "call_node.hpp"
#include "ref_node.hpp"
#include "ref_bracket_node.hpp"
#include "get_context_node.hpp"
#include "wrapped_expr_node.hpp"
#include <be/core/logging.hpp>
#include <be/core/exceptions.hpp>

namespace be {
namespace blt {

///////////////////////////////////////////////////////////////////////////////
Parser::Parser(const token_container& tokens)
   : tokens_(tokens),
     error_start_(tokens.end()),
     next_(tokens.end())
{ }

///////////////////////////////////////////////////////////////////////////////
void Parser::generate(std::ostream& os) {
   error_start_ = tokens_.begin();
   next_ = tokens_.begin();
   template_wrapper_()(os);
}

///////////////////////////////////////////////////////////////////////////////
void Parser::debug(std::ostream& os) {
   error_start_ = tokens_.begin();
   next_ = tokens_.begin();
   bool e = true;
   NodeDebugContext ctx { S("  "), S("--"), S("  "), e };
   template_wrapper_().debug(os, ctx);
}

///////////////////////////////////////////////////////////////////////////////
bool Parser::check_next_() const {
   return next_ != tokens_.end();
}

///////////////////////////////////////////////////////////////////////////////
bool Parser::check_next_(TokenType type) const {
   return check_next_() && next_->type == type;
}

///////////////////////////////////////////////////////////////////////////////
bool Parser::try_next_(TokenType type) {
   if (check_next_(type)) {
      error_start_ = next_;
      ++next_;
      return true;
   } else {
      return false;
   }
}

///////////////////////////////////////////////////////////////////////////////
const TokenData* Parser::expect_next_(TokenType type) {
   const TokenData* retval = nullptr;
   if (check_next_(type)) {
      retval = &(*next_);
      error_start_ = next_;
      ++next_;
   } else {
      error_(get_name(type));
   }
   return retval;
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::expect_(Node node, const char* expected) {
   if (is_empty(node)) {
      error_(expected);
   }
   return node;
}

///////////////////////////////////////////////////////////////////////////////
void Parser::error_(const char* expected) {
   if (check_next_()) {
      be_short_error("Parse Error") << "Expected " << expected << " but found " << pretty_token(*next_) | default_log();
   } else {
      be_short_error("Parse Error") << "Expected " << expected << " but found EOF" | default_log();
   }

   throw RecoverableException<void>("Parse Error");
}

///////////////////////////////////////////////////////////////////////////////
TemplateWrapperNode Parser::template_wrapper_() {
   TemplateWrapperNode n;
   Node part;
   while (!is_empty(part = template_part_())) {
      n.seq.push_back(std::move(part));
   }
   if (check_next_()) {
      be_short_error("Parse Error") << "Expected EOF but found " << pretty_token(*next_) | default_log();
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
SequenceNode Parser::template_() {
   SequenceNode n;
   Node part;
   while (!is_empty(part = template_part_())) {
      n.seq.push_back(std::move(part));
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::template_part_() {
   Node n;
   for (;;) {
      try {
         if (check_next_(TokenType::document)) {
            n = DocumentNode { &(*next_) };
            ++next_;
         } else {
            n = stmt_();
         }
         break;
      } catch (const RecoverableException<void>&) {
         using std::swap;
         swap(error_start_, next_);
         if (check_next_()) {
            ++next_;
         } else {
            break;
         }
      }
      if (!check_next_()) {
         break;
      }
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::stmt_or_seq_() {
   Node n;
   if (check_next_(TokenType::brace_opener)) {
      n = block_<SequenceNode>();
      if (!is_empty(n) && try_next_(TokenType::kw_until)) {
         n = UntilNode { std::move(n), expect_(expr_(), "expression") };
      }
   } else {
      n = stmt_();
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::stmt_() {
   Node n = stmt_prefix_();
   if (!is_empty(n) && try_next_(TokenType::kw_until)) {
      n = UntilNode { std::move(n), expect_(expr_(), "expression") };
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::stmt_prefix_() {
   Node n;
   if (check_next_()) {
      switch (next_->type) {
         case TokenType::lua:          n = LuaNode { &(*next_) }; ++next_; break;
         case TokenType::brace_opener: n = block_(); break;
         case TokenType::kw_if:        n = if_(); break;
         case TokenType::kw_while:     n = while_(); break;
         case TokenType::kw_for:       n = for_(); break;
         case TokenType::kw_with:      n = with_(); break;
         case TokenType::kw_choose:    n = choose_(); break;

         case TokenType::semicolon:
         case TokenType::kw_break:
         case TokenType::kw_continue:
            n = TokenNode { next_->type };
            ++next_;
            break;

         default: {
            n = assign_();
            if (is_empty(n)) {
               n = wrapped_expr_list_();
            }
            break;
         }
      }
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
template <typename N>
Node Parser::block_() {
   Node n;
   if (try_next_(TokenType::brace_opener)) {
      SequenceNode block = template_();
      expect_next_(TokenType::brace_closer);
      n = N { std::move(block) };
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::if_(IfNode* chain) {
   Node n;
   if (try_next_(TokenType::kw_if)) {
      Node expr = expect_(expr_(), "expression");
      Node block = stmt_or_seq_();
      if (chain) {
         chain->clauses.emplace_back(std::move(expr), std::move(block));
         if_else_(*chain);
      } else {
         IfNode node;
         node.clauses.emplace_back(std::move(expr), std::move(block));
         if_else_(node);
         n = node;
      }
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
void Parser::if_else_(IfNode& chain) {
   if (try_next_(TokenType::kw_else)) {
      if (check_next_(TokenType::kw_if)) {
         if_(&chain);
      } else if (check_next_(TokenType::brace_opener)) {
         chain.else_block = block_<SequenceNode>();
      } else {
         chain.else_block = stmt_();
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::while_() {
   Node n;
   if (try_next_(TokenType::kw_while)) {
      Node expr = expect_(expr_(), "expression");
      n = WhileNode { std::move(expr), stmt_or_seq_() };
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::for_() {
   Node n;
   if (try_next_(TokenType::kw_for)) {
      expect_next_(TokenType::paren_opener);
      Node init = assign_();
      expect_next_(TokenType::semicolon);
      Node condition = expr_();
      expect_next_(TokenType::semicolon);
      Node update = assign_(false);
      expect_next_(TokenType::paren_closer);
      n = ForNode { std::move(init), std::move(condition), std::move(update), stmt_or_seq_() };
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::with_() {
   Node n;
   if (try_next_(TokenType::kw_with)) {
      bool each = try_next_(TokenType::kw_each);
      Node expr = expect_(expr_(), "expression");
      if (each) {
         if (try_next_(TokenType::kw_using)) {
            if (try_next_(TokenType::pound)) {
               n = WithEachUsingIndexNode { std::move(expr), stmt_or_seq_() };
            } else {
               Node using_expr = expr_();
               if (!is_empty(using_expr)) {
                  n = WithEachUsingNode { std::move(expr), std::move(using_expr), stmt_or_seq_() };
               } else {
                  n = WithEachNode { std::move(expr), stmt_or_seq_() };
               }
            }
         } else {
            n = WithEachNode { std::move(expr), stmt_or_seq_() };
         }
      } else {
         n = WithNode { std::move(expr), stmt_() };
      }
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::choose_() {
   Node n;
   if (try_next_(TokenType::kw_choose)) { 
      expect_next_(TokenType::brace_opener);
      ChooseNode node;
      while (choose_clause_(node));
      expect_next_(TokenType::brace_closer);
      node.init();
      n = node;
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
bool Parser::choose_clause_(IfNode& chain) {
   if (try_next_(TokenType::paren_opener)) {
      Node expr = expr_();
      expect_next_(TokenType::paren_closer);
      if (!is_empty(expr)) {
         chain.clauses.emplace_back(std::move(expr), stmt_or_seq_());
      } else {
         chain.else_block = stmt_or_seq_();
      }
   }
   return false;
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::assign_(bool local) {
   Node n;
   auto restore_point = next_;
   Node ids = id_list_();
   if (!is_empty(ids) && try_next_(TokenType::eq)) {
      n = AssignNode { std::move(ids), expect_(expr_list_(), "expression-list"), local };
   } else {
      next_ = restore_point;
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::id_list_() {
   Node n ;
   if (check_next_(TokenType::identifier)) {
      IdListNode list;

      list.ids.push_back(next_->text);
      error_start_ = next_;
      ++next_;

      while (try_next_(TokenType::comma)) {
         list.ids.push_back(expect_next_(TokenType::identifier)->text);
      }

      n = list;
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::wrapped_expr_list_() {
   Node n = expr_();
   if (!is_empty(n)) {
      n = WrappedExprNode { std::move(n) };

      if (check_next_(TokenType::comma)) {
         SequenceNode list;
         list.seq.push_back(std::move(n));

         while (try_next_(TokenType::comma)) {
            list.seq.push_back(WrappedExprNode { expect_(expr_(), "expression") });
         }

         n = list;
      }
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::expr_list_() {
   Node n = expr_();
   if (!is_empty(n) && check_next_(TokenType::comma)) {
      ExprListNode list;
      list.exprs.push_back(std::move(n));

      while (try_next_(TokenType::comma)) {
         list.exprs.push_back(expect_(expr_(), "expression"));
      }

      n = list;
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::expr_() {
   Node n = subexpr_();
   if (try_next_(TokenType::question)) {
      Node true_expr = expect_(expr_(), "expression");
      expect_next_(TokenType::colon);
      n = TernaryNode { std::move(n), std::move(true_expr), expect_(expr_(), "expression") };
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::subexpr_() {
   Node n = subexpr_prefix_();
   if (!is_empty(n)) {
      while (subexpr_suffix_(n));
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::subexpr_prefix_() {
   Node n;
   if (check_next_()) {
      switch (next_->type) {
         case TokenType::kw_nil:
         case TokenType::kw_false:
         case TokenType::kw_true:
            n = TokenNode { next_->type };
            ++next_;
            break;

         case TokenType::numeric_literal:
         case TokenType::string_literal:
            n = TokenDataNode { &(*next_) };
            ++next_;
            break;

         case TokenType::minus:
         case TokenType::tilde:
         case TokenType::kw_not:
         case TokenType::pound: {
            TokenType type = next_->type;
            error_start_ = next_;
            ++next_;
            n = UnaryOpNode { type, expect_(subexpr_(), "expression") };
            break;
         }
         
         case TokenType::paren_opener:
            error_start_ = next_;
            ++next_;
            n = ParenExprNode { expect_(expr_(), "expression") };
            expect_next_(TokenType::paren_closer);
            break;

         default:
            n = ref_or_call_();
            break;
      }
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
bool Parser::subexpr_suffix_(Node& expr_node) {
   if (check_next_()) {
      switch (next_->type) {
         case TokenType::plus:
         case TokenType::minus:
         case TokenType::star:
         case TokenType::slash:
         case TokenType::slash2:
         case TokenType::percent:
         case TokenType::chevron:
         case TokenType::amp:
         case TokenType::amp2:
         case TokenType::bar:
         case TokenType::bar2:
         case TokenType::tilde:
         case TokenType::left:
         case TokenType::left2:
         case TokenType::right:
         case TokenType::right2:
         case TokenType::eq2:
         case TokenType::tilde_eq:
         case TokenType::left_eq:
         case TokenType::right_eq:
         case TokenType::dot2:
         case TokenType::kw_and:
         case TokenType::kw_or: {
            TokenType token = next_->type;
            error_start_ = next_;
            ++next_;
            expr_node = BinaryOpNode { std::move(expr_node), token, expect_(subexpr_(), "expression") };
            return true;
         }

      }
   }
   return false;
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::ref_or_call_() {
   //ref-or-call-expr := ref [ [ ':' Id ] '(' expr-list ')' ] ;
   Node n = ref_();
   if (!is_empty(n)) {
      if (try_next_(TokenType::colon)) {
         gsl::cstring_span<> method = expect_next_(TokenType::identifier)->text;
         expect_next_(TokenType::paren_opener);
         Node expr_list = expr_list_();
         expect_next_(TokenType::paren_closer);
         n = CallNode { std::move(n), std::move(expr_list), method };
      } else if (try_next_(TokenType::paren_opener)) {
         Node expr_list = expr_list_();
         expect_next_(TokenType::paren_closer);
         n = CallNode { std::move(n), std::move(expr_list) };
      }
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
Node Parser::ref_() {
   Node n;
   U32 money = 0;
   while (try_next_(TokenType::dollars)) ++money;
   if (money) {
      n = GetContextNode { money - 1 };
   } else if (check_next_(TokenType::identifier)) {
      n = RefNode { next_->text };
      error_start_ = next_;
      ++next_;
   }
   if (!is_empty(n)) {
      while (ref_suffix_(n));
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
bool Parser::ref_suffix_(Node& ref_node) {
   if (try_next_(TokenType::dot)) {
      ref_node = RefNode { expect_next_(TokenType::identifier)->text, std::move(ref_node) };
      return true;
   } else if (try_next_(TokenType::bracket_opener)) {
      ref_node = RefBracketNode { std::move(ref_node), expect_(expr_(), "expression") };
      expect_next_(TokenType::bracket_closer);
      return true;
   } else {
      return false;
   }
}

} // be::blt
} // be
