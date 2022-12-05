#include <iostream>
#include <time.h>

int if_else_branch(int a, int b) {
    if(a) {
        return a;
    }
    else if(b) {
        return b;
    }
    else return 42;
}

int foo(int a, int b) {
    std::cout << "standard" << std::endl;
    if(a || b) {
        return 5;
    }
    else return -1;
}

int foo2(int a, int b) {
    std::cout << "standard" << std::endl;
    if(a || b) {
        return 5;
    }
    else if(a + b) {
        return -1;
    }
}

int foo_branchless(int a, int b) {
    std::cout << "branchless" << std::endl;
    bool c = a || b;
    return c*5 + !c*(-1);
}

void run_n_times(int (*func)(int , int), int n) {

    for(int i = 0; i < n; i++) {
        int a = rand()%100;
        int b = rand()%100;

        std::cout << "a: " << a << " b: " << b << std::endl;
        std::cout << func(a, b) << std::endl;
    }
}

int
main(int argc, char ** argv)
{
    int result = 0;
    /* initialize random seed */
    srand(time(NULL));

    run_n_times(foo, 1000);
    run_n_times(foo_branchless, 1000);

    return result;
}