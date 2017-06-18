#pragma once
#ifndef BE_BLT_CHOOSE_NODE_HPP_
#define BE_BLT_CHOOSE_NODE_HPP_

#include "if_node.hpp"
#include "choose_comparison_node.hpp"
#include <be/core/service_atomic.hpp>
#include <be/core/services.hpp>
#include <sstream>

namespace be::blt {

struct ChooseNode : IfNode {

   int table_cache_id;

   void init() {
      if (!use_table_lookup()) {
         // wrap exprs
         for (auto& clause : clauses) {
            clause.first = ChooseComparisonNode { std::move(clause.first) };
         }
      } else {
         table_cache_id = service<std::atomic<U32>>(Id("be:blt:service:atomic-u32:table-cache-id")).fetch_add(1);
      }
   }

   bool use_table_lookup() const {
      if (clauses.size() > 4) {
         for (auto& clause : clauses) {
            if (!clause.first.is_static_constant()) {
               // table-based lookup doesn't work well with keys that aren't known until runtime
               return false;
            }
         }
         return true;
      }
      return false;
   }

   void operator()(std::ostream& os) const {
      if (use_table_lookup()) {
         do_table_lookup(os);
      } else {
         do_chained_if(os);
      }
   }

   void do_chained_if(std::ostream& os) const {
      os << nl << "do" << indent;
      os << nl << "local __k__ = __ctx__:get()";
      IfNode::operator()(os);
      os << unindent << nl << "end ";
   }

   void do_table_lookup(std::ostream& os) const {
      const std::pair<Node, Node>* nil_clause = nullptr;
      for (auto& clause : clauses) {
         if (!clause.first.is_nonnil_constant()) {
            // can't store a nil key in a table
            nil_clause = &clause;
         }
      }

      os << nl << "do" << indent;
      os << nl << "local __k__ = __ctx__:get()";

      if (nil_clause && !is_empty(nil_clause->second)) {
         os << nl << "if __k__ == nil then " << indent;
         nil_clause->second(os);
         os << unindent << nl << "else" << indent;
      }
         
      os << nl << "__k__ = __blt__.table_cache_(" << table_cache_id << ", function ()" << indent << indent;
      os << nl << "return {" << indent;
      
      for (auto& clause : clauses) {
         if (&clause != nil_clause) {
            os << nl << "[ " << indent;
            clause.first(os);
            os << " ] = function () " << indent;
            if (!is_empty(clause.second)) {
               clause.second(os);
            }
            os << unindent << nl << " end";
         }
      }
      
      os << unindent << nl << '}';
      os << unindent << nl << "end)[__k__]" << unindent;

      os << nl << "if __k__ then __k__() end";

      if (nil_clause && !is_empty(nil_clause->second)) {
         os << unindent << nl << "end";
      }
      os << unindent << nl << "end ";
   }

   void debug(std::ostream& os, NodeDebugContext& ctx) const {
      debug_c("Choose", os, ctx.c_prefix, ctx.last_line_empty);
      debug_if(os, ctx);
   }

};

} // be::blt

#endif
