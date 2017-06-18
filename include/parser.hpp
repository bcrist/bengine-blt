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
   Node expect_(Node node, const char* expected);

   void error_(const char* expected);

   TemplateWrapperNode template_wrapper_();
   SequenceNode template_();
   Node template_part_();
   Node stmt_or_seq_();
   Node stmt_();
   Node stmt_prefix_();

   template <typename N = BlockNode>
   Node block_();
   Node if_(IfNode* chain = nullptr);
   void if_else_(IfNode& chain);
   Node while_();
   Node for_();
   Node with_();
   Node choose_();
   bool choose_clause_(IfNode& chain);
   Node assign_(bool local = true);

   Node id_list_();

   Node wrapped_expr_list_();
   Node expr_list_();
   Node expr_();
   Node subexpr_();
   Node subexpr_prefix_();
   bool subexpr_suffix_(Node& expr_node);

   Node ref_or_call_();
   Node ref_();
   bool ref_suffix_(Node& ref_node);
};

} // be::blt

#endif
