# clang_refactoring_project

/usr/bin/../lib/llvm-10/cmake/
/usr/bin/../lib/cmake/clang-10/ClangConfig.cmake

cd build
cmake -DGTEST_ROOT=../googletest ..

./test/test-progs/if_else_basic.cc runs branch version and
branchless version each n times. This can be profiled with perf. The assembly output is also given which can be analysed to get the exact number of branch instructions present in the binary. This will enable reasoning about how many branches get eliminated, barring, the hardware optimisations which the cpu does.

TODO: per function analysis should give branch misses and overhead in detail.

References:

Fedor Pikus: C++ Branchless Programming videos
https://github.com/alpbintug/Branchless-programming/tree/master/basicOperations

https://stackoverflow.com/questions/11083066/getting-the-source-behind-clangs-ast