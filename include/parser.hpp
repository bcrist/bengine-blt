#pragma once
#ifndef BE_BLT_PARSER_HPP_
#define BE_BLT_PARSER_HPP_

#include "token_data.hpp"
#include "template_wrapper_node.hpp"
#include "block_node.hpp"
#include "if_node.hpp"
#include <vector>

namespace be::blt {

///////////////////////////////////////////////////////////////////////////////
class Parser final : Immovable {
   using token_container = std::vector<TokenData>;
   using token_iterator = token_container::const_iterator;
   using node_ptr = std::unique_ptr<Node>;
public:
   Parser(const token_container& tokens);
   void generate(std::ostream& os);
   void debug(std::ostream& os);

private:
   const token_container& tokens_;
   token_iterator error_start_;
   token_iterator next_;

   bool check_next_() const;
   bool check_next_(TokenType token) const;
   bool try_next_(TokenType token);
   const TokenData* expect_next_(TokenType token);
   node_ptr expect_(node_ptr node, const char* expected);

   void error_(const char* expected);

   node_ptr template_wrapper_();
   void template_(SequenceNode& node);
   node_ptr template_part_();
   node_ptr stmt_or_seq_();
   node_ptr stmt_();
   node_ptr stmt_prefix_();

   node_ptr seq_block_();
   node_ptr block_();
   node_ptr if_(IfNode* chain = nullptr);
   void if_else_(IfNode& chain);
   node_ptr while_();
   node_ptr for_();
   node_ptr with_();
   node_ptr choose_();
   bool choose_clause_(IfNode& chain);
   node_ptr assign_(bool local = true);

   node_ptr id_list_();

   node_ptr wrapped_expr_list_();
   node_ptr expr_list_();
   node_ptr expr_();
   node_ptr subexpr_();
   node_ptr subexpr_prefix_();
   bool subexpr_suffix_(node_ptr& expr_node);

   node_ptr ref_or_call_();
   node_ptr ref_();
   bool ref_suffix_(node_ptr& ref_node);
};

} // be::blt

#endif
