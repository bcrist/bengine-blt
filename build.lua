module { name = 'blt',
   projects = {
      lib {
         custom = {
            outputs = { 'src/lexer.cpp' },
            inputs = { 'meta/lexer.rl' },
            name = 'ragel',
            command = 'ragel $flags -o $out $in',
            flags = '-s -C -L -G2',
            generator = true,
            restat = true
         },
         src = {
            'src/*.cpp',
            pch = 'src/pch.cpp'
         },
         preprocessor = {
            'BE_BLT_IMPL'
         }
      }
   }
}
