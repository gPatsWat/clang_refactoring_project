# clang_refactoring_project

sudo apt install llvm-11-dev
sudo apt install clang-11
sudo apt install libclang-11-dev

/usr/bin/../lib/llvm-10/cmake/
/usr/bin/../lib/cmake/clang-10/ClangConfig.cmake

cd build
cmake -DCMAKE_BUILD_TYPE=Release ..

To run branch-converter over actual Cpp code:
./src/branch-converter ../test/test-progs/simple_if_else.cc (while in build directory)

For dry-run add -d flag.

To run unit tests:
./test/corct_unittests

To run benchmarks:
./test/benchmarks/bm_[OR/PLUS/BRANCHED/BRANCHLESS]

Clang API nits and grits:
getElse can return all kinds of statements (you can check by doing clang-check -ast-dump -ast-dump-filter=foo test/test-progs/if_else_basic.cc).

Some of these which can be:
IfStmt
CompoundStmt
ReturnStmt

So, we have to check while traversing a branch whether the node is really what we think it is!

Tools:
branchsite-lister:
prints all if-else statements, -DPRINT_SOURCELOC=1 will also print the location in project.

TODO: per function analysis should give branch misses and overhead in detail.
LOOK AT TRANSFORM CODE FOR LOOPCONVERT.CC!!!! IN ORDER TO HANDLE POTENTIALLY CROSS TU REFACTORING

Examples of transformations:
1. single if-else condition with only returns (no side-effects).

Eg:
singleton condition in if:
if(a) {
    return a;             -->           return bool(a)\*a + bool(a)*b
}
else return b;

complex condition in if:
if(a || b) {
    return a;             -->           return (c)\*a + (!c)*b where c = a || b
}
else return b;

Note: here, || can be replaced by any operator

2. multiple if else chain:
singleton if condition:
if(a) {
    return a;            -->            return bool(a)\*a + !bool(a)\*bool(b)*b
} else if(b) {
    return b;
}

complex if else condition:
if(a || b) {
    return a;           -->       return c\*a + !c\*d*b where c = a || b and d = a && b
} else if(a && b) {
    return b;
}

What happens if there are other non-arithmetico-logical conditions inside conditionals?
Let's look at some cases:

1. One such popular style of coding is to do variable assignment and wrap it in if conditions to ensure that the resource has been initialised.


In general, according to Fedor Pikus' technique, any general statement of the form:
sum += cond ? expr1 : expr2

or

if(cond) {
    var (op) expr1;
}
else var (op) expr2;

can be converted to:

term[2] = {expr2, expr1};
return term[bool(cond)];
or var (op) term[bool(cond)];

Note that this is a little difficult if there are multiple side effects.
TODO: extrapolate to two or more cases which depend on condition.

References:

Branch Pipelines:
https://www.youtube.com/watch?v=ckQAlp7WeHo

Fedor Pikus: C++ Branchless Programming videos
https://github.com/alpbintug/Branchless-programming/tree/master/basicOperations

https://stackoverflow.com/questions/11083066/getting-the-source-behind-clangs-ast

https://github.com/lifting-bits/rellic/blob/master/lib/AST/NestedCondProp.cpp

https://stackoverflow.com/questions/37320484/why-is-the-source-location-end-off-by-two-characters-here