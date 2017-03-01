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
      src {
         'src/*.cpp',
         pch_src 'src/pch.cpp'
      },
      define 'BE_BLT_IMPL',
      link_project 'core'
   },
   lib '-lua' {
      src 'src-lua/*.cpp',
      define 'BE_BLT_LUA_IMPL',
      link_project {
         'belua',
         'blt'
      }
   }
}
