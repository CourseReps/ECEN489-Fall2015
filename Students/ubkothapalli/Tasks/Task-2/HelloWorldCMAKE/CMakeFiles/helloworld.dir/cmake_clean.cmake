FILE(REMOVE_RECURSE
  "CMakeFiles/helloworld.dir/helloworld.cpp.o"
  "helloworld.pdb"
  "helloworld"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang CXX)
  INCLUDE(CMakeFiles/helloworld.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
