#include <iostream>
#include <time.h>

int glob1 = 2, glo = 3;

int if_else_branch(int a1, int b1)
{
    glo += 4;
    if (a1)
    {
        return a1;
    }
    else if (b1)
    {
        return b1;
    }
    else
        return 42;
}

int if_else_simple_return(int a2, int b2)
{
    if (a2 || b2)
    {
        return a2;
    }
    else
        return b2;
}

int simple_if_else_simple_return(int a3, int b3)
{
    if (a3 || b3)
        return a3;
    else
        return b3;
}

int simple_if_else_return(int a4, int b4)
{
    if (a4 || b4)
        return a4;
    else {
        return b4;
    }
}

int if_else_simple_return_side_effects(int a5, int b5)
{
    std::cout << "standard" << std::endl;
    if (a5 + b5)
    {

        std::cout << "hola!" << std::endl;
        return a5;
    }
    else
        return b5;
}

int if_else_no_side_effects(int a6, int b6)
{
    std::cout << "standard" << std::endl;
    if (a6 || b6)
    {
        return a6;
    }
    else if (a6 + b6)
    {
        return b6;
    }
}

int foo_branchless(int a, int b)
{
    std::cout << "branchless" << std::endl;
    bool c = a || b;
    return c * 5 + !c * (-1);
}

void run_n_times(int (*func)(int, int), int n)
{

    for (int i = 0; i < n; i++)
    {
        int a = rand() % 100;
        int b = rand() % 100;

        std::cout << "a: " << a << " b: " << b << std::endl;
        std::cout << func(a, b) << std::endl;
    }
}

int main(int argc, char **argv)
{
    int result = 0;
    /* initialize random seed */
    srand(time(NULL));

    run_n_times(if_else_branch, 1000);
    run_n_times(foo_branchless, 1000);

    return result;
}