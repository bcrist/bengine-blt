#include "pch.hpp"
#include "parser.hpp"
#include "document_node.hpp"
#include "raw_lua_node.hpp"
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
#include "simple_sequence_node.hpp"
#include "wrapped_expr_node.hpp"
#include <be/core/logging.hpp>
#include <be/core/exceptions.hpp>

namespace be::blt {

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
   (*template_wrapper_())(os);
}

///////////////////////////////////////////////////////////////////////////////
void Parser::debug(std::ostream& os) {
   error_start_ = tokens_.begin();
   next_ = tokens_.begin();
   bool e = true;
   NodeDebugContext ctx { S("  "), S("--"), S("  "), e };
   template_wrapper_()->debug(os, ctx);
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
std::unique_ptr<Node> Parser::expect_(node_ptr node, const char* expected) {
   if (!node) {
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

   throw RecoverableError(std::make_error_code(std::errc::illegal_byte_sequence), "Parse Error");
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::template_wrapper_() {
   auto n = std::make_unique<TemplateWrapperNode>();
   template_(*n);
   if (check_next_()) {
      be_short_error("Parse Error") << "Expected EOF but found " << pretty_token(*next_) | default_log();
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
void Parser::template_(SequenceNode& node) {
   node_ptr part;
   while (part = template_part_()) {
      node.seq.push_back(std::move(part));
   }
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::template_part_() {
   node_ptr n;
   for (;;) {
      try {
         if (check_next_(TokenType::document)) {
            n = std::make_unique<DocumentNode>(&(*next_));
            ++next_;
         } else {
            n = stmt_();
         }
         break;
      } catch (const RecoverableError&) {
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
std::unique_ptr<Node> Parser::stmt_or_seq_() {
   node_ptr n;
   if (check_next_(TokenType::brace_opener)) {
      n = seq_block_();
      if (n && try_next_(TokenType::kw_until)) {
         n = std::make_unique<UntilNode>(std::move(n), expect_(expr_(), "expression"));
      }
   } else {
      n = stmt_();
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::stmt_() {
   node_ptr n = stmt_prefix_();
   if (n && try_next_(TokenType::kw_until)) {
      n = std::make_unique<UntilNode>(std::move(n), expect_(expr_(), "expression"));
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::stmt_prefix_() {
   node_ptr n;
   if (check_next_()) {
      switch (next_->type) {
         case TokenType::lua:          n = std::make_unique<LuaNode>(&(*next_)); ++next_; break;
         case TokenType::brace_opener: n = block_(); break;
         case TokenType::kw_if:        n = if_(); break;
         case TokenType::kw_while:     n = while_(); break;
         case TokenType::kw_for:       n = for_(); break;
         case TokenType::kw_with:      n = with_(); break;
         case TokenType::kw_choose:    n = choose_(); break;

         case TokenType::semicolon:
         case TokenType::kw_break:
         case TokenType::kw_continue:
            n = std::make_unique<TokenNode>(next_->type);
            ++next_;
            break;

         default: {
            n = assign_();
            if (!n) {
               n = wrapped_expr_list_();
            }
            break;
         }
      }
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::seq_block_() {
   if (try_next_(TokenType::brace_opener)) {
      std::unique_ptr<SimpleSequenceNode> seq = std::make_unique<SimpleSequenceNode>();
      template_(*seq);
      expect_next_(TokenType::brace_closer);
      return seq;
   }
   return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::block_() {
   if (try_next_(TokenType::brace_opener)) {
      std::unique_ptr<BlockNode> block = std::make_unique<BlockNode>();
      template_(*block);
      expect_next_(TokenType::brace_closer);
      return block;
   }
   return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::if_(IfNode* chain) {
   node_ptr n;
   if (try_next_(TokenType::kw_if)) {
      node_ptr expr = expect_(expr_(), "expression");
      node_ptr block = stmt_or_seq_();
      if (chain) {
         chain->clauses.emplace_back(std::move(expr), std::move(block));
         if_else_(*chain);
      } else {
         auto node = std::make_unique<IfNode>();
         node->clauses.emplace_back(std::move(expr), std::move(block));
         if_else_(*node);
         n = std::move(node);
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
         chain.else_block = seq_block_();
      } else {
         chain.else_block = stmt_();
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::while_() {
   node_ptr n;
   if (try_next_(TokenType::kw_while)) {
      node_ptr expr = expect_(expr_(), "expression");
      n = std::make_unique<WhileNode>(std::move(expr), stmt_or_seq_());
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::for_() {
   node_ptr n;
   if (try_next_(TokenType::kw_for)) {
      expect_next_(TokenType::paren_opener);
      node_ptr init = assign_();
      expect_next_(TokenType::semicolon);
      node_ptr condition = expr_();
      expect_next_(TokenType::semicolon);
      node_ptr update = assign_(false);
      expect_next_(TokenType::paren_closer);
      n = std::make_unique<ForNode>(std::move(init), std::move(condition), std::move(update), stmt_or_seq_());
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::with_() {
   node_ptr n;
   if (try_next_(TokenType::kw_with)) {
      bool each = try_next_(TokenType::kw_each);
      node_ptr expr = expect_(expr_(), "expression");
      if (each) {
         if (try_next_(TokenType::kw_using)) {
            if (try_next_(TokenType::pound)) {
               n = std::make_unique<WithEachUsingIndexNode>(std::move(expr), stmt_or_seq_());
            } else {
               node_ptr using_expr = expr_();
               if (using_expr) {
                  n = std::make_unique<WithEachUsingNode>(std::move(expr), std::move(using_expr), stmt_or_seq_());
               } else {
                  n = std::make_unique<WithEachNode>(std::move(expr), stmt_or_seq_());
               }
            }
         } else {
            n = std::make_unique<WithEachNode>(std::move(expr), stmt_or_seq_());
         }
      } else {
         n = std::make_unique<WithNode>(std::move(expr), stmt_());
      }
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::choose_() {
   node_ptr n;
   if (try_next_(TokenType::kw_choose)) {
      expect_next_(TokenType::brace_opener);
      auto node = std::make_unique<ChooseNode>();
      while (choose_clause_(*node));
      expect_next_(TokenType::brace_closer);
      node->init();
      n = std::move(node);
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
bool Parser::choose_clause_(IfNode& chain) {
   if (try_next_(TokenType::paren_opener)) {
      node_ptr case_expr = expr_();
      expect_next_(TokenType::paren_closer);
      node_ptr block = expect_(seq_block_(), "block");
      if (case_expr) {
         chain.clauses.emplace_back(std::move(case_expr), std::move(block));
      } else {
         chain.else_block = std::move(block);
      }
      return true;
   }
   return false;
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::assign_(bool local) {
   node_ptr n;
   auto restore_point = next_;
   node_ptr ids = id_list_();
   if (ids && try_next_(TokenType::eq)) {
      n = std::make_unique<AssignNode>(std::move(ids), expect_(expr_list_(), "expression-list"), local);
   } else {
      next_ = restore_point;
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::id_list_() {
   node_ptr n ;
   if (check_next_(TokenType::identifier)) {
      auto list = std::make_unique<IdListNode>();

      list->ids.push_back(next_->text);
      error_start_ = next_;
      ++next_;

      while (try_next_(TokenType::comma)) {
         list->ids.push_back(expect_next_(TokenType::identifier)->text);
      }

      n = std::move(list);
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::wrapped_expr_list_() {
   node_ptr n = expr_();
   if (n) {
      n = std::make_unique<WrappedExprNode>(std::move(n));

      if (check_next_(TokenType::comma)) {
         auto list = std::make_unique<BlockNode>();
         list->seq.push_back(std::move(n));

         while (try_next_(TokenType::comma)) {
            list->seq.push_back(std::make_unique<WrappedExprNode>(expect_(expr_(), "expression")));
         }

         n = std::move(list);
      }
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::expr_list_() {
   node_ptr n = expr_();
   if (n && check_next_(TokenType::comma)) {
      auto list = std::make_unique<ExprListNode>();
      list->exprs.push_back(std::move(n));

      while (try_next_(TokenType::comma)) {
         list->exprs.push_back(expect_(expr_(), "expression"));
      }

      n = std::move(list);
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::expr_() {
   node_ptr n = subexpr_();
   if (try_next_(TokenType::question)) {
      node_ptr true_expr = expect_(expr_(), "expression");
      expect_next_(TokenType::colon);
      n = std::make_unique<TernaryNode>(std::move(n), std::move(true_expr), expect_(expr_(), "expression"));
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::subexpr_() {
   node_ptr n = subexpr_prefix_();
   if (n) {
      while (subexpr_suffix_(n));
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::subexpr_prefix_() {
   node_ptr n;
   if (check_next_()) {
      switch (next_->type) {
         case TokenType::kw_nil:
         case TokenType::kw_false:
         case TokenType::kw_true:
            n = std::make_unique<TokenNode>(next_->type);
            ++next_;
            break;

         case TokenType::numeric_literal:
         case TokenType::string_literal:
            n = std::make_unique<TokenDataNode>(&(*next_));
            ++next_;
            break;

         case TokenType::minus:
         case TokenType::tilde:
         case TokenType::kw_not:
         case TokenType::pound: {
            TokenType type = next_->type;
            error_start_ = next_;
            ++next_;
            n = std::make_unique<UnaryOpNode>(type, expect_(subexpr_(), "expression"));
            break;
         }

         case TokenType::paren_opener:
            error_start_ = next_;
            ++next_;
            n = std::make_unique<ParenExprNode>(expect_(expr_(), "expression"));
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
bool Parser::subexpr_suffix_(node_ptr& expr_node) {
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
            expr_node = std::make_unique<BinaryOpNode>(std::move(expr_node), token, expect_(subexpr_(), "expression"));
            return true;
         }

      }
   }
   return false;
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::ref_or_call_() {
   //ref-or-call-expr := ref [ [ ':' Id ] '(' expr-list ')' ] ;
   node_ptr n = ref_();
   if (n) {
      if (try_next_(TokenType::colon)) {
         SV method = expect_next_(TokenType::identifier)->text;
         expect_next_(TokenType::paren_opener);
         node_ptr expr_list = expr_list_();
         expect_next_(TokenType::paren_closer);
         n = std::make_unique<CallNode>(std::move(n), std::move(expr_list), method);
      } else if (try_next_(TokenType::paren_opener)) {
         node_ptr expr_list = expr_list_();
         expect_next_(TokenType::paren_closer);
         n = std::make_unique<CallNode>(std::move(n), std::move(expr_list));
      }
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Node> Parser::ref_() {
   node_ptr n;
   U32 money = 0;
   while (try_next_(TokenType::dollars)) ++money;
   if (money) {
      n = std::make_unique<GetContextNode>(money - 1);
   } else if (check_next_(TokenType::identifier)) {
      n = std::make_unique<RefNode>(next_->text);
      error_start_ = next_;
      ++next_;
   }
   if (n) {
      while (ref_suffix_(n));
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
bool Parser::ref_suffix_(node_ptr& ref_node) {
   if (try_next_(TokenType::dot)) {
      ref_node = std::make_unique<RefNode>(expect_next_(TokenType::identifier)->text, std::move(ref_node));
      return true;
   } else if (try_next_(TokenType::bracket_opener)) {
      ref_node = std::make_unique<RefBracketNode>(std::move(ref_node), expect_(expr_(), "expression"));
      expect_next_(TokenType::bracket_closer);
      return true;
   } else {
      return false;
   }
}

} // be::blt
