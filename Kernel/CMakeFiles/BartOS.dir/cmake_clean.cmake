file(REMOVE_RECURSE
  "BartOS"
  "BartOS.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang ASM_NASM CXX)
  include(CMakeFiles/BartOS.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
