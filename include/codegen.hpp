#pragma once
#ifndef BE_BLT_CODEGEN_HPP_
#define BE_BLT_CODEGEN_HPP_

#include <be/core/console_indent.hpp>
#include <sstream>

namespace be::blt {

///////////////////////////////////////////////////////////////////////////////
template <typename Str>
void single_quote_escape(const Str& data, std::ostream& os, I32 wrap = 80, I32 initial_wrap = -1) {
   if (initial_wrap < 0) {
      initial_wrap = wrap;
   }

   os << '\'';
   I32 line_count = 1;
   bool initial_line = true;
   bool escape_space = false;
   for (auto c : data) {
      if (escape_space) {
         escape_space = false;
         if (c == ' ') {
            os << "\\x20";
            line_count += 4;
            continue;
         }
      }
      if (c >= ' ' && c < '\'' ||
          c > '\'' && c < '\\' ||
          c > '\\' && c <= '~') {
         os << c;
         ++line_count;
      } else switch (c) {
         case '\n':  line_count += 2; os << "\\n"; break;
         case '\r':  line_count += 2; os << "\\r"; break;
         case '\t':  line_count += 2; os << "\\t"; break;
         case '\\':  line_count += 2; os << "\\\\"; break;
         case '\'':  line_count += 2; os << "\\'"; break;
         default: {
            const char* hex = "0123456789ABCDEF";
            os << "\\x" << hex[(c >> 4) & 0xF] << hex[c & 0xF];
            line_count += 4;
            break;
         }
      }
      if (initial_line) {
         if (initial_wrap > 0 && line_count >= initial_wrap) {
            os << "\\z" << nl;
            initial_line = false;
            line_count = 0;
            escape_space = true;
         }
      } else {
         if (wrap > 0 && line_count >= wrap) {
            os << "\\z" << nl;
            line_count = 0;
            escape_space = true;
         }
      }
   }
   os << '\'';
}

///////////////////////////////////////////////////////////////////////////////
template <typename Str>
S single_quote_escape(const Str& data) {
   std::ostringstream oss;
   single_quote_escape(data, oss, -1);
   return oss.str();
}

///////////////////////////////////////////////////////////////////////////////
template <typename Str>
S limit_length(const Str& data, I32 max_chars = 15, bool ellipsis = true) {
   if (data.size() > max_chars) {
      S val(data.begin(), data.begin() + max_chars);
      if (ellipsis) {
         val.append("...");
      }
      return val;
   } else {
      return S(data.begin(), data.end());
   }
}

} // be::blt

#endif
