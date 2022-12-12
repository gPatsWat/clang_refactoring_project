#include <iostream>

int simple_if_else(int a3, int b3)
{
    if (a3 || b3)
        return a3;
    else
    {
        return b3;
    }
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

int simple_if_else_simple_return(int a2, int b2)
{
    if (a2 || b2)
        return a2;
    else
        return b2;
}

int main()
{
    return 0;
}