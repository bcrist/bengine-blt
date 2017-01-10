module 'blt' {
   lib {
      custom {
         outputs = 'src/lexer.cpp',
         inputs = 'meta/lexer.rl',
         rule = 'ragel',
         command = 'ragel $flags -o $out $in',
         flags = '-s -C -L -G2',
         generator = true,
         restat = true
      },
      pch_src 'src/pch.cpp',
      define 'BE_BLT_IMPL',
      link_project 'core'
   }
}
